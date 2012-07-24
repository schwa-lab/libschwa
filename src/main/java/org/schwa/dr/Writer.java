package org.schwa.dr;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import java.util.List;

import org.msgpack.MessagePack;
import org.msgpack.packer.Packer;

import org.schwa.dr.Ann;
import org.schwa.dr.Slice;
import org.schwa.dr.runtime.RTAnnSchema;
import org.schwa.dr.runtime.RTFieldSchema;
import org.schwa.dr.runtime.RTManager;
import org.schwa.dr.runtime.RTStoreSchema;
import org.schwa.dr.schemas.DocSchema;
import org.schwa.dr.schemas.FieldSchema;


public class Writer {
  private final OutputStream out;
  private final DocSchema docSchema;
  private final MessagePack msgpack;
  private final Packer packer;

  private static class WriteHelper {
    public static boolean write(final Packer p, final RTFieldSchema field, final Ann ann) throws IOException {
      final FieldSchema def = field.getDef();
      final int fieldId = field.getFieldId();
      final Object value = def.getFieldValue(ann);

      switch (def.getFieldType()) {
      case PRIMITIVE:
        break;
      case SLICE:
        return writeSlice(p, fieldId, (Slice) value);
      case ANN_SLICE:
        return writeAnnSlice(p, fieldId, (AnnSlice<? extends Ann>) value);
      case POINTER:
        return writePointer(p, fieldId, (Ann) value);
      case POINTERS:
        return writePointers(p, fieldId, (List<? extends Ann>) value);
      case STORE:
        throw new AssertionError("Field type of type STORE should never exist here");
      default:
        throw new AssertionError("Field type is unknown (" + def.getFieldType() + ")");
      }

      final Class<?> type = def.getField().getType();
      if (type.equals(String.class))
        return write(p, fieldId, (String) value);
      else if (type.equals(byte.class) || type.equals(Byte.class)) {
        final Byte v = (Byte) value;
        if (v != null) {
          p.write(fieldId);
          p.write(v.byteValue());
          return true;
        }
      }
      else if (type.equals(char.class) || type.equals(Character.class)) {
        final Character v = (Character) value;
        if (v != null) {
          p.write(fieldId);
          p.write(v.charValue());
          return true;
        }
      }
      else if (type.equals(short.class) || type.equals(Short.class)) {
        final Short v = (Short) value;
        if (v != null) {
          p.write(fieldId);
          p.write(v.shortValue());
          return true;
        }
      }
      else if (type.equals(int.class) || type.equals(Integer.class)) {
        final Integer v = (Integer) value;
        if (v != null) {
          p.write(fieldId);
          p.write(v.intValue());
          return true;
        }
      }
      else if (type.equals(long.class) || type.equals(Long.class)) {
        final Long v = (Long) value;
        if (v != null) {
          p.write(fieldId);
          p.write(v.longValue());
          return true;
        }
      }
      else if (type.equals(float.class) || type.equals(Float.class)) {
        final Float v = (Float) value;
        if (v != null) {
          p.write(fieldId);
          p.write(v.floatValue());
          return true;
        }
      }
      else if (type.equals(double.class) || type.equals(Double.class)) {
        final Double v = (Double) value;
        if (v != null) {
          p.write(fieldId);
          p.write(v.doubleValue());
          return true;
        }
      }
      else if (type.equals(boolean.class) || type.equals(Boolean.class)) {
        final Boolean v = (Boolean) value;
        if (v != null) {
          p.write(fieldId);
          p.write(v.booleanValue());
          return true;
        }
      }
      else
        throw new RuntimeException("Unknown type of field (" + type + ")");
      return false;
    }

    private static boolean writeAnnSlice(final Packer p, final int fieldId, final AnnSlice<? extends Ann> slice) throws IOException {
      p.writeArrayBegin(2);
      p.write(slice.start.getDRIndex());
      p.write(slice.stop.getDRIndex() - slice.start.getDRIndex());
      p.writeArrayEnd();
      return true;
    }

    private static boolean writePointer(final Packer p, final int fieldId, final Ann ann) throws IOException {
      if (ann == null)
        return false;
      p.write(ann.getDRIndex());
      return true;
    }

    private static boolean writePointers(final Packer p, final int fieldId, final List<? extends Ann> annotations) throws IOException {
      if (annotations == null || annotations.isEmpty())
        return false;
      p.writeArrayBegin(annotations.size());
      for (Ann ann : annotations)
        p.write(ann.getDRIndex());
      p.writeArrayEnd();
      return true;
    }

    private static boolean writeSlice(final Packer p, final int fieldId, final Slice slice) throws IOException {
      p.writeArrayBegin(2);
      p.write(slice.start);
      p.write(slice.stop - slice.start);
      p.writeArrayEnd();
      return true;
    }

    private static boolean write(final Packer p, final int fieldId, final String s) throws IOException {
      if (s == null || s.isEmpty())
        return false;
      p.write(fieldId);
      p.write(s);
      return true;
    }
  }

  public Writer(OutputStream out, DocSchema docSchema) {
    this.out = out;
    this.docSchema = docSchema;
    this.msgpack = new MessagePack();
    this.packer = msgpack.createPacker(out);
  }

  public void write(final Doc doc) throws IOException {
    final RTManager rt = doc.createOrMergeRT(docSchema);
    final RTAnnSchema rtDocSchema = rt.getDocSchema();

    // <klasses>
    writeKlassesHeader(rt.getSchemas(), rtDocSchema);

    // <stores>
    writeStoresHeader(rtDocSchema.getStores(), doc);

    // <doc_instance> ::= <instances_nbytes> <instance>
    {
      final ByteArrayOutputStream bos = new ByteArrayOutputStream();
      final DataOutputStream dos = new DataOutputStream(bos);
      writeInstance(doc, rtDocSchema, doc, dos);
      packer.write(bos.size());
      bos.writeTo(out);
    }

    // <instances_groups> ::= <instances_group>*
    for (RTStoreSchema store : rtDocSchema.getStores()) {
      // <instances_group> ::= <instances_nbytes> <instances>
      if (store.isLazy()) {
        final byte[] lazy = store.getLazyData();
        packer.write(lazy.length);
        packer.flush();
        out.write(lazy, 0, lazy.length);
      }
      else {
        final RTAnnSchema storedKlass = store.getStoredKlass();
        final Store<? extends Ann> annotations = store.getDef().getStore(doc);

        final ByteArrayOutputStream bos = new ByteArrayOutputStream();
        final DataOutputStream dos = new DataOutputStream(bos);
        writeArrayBegin(dos, annotations.size());
        for (Ann ann : annotations)
          writeInstance(ann, storedKlass, doc, dos);
        packer.write(bos.size());
        bos.writeTo(out);
      }
    }

    // flush since we've written a whole document
    out.flush();
  }

  private void writeKlassesHeader(final List<RTAnnSchema> schemas, final RTAnnSchema rtDocSchema) throws IOException {
    // <klasses> ::= [ <klass> ]
    packer.writeArrayBegin(schemas.size());
    int i = 0;
    for (RTAnnSchema schema : schemas) {
      // <klass> ::= ( <klass_name>, <fields> )
      packer.writeArrayBegin(2);
      if (schema.getKlassId() != i)
        throw new AssertionError();

      // <klass_name>
      if (schema == rtDocSchema)
        packer.write("__meta__");
      else if (schema.isLazy())
        packer.write(schema.getSerial());
      else
        packer.write(schema.getDef().getSerial());

      // <fields> ::= [ <field> ]
      packer.writeArrayBegin(schema.getFields().size());
      for (RTFieldSchema field : schema.getFields()) {
        // <field> ::= { <field_type> : <field_val> }
        final int nfields = 1 + (field.isPointer() ? 1 : 0) + (field.isSlice() ? 1 : 0);
        packer.writeMapBegin(nfields);

        // <field_type> ::= 0 # NAME => the name of the field
        packer.write((byte) 0);
        if (field.isLazy())
          packer.write(field.getSerial());
        else
          packer.write(field.getDef().getSerial());

        // <field_type> ::= 1 # POINTER_TO => the <store_id> that this field points into
        if (field.isPointer()) {
          packer.write((byte) 1);
          packer.write(field.getContainingStore().getStoreId());
        }

        // <field_type> ::= 2 # IS_SLICE => whether or not this field is a "Slice" field
        if (field.isSlice()) {
          packer.write((byte) 2);
          packer.write(true);
        }

        packer.writeMapEnd(); // <field>
      }
      packer.writeArrayEnd(); // <fields>

      packer.writeArrayEnd(); // <klass>
      i++;
    }
    packer.writeArrayEnd(); // <klasses>
  }

  public void writeStoresHeader(final List<RTStoreSchema> stores, final Doc doc) throws IOException {
    // <stores> ::= [ <store> ]
    packer.writeArrayBegin(stores.size());
    for (RTStoreSchema store : stores) {
      // <store> ::= ( <store_name>, <type_id>, <store_nelem> )
      packer.writeArrayBegin(3);

      // <store_name>
      if (store.isLazy())
        packer.write(store.getSerial());
      else
        packer.write(store.getDef().getSerial());

      // <type_id>
      packer.write(store.getStoredKlass().getKlassId());

      // <store_nelem>
      if (store.isLazy())
        packer.write(store.getLazyNElem());
      else
        packer.write(store.getDef().size(doc));

      packer.writeArrayEnd(); // <store>
    }
    packer.writeArrayEnd(); // <stores>
  }

  private void writeInstance(final Ann ann, final RTAnnSchema schema, final Doc doc, final DataOutputStream out) throws IOException {
    final ByteArrayOutputStream bos = new ByteArrayOutputStream();
    final Packer p = msgpack.createPacker(bos);

    int nNewElem = 0;
    for (RTFieldSchema field : schema.getFields())
      if (!field.isLazy() && WriteHelper.write(p, field, ann))
        nNewElem++;
    p.flush();

    final int nElem = nNewElem + ann.getLazyNElem();
    writeMapBegin(out, nElem);
    if (nElem != 0) {
      if (ann.getLazyNElem() != 0) {
        final byte[] lazy = ann.getLazy();
        out.write(lazy, 0, lazy.length);
      }
      bos.writeTo(out);
    }
  }

  private static void writeArrayBegin(final DataOutputStream out, final int size) throws IOException {
    if (size < 16)
      out.write((byte) (0x90 | size));
    else if (size < 65536) {
      out.write((byte) 0xdc);
      out.writeShort((short) size);
    }
    else {
      out.write((byte) 0xdd);
      out.writeInt(size);
    }
  }

  private static void writeMapBegin(final DataOutputStream out, final int size) throws IOException {
    if (size < 16)
      out.write((byte) (0x80 | size));
    else if (size < 65536) {
      out.write((byte) 0xde);
      out.writeShort((short) size);
    }
    else {
      out.write((byte) 0xdf);
      out.writeInt(size);
    }
  }
}
