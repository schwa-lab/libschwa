package org.schwa.dr;

import java.lang.reflect.Field;

import java.io.ByteArrayOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.io.InputStream;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.msgpack.MessagePack;
import org.msgpack.packer.Packer;
import org.msgpack.type.Value;
import org.msgpack.unpacker.Unpacker;

import org.schwa.dr.runtime.RTAnnSchema;
import org.schwa.dr.runtime.RTFactory;
import org.schwa.dr.runtime.RTFieldSchema;
import org.schwa.dr.runtime.RTManager;
import org.schwa.dr.runtime.RTStoreSchema;


public final class Reader <T extends Doc> implements Iterable<T>, Iterator<T> {
  private final InputStream in;
  private final DocSchema docSchema;
  private final MessagePack msgpack;
  private final Unpacker unpacker;
  private T doc;

  private static class ReadHelper {
    public static void read(final RTFieldSchema rtFieldSchema, final Ann ann, final Doc doc, final Unpacker unpacker) throws IOException, IllegalAccessException {
      final FieldSchema fieldSchema = rtFieldSchema.getDef();
      final Field field = fieldSchema.getField();
      final RTStoreSchema rtStoreSchema = rtFieldSchema.getContainingStore();
      final StoreSchema storeSchema = (rtStoreSchema == null) ? null : rtStoreSchema.getDef();
      if (rtFieldSchema.isPointer() && rtStoreSchema.isLazy())
        throw new ReaderException("Pointer field '" + field + "' cannot point into a lazy store");
      switch (fieldSchema.getFieldType()) {
      case PRIMITIVE:
        readPrimitive(field, ann, unpacker);
        break;
      case BYTE_SLICE:
        readByteSlice(field, ann, unpacker);
        break;
      case SLICE:
        readSlice(field, ann, storeSchema, doc, unpacker);
        break;
      case POINTER:
        readPointer(field, ann, storeSchema, doc, unpacker);
        break;
      case POINTERS:
        readPointers(field, ann, storeSchema, doc, unpacker);
        break;
      default:
        throw new AssertionError("Field type is unknown (" + fieldSchema.getFieldType() + ")");
      }
    }

    private static void readPrimitive(final Field field, final Ann ann, final Unpacker unpacker) throws IOException, IllegalAccessException {
      final Class<?> type = field.getType();
      if (type.equals(String.class)) {
        final String value = unpacker.readString();
        field.set(ann, value);
      }
      else if (type.equals(byte.class) || type.equals(Byte.class)) {
        final byte value = unpacker.readByte();
        field.set(ann, value);
      }
      else if (type.equals(char.class) || type.equals(Character.class)) {
        final char value = (char) unpacker.readInt();
        field.set(ann, value);
      }
      else if (type.equals(short.class) || type.equals(Short.class)) {
        final short value = unpacker.readShort();
        field.set(ann, value);
      }
      else if (type.equals(int.class) || type.equals(Integer.class)) {
        final int value = unpacker.readInt();
        field.set(ann, value);
      }
      else if (type.equals(long.class) || type.equals(Long.class)) {
        final long value = unpacker.readLong();
        field.set(ann, value);
      }
      else if (type.equals(float.class) || type.equals(Float.class)) {
        final float value = unpacker.readFloat();
        field.set(ann, value);
      }
      else if (type.equals(double.class) || type.equals(Double.class)) {
        final double value = unpacker.readDouble();
        field.set(ann, value);
      }
      else if (type.equals(boolean.class) || type.equals(Boolean.class)) {
        final boolean value = unpacker.readBoolean();
        field.set(ann, value);
      }
      else
        throw new ReaderException("Unknown type (" + type + ") of field '" + field + "'");
    }

    private static void readByteSlice(final Field field, final Ann ann, final Unpacker unpacker) throws IOException, IllegalAccessException {
      final int npair = unpacker.readArrayBegin();
      if (npair != 2)
        throw new ReaderException("Invalid sized list read in for SLICE: expected 2 elements but found " + npair);
      final long a = unpacker.readLong();
      final long b = unpacker.readLong();
      ByteSlice slice = (ByteSlice) field.get(ann);
      if (slice == null) {
        slice = new ByteSlice();
        field.set(ann, slice);
      }
      slice.start = a;
      slice.stop = a + b;
      unpacker.readArrayEnd();
    }

    private static void readPointer(final Field field, final Ann ann, final StoreSchema storeSchema, final Doc doc, final Unpacker unpacker) throws IOException, IllegalAccessException {
      final Store<? extends Ann> store = storeSchema.getStore(doc);
      final int index = unpacker.readInt();
      field.set(ann, store.get(index));
    }

    private static void readPointers(final Field field, final Ann ann, final StoreSchema storeSchema, final Doc doc, final Unpacker unpacker) throws IOException, IllegalAccessException {
      final Store<? extends Ann> store = storeSchema.getStore(doc);
      List<Ann> list = new ArrayList<Ann>();
      final int nitems = unpacker.readArrayBegin();
      for (int i = 0; i != nitems; i++) {
        final int index = unpacker.readInt();
        list.add(store.get(index));
      }
      unpacker.readArrayEnd();
      field.set(ann, list);
    }

    private static void readSlice(final Field field, final Ann ann, final StoreSchema storeSchema, final Doc doc, final Unpacker unpacker) throws IOException, IllegalAccessException {
      final Store<? extends Ann> store = storeSchema.getStore(doc);
      final int npair = unpacker.readArrayBegin();
      if (npair != 2)
        throw new ReaderException("Invalid sized list read in for SLICE: expected 2 elements but found " + npair);
      final int a = unpacker.readInt();
      final int b = unpacker.readInt();
      Slice slice = (Slice) field.get(ann);
      if (slice == null) {
        slice = new Slice();
        field.set(ann, slice);
      }
      slice.start = store.get(a);
      slice.stop = store.get(a + b);
      unpacker.readArrayEnd();
    }
  }


  public Reader(InputStream in, DocSchema docSchema) {
    this.in = in;
    this.docSchema = docSchema;
    this.msgpack = new MessagePack();
    this.unpacker = msgpack.createUnpacker(in);
    readNext();
  }

  @Override
  public boolean hasNext() {
    return doc != null;
  }

  @Override
  public Iterator<T> iterator() {
    return this;
  }

  @Override
  public T next() {
    final T doc = this.doc;
    readNext();
    return doc;
  }

  @Override
  public void remove() {
    throw new UnsupportedOperationException();
  }

  private void readNext() {
    try {
      _readNext();
    }
    catch (IOException e) {
      throw new RuntimeException(e);
    }
    catch (InstantiationException e) {
      throw new RuntimeException(e);
    }
    catch (IllegalAccessException e) {
      throw new RuntimeException(e);
    }
  }

  private void _readNext() throws IOException, InstantiationException, IllegalAccessException {
    // construct the lazy runtime manager for the document
    final RTManager rt = RTFactory.create();
    doc = (T) docSchema.getKlass().newInstance();
    doc.setRT(rt);

    // map of each of the registered types
    Map<String, AnnSchema> klassNameMap = new HashMap<String, AnnSchema>();
    klassNameMap.put("__meta__", docSchema);
    for (AnnSchema ann : docSchema.getSchemas())
      klassNameMap.put(ann.getSerial(), ann);

    // keeping track of the temporary mapping for the storeId's for fields
    Map<RTFieldSchema, Integer> rtFieldSchemaStoreIds = new HashMap<RTFieldSchema, Integer>();

    // keep track of the klass_id of __meta__
    Integer klassIdMeta = null;

    // read the klasses header
    // <klasses> ::= [ <klass> ]
    int nklasses;
    try {
      nklasses = unpacker.readArrayBegin();
    }
    catch (EOFException e) {
      doc = null;
      return;
    }
    for (int k = 0; k != nklasses; k++) {
      // <klass> ::= ( <klass_name>, <fields> )
      final int npair = unpacker.readArrayBegin();
      if (npair != 2)
        throw new ReaderException("Invalid sized tuple read in: expected 2 elements but found " + npair);

      // read in the class name and check that we have a registered class with this name
      RTAnnSchema rtAnnSchema;
      final String klassName = unpacker.readString();
      final AnnSchema schema = klassNameMap.get(klassName);
      if (schema == null)
        rtAnnSchema = new RTAnnSchema(k, klassName);
      else
        rtAnnSchema = new RTAnnSchema(k, klassName, schema);
      rt.addAnn(rtAnnSchema);

      // keep track of the klass_id of __meta__
      if (klassName.equals("__meta__"))
        klassIdMeta = k;

      // <fields> ::= [ <field> ]
      final int nfields = unpacker.readArrayBegin();
      for (int f = 0; f != nfields; f++) {
        String fieldName = null;
        int storeId = 0;
        boolean isPointer = false, isSlice = false;

        // <field> ::= { <field_type> : <field_val> }
        final int nitems = unpacker.readMapBegin();
        for (int i = 0; i != nitems; ++i) {
          final byte key = unpacker.readByte();
          switch (key) {
          case 0:
            fieldName = unpacker.readString();
            break;
          case 1:
            storeId = unpacker.readInt();
            isPointer = true;
            break;
          case 2:
            isSlice = unpacker.readBoolean();
            break;
          default:
            throw new ReaderException("Unknown value " + ((int) key) +  " as key in <field> map");
          }
        }
        unpacker.readMapEnd();
        if (fieldName == null)
          throw new ReaderException("Field number " + (f + 1) + " did not contain a NAME key");

        // see if the read in field exists on the registered class's schema
        RTFieldSchema rtFieldSchema;
        if (rtAnnSchema.isLazy()) {
          rtFieldSchema = new RTFieldSchema(f, fieldName, null, isSlice);
          rtFieldSchemaStoreIds.put(rtFieldSchema, isPointer ? storeId : null);
        }
        else {
          // try and find the field on the registered class
          FieldSchema fieldDef = null;
          for (FieldSchema field : rtAnnSchema.getDef().getFields()) {
            if (field.getSerial().equals(fieldName)) {
              fieldDef = field;
              break;
            }
          }

          rtFieldSchema = new RTFieldSchema(f, fieldName, null, isSlice, fieldDef);
          rtFieldSchemaStoreIds.put(rtFieldSchema, isPointer ? storeId : null);

          // perform some sanity checks that the type of data on the stream is what we're expecting
          if (fieldDef != null) {
            if (isPointer != fieldDef.isPointer())
              throw new ReaderException("Field '" + fieldName + "' of class '" + klassName + "' has IS_POINTER as " + isPointer + " on the stream, but " + fieldDef.isPointer() + " on the class's field");
            if (isSlice != fieldDef.isSlice())
              throw new ReaderException("Field '" + fieldName + "' of class '" + klassName + "' has IS_SLICE as " + isSlice + " on the stream, but " + fieldDef.isSlice() + " on the class's field");
          }
        }

        rtAnnSchema.addField(rtFieldSchema);
      } // for each field
      unpacker.readArrayEnd();
      unpacker.readArrayEnd();
    } // for each klass
    unpacker.readArrayEnd();

    if (klassIdMeta == null)
      throw new ReaderException("Did not read in a __meta__ class");
    final RTAnnSchema rtDocSchema = rt.getSchema(klassIdMeta);
    rt.setDocSchema(rtDocSchema);

    // read the stores header
    // <stores> ::= [ <store> ]
    final int nstores = unpacker.readArrayBegin();
    for (int n = 0; n != nstores; n++) {
      // <store> ::= ( <store_name>, <klass_id>, <store_nelem> )
      final int ntriple = unpacker.readArrayBegin();
      if (ntriple != 3)
        throw new ReaderException("Invalid sized tuple read in: expected 3 elements but found " + ntriple);
      final String storeName = unpacker.readString();
      final int klassId = unpacker.readInt();
      final int nelem = unpacker.readInt();
      unpacker.readArrayEnd();

      // sanity check on the value of the klassId
      if (klassId >= rt.getSchemas().size())
        throw new ReaderException("klassId value " + klassId + " >= number of klasses (" + rt.getSchemas().size() + ")");

      // lookup the store on the Doc class
      StoreSchema def = null;
      for (StoreSchema store : docSchema.getStores()) {
        if (store.getSerial().equals(storeName)) {
          def = store;
          break;
        }
      }

      final RTAnnSchema klass = rt.getSchema(klassId);
      RTStoreSchema rtStoreSchema;
      if (def == null)
        rtStoreSchema = new RTStoreSchema(n, storeName, klass, null, nelem);
      else
        rtStoreSchema = new RTStoreSchema(n, storeName, klass, def);
      rtDocSchema.addStore(rtStoreSchema);

      // ensure that the stream store and the static store agree on the klass they're storing
      if (!rtStoreSchema.isLazy()) {
        final Class<? extends Ann> storeKlass = def.getStoredKlass();
        final Class<? extends Ann> klassKlass = klass.getDef().getKlass();
        if (!storeKlass.equals(klassKlass))
          throw new ReaderException("Store '" + storeName + "' points to " + storeKlass + " but the stream says it points to " + klassKlass);

        // resize the store to house the correct number of instances
        def.resize(nelem, doc);
      }
    }
    unpacker.readArrayEnd();


    // back-fill each of the pointer fields to point to the actual RTStoreSchema instance
    for (RTAnnSchema rtSchema : rt.getSchemas()) {
      for (RTFieldSchema rtField : rtSchema.getFields()) {
        final Integer storeIdObj = rtFieldSchemaStoreIds.get(rtField);
        if (storeIdObj == null)
          continue;

        // sanity check on the value of store_id
        final int storeId = storeIdObj;
        if (storeId >= rtDocSchema.getStores().size())
          throw new ReaderException("storeId value " + storeId + " >= number of stores (" + rtDocSchema.getStores().size()+ ")");
        final RTStoreSchema rtStore = rtDocSchema.getStore(storeId);

        rtField.setContainingStore(rtStore);
      }
    }


    // read the document instance
    // <doc_instance> ::= <instances_nbytes> <instance>
    {
      final long nbytes = unpacker.readLong();

      final ByteArrayOutputStream lazyBOS = new ByteArrayOutputStream();
      final Packer lazyPacker = msgpack.createPacker(lazyBOS);
      int lazyNElem = 0;

      // <instance> ::= { <field_id> : <obj_val> }
      final int nitems = unpacker.readMapBegin();
      for (int i = 0; i != nitems; i++) {
        final int key = unpacker.readInt();
        final RTFieldSchema field = rtDocSchema.getField(key);

        // deserialize the field value if required
        if (field.isLazy()) {
          final Value lazyValue = unpacker.readValue();
          lazyPacker.write(key);
          lazyValue.writeTo(lazyPacker);
          lazyNElem++;
        }
        else
          ReadHelper.read(field, doc, doc, unpacker);
      }
      unpacker.readMapEnd();

      if (lazyNElem != 0) {
        lazyPacker.flush();
        doc.setDRLazy(lazyBOS.toByteArray());
        doc.setDRLazyNElem(lazyNElem);
      }
    }


    // read the store instances
    // <instances_groups> ::= <instances_group>*
    for (RTStoreSchema rtStoreSchema : rtDocSchema.getStores()) {
      // <instances_group>  ::= <instances_nbytes> <instances>
      final long nbytes = unpacker.readLong();

      // <instances> ::= [ <instance> ]
      if (rtStoreSchema.isLazy()) {
        final ByteArrayOutputStream bos = new ByteArrayOutputStream();
        final Packer packer = msgpack.createPacker(bos);
        final Value value = unpacker.readValue();
        value.writeTo(packer);
        packer.flush();
        rtStoreSchema.setLazy(bos.toByteArray());
      }
      else {
        final RTAnnSchema storedKlass = rtStoreSchema.getStoredKlass();
        final Store<? extends Ann> store = rtStoreSchema.getDef().getStore(doc);

        final int nobjects = unpacker.readArrayBegin();
        for (int o = 0; o != nobjects; o++) {
          final Ann ann = store.get(o);
          final ByteArrayOutputStream lazyBOS = new ByteArrayOutputStream();
          final Packer lazyPacker = msgpack.createPacker(lazyBOS);
          int lazyNElem = 0;

          // <instance> ::= { <field_id> : <obj_val> }
          final int nitems = unpacker.readMapBegin();
          for (int i = 0; i != nitems; i++) {
            final int key = unpacker.readInt();
            final RTFieldSchema field = storedKlass.getField(key);

            // deserialize the field value if required
            if (field.isLazy()) {
              final Value lazyValue = unpacker.readValue();
              lazyPacker.write(key);
              lazyValue.writeTo(lazyPacker);
              lazyNElem++;
            }
            else
              ReadHelper.read(field, ann, doc, unpacker);
          }
          unpacker.readMapEnd();

          if (lazyNElem != 0) {
            lazyPacker.flush();
            ann.setDRLazy(lazyBOS.toByteArray());
            ann.setDRLazyNElem(lazyNElem);
          }
        }
        unpacker.readArrayEnd();
      }
    }
  }
}
