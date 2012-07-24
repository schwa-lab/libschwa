package org.schwa.dr;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import org.junit.Assert;
import org.junit.Test;


public class WriterTest {

  @dr.Doc
  public static class DocWithField extends Doc {
    @dr.Field public String name;
  }

  @dr.Doc
  public static class DocWithFieldSerial extends Doc {
    @dr.Field(serial="filename")
    public String name;
  }

  @dr.Ann
  public static class A extends Ann {
    @dr.Field public String v_str;
    @dr.Field public byte v_uint8;
    @dr.Field public boolean v_bool;
  }

  @dr.Ann
  public static class Y extends Ann {
    @dr.Pointer(store="as")
    public A p;
  }

  @dr.Ann
  public static class Z extends Ann {
    @dr.Pointer(store="as")
    public A p;
    @dr.Field
    public boolean value;
  }

  @dr.Doc
  public static class DocWithA extends Doc {
    @dr.Store public Store<A> as = new Store<A>();
  }

  @dr.Doc
  public static class DocWithAYZ extends Doc {
    @dr.Store public Store<A> as = new Store<A>();
    @dr.Store public Store<Y> ys = new Store<Y>();
    @dr.Store public Store<Z> zs = new Store<Z>();
  }


  @Test
  public void test_DocWithField__name_isnull() throws IOException {
    ByteArrayOutputStream actual = new ByteArrayOutputStream();
    DocSchema schema = DocSchema.create(DocWithField.class);
    Writer writer = new Writer(actual, schema);

    DocWithField d = new DocWithField();
    writer.write(d);

    ByteArrayOutputStream expected = new ByteArrayOutputStream();
    DataOutputStream dos = new DataOutputStream(expected);

    dos.write((byte) 0x91);  // <klasses>: 1-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
    dos.write("__meta__".getBytes("UTF-8"));
    dos.write((byte) 0x91);  // <fields>: 1-element array
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa4);  // 4-bytes of utf-8 encoded "name"
    dos.write("name".getBytes("UTF-8"));
    dos.write((byte) 0x90);  // <stores>: 0-element array
    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this
    dos.write((byte) 0x80);  // <instance>: 0-element map

    assertArrayEquals(expected.toByteArray(), actual.toByteArray());
  }


  @Test
  public void test_DocWithField__name() throws IOException {
    ByteArrayOutputStream actual = new ByteArrayOutputStream();
    DocSchema schema = DocSchema.create(DocWithField.class);
    Writer writer = new Writer(actual, schema);

    DocWithField d = new DocWithField();
    d.name = "/etc/passwd";
    writer.write(d);

    ByteArrayOutputStream expected = new ByteArrayOutputStream();
    DataOutputStream dos = new DataOutputStream(expected);

    dos.write((byte) 0x91);  // <klasses>: 1-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
    dos.write("__meta__".getBytes("UTF-8"));
    dos.write((byte) 0x91);  // <fields>: 1-element array
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa4);  // 4-bytes of utf-8 encoded "name"
    dos.write("name".getBytes("UTF-8"));
    dos.write((byte) 0x90);  // <stores>: 0-element array
    dos.write((byte) 0x0e);  // <instance_nbytes>: 14 bytes after this
    dos.write((byte) 0x81);  // <instance>: 1-element map
    dos.write((byte) 0x00);  // 0: field number 0 (=> name;
    dos.write((byte) 0xab);  // utf-8 encoded "/etc/passwd"
    dos.write("/etc/passwd".getBytes("UTF-8"));

    assertArrayEquals(expected.toByteArray(), actual.toByteArray());
  }


  @Test
  public void test_DocWithFieldSerial__name_isnull() throws IOException {
    ByteArrayOutputStream actual = new ByteArrayOutputStream();
    DocSchema schema = DocSchema.create(DocWithFieldSerial.class);
    Writer writer = new Writer(actual, schema);

    DocWithFieldSerial d = new DocWithFieldSerial();
    writer.write(d);

    ByteArrayOutputStream expected = new ByteArrayOutputStream();
    DataOutputStream dos = new DataOutputStream(expected);

    dos.write((byte) 0x91);  // <klasses>: 1-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
    dos.write("__meta__".getBytes("UTF-8"));
    dos.write((byte) 0x91);  // <fields>: 1-element array
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa8);  // 8-bytes of utf-8 encoded "filename"
    dos.write("filename".getBytes("UTF-8"));
    dos.write((byte) 0x90);  // <stores>: 0-element array
    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this
    dos.write((byte) 0x80);  // <instance>: 0-element map

    assertArrayEquals(expected.toByteArray(), actual.toByteArray());
  }


  @Test
  public void test_DocWithFieldSerial__name() throws IOException {
    ByteArrayOutputStream actual = new ByteArrayOutputStream();
    DocSchema schema = DocSchema.create(DocWithFieldSerial.class);
    Writer writer = new Writer(actual, schema);

    DocWithFieldSerial d = new DocWithFieldSerial();
    d.name = "/etc/passwd";
    writer.write(d);

    ByteArrayOutputStream expected = new ByteArrayOutputStream();
    DataOutputStream dos = new DataOutputStream(expected);

    dos.write((byte) 0x91);  // <klasses>: 1-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
    dos.write("__meta__".getBytes("UTF-8"));
    dos.write((byte) 0x91);  // <fields>: 1-element array
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa8);  // 8-bytes of utf-8 encoded "filename"
    dos.write("filename".getBytes("UTF-8"));
    dos.write((byte) 0x90);  // <stores>: 0-element array
    dos.write((byte) 0x0e);  // <instance_nbytes>: 14 bytes after this
    dos.write((byte) 0x81);  // <instance>: 1-element map
    dos.write((byte) 0x00);  // 0: field number 0 (=> name;
    dos.write((byte) 0xab);  // utf-8 encoded "/etc/passwd"
    dos.write("/etc/passwd".getBytes("UTF-8"));

    assertArrayEquals(expected.toByteArray(), actual.toByteArray());
  }


  @Test
  public void test_DocWithA__empty() throws IOException {
    ByteArrayOutputStream actual = new ByteArrayOutputStream();
    DocSchema docSchema = DocSchema.create(DocWithA.class);

    docSchema.getSchema(A.class).setSerial("writer.A");


    Writer writer = new Writer(actual, docSchema);

    DocWithA d = new DocWithA();
    writer.write(d);

    ByteArrayOutputStream expected = new ByteArrayOutputStream();
    DataOutputStream dos = new DataOutputStream(expected);

    dos.write((byte) 0x92);  // <klasses>: 2-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: utf-8 encoded "__meta__"
    dos.write("__meta__".getBytes("UTF-8"));
    dos.write((byte) 0x90);  // <fields>: 0-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: utf-8 encoded "writer.A"
    dos.write("writer.A".getBytes("UTF-8"));
    dos.write((byte) 0x93);  // <fields>: 3-element array
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa5);  // utf-8 encoded "v_str"
    dos.write("v_str".getBytes("UTF-8"));
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa7);  // utf-8 encoded "v_uint8"
    dos.write("v_uint8".getBytes("UTF-8"));
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa6);  // utf-8 encoded "v_bool"
    dos.write("v_bool".getBytes("UTF-8"));
    dos.write((byte) 0x91);  // <stores>: 1-element array
    dos.write((byte) 0x93);  // <store>: 3-element array
    dos.write((byte) 0xa2);  // <store_name>: utf-8 encoded "as"
    dos.write("as".getBytes("UTF-8"));
    dos.write((byte) 0x01);  // <klass_id>: 1
    dos.write((byte) 0x00);  // <store_nelem>: 0
    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this for the document
    dos.write((byte) 0x80);  // <instance>: 0-element map
    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this for the "as" store
    dos.write((byte) 0x90);  // <instance>: 0-element array

    assertArrayEquals(expected.toByteArray(), actual.toByteArray());
  }


  @Test
  public void test_DocWithA__four_elements() throws IOException {
    ByteArrayOutputStream actual = new ByteArrayOutputStream();
    DocSchema docSchema = DocSchema.create(DocWithA.class);

    docSchema.getSchema(A.class).setSerial("writer.A");


    Writer writer = new Writer(actual, docSchema);

    DocWithA d = new DocWithA();
    for (int i = 0; i != 4; i++)
      d.as.add(new A());
    d.as.get(0).v_str = "first";
    d.as.get(1).v_uint8 = 2;
    d.as.get(3).v_bool = true;
    writer.write(d);

    ByteArrayOutputStream expected = new ByteArrayOutputStream();
    DataOutputStream dos = new DataOutputStream(expected);

    dos.write((byte) 0x92);  // <klasses>: 2-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: utf-8 encoded "__meta__"
    dos.write("__meta__".getBytes("UTF-8"));
    dos.write((byte) 0x90);  // <fields>: 0-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: utf-8 encoded "writer.A"
    dos.write("writer.A".getBytes("UTF-8"));
    dos.write((byte) 0x93);  // <fields>: 3-element array
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa5);  // utf-8 encoded "v_str"
    dos.write("v_str".getBytes("UTF-8"));
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa7);  // utf-8 encoded "v_uint8"
    dos.write("v_uint8".getBytes("UTF-8"));
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa6);  // utf-8 encoded "v_bool"
    dos.write("v_bool".getBytes("UTF-8"));
    dos.write((byte) 0x91);  // <stores>: 1-element array
    dos.write((byte) 0x93);  // <store>: 3-element array
    dos.write((byte) 0xa2);  // <store_name>: utf-8 encoded "as"
    dos.write("as".getBytes("UTF-8"));
    dos.write((byte) 0x01);  // <klass_id>: 1
    dos.write((byte) 0x04);  // <store_nelem>: 4
    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this for the document
    dos.write((byte) 0x80);  // <instance>: 0-element map
    dos.write((byte) 0x1c);  // <instance_nbytes>: 28 byte after this for the "as" store
    dos.write((byte) 0x94);  // <instance>: 4-element array
    dos.write(new byte[]{(byte)0x83, (byte)0x00, (byte)0xa5}); // {0: 'first', 1: 0, 2: false}
    dos.write("first".getBytes("UTF-8"));
    dos.write(new byte[]{(byte)0x01, (byte)0x00, (byte)0x02, (byte)0xc2});
    dos.write(new byte[]{(byte)0x82, (byte)0x01, (byte)0x02, (byte)0x02, (byte)0xc2}); // {1: 2, 2: false}
    dos.write(new byte[]{(byte)0x82, (byte)0x01, (byte)0x00, (byte)0x02, (byte)0xc2}); // {1: 0, 2: false}
    dos.write(new byte[]{(byte)0x82, (byte)0x01, (byte)0x00, (byte)0x02, (byte)0xc3}); // {1: 0, 2: true}

    assertArrayEquals(expected.toByteArray(), actual.toByteArray());
  }


  @Test
  public void test_DocWithAYZ__empty() throws IOException {
    ByteArrayOutputStream actual = new ByteArrayOutputStream();
    DocSchema docSchema = DocSchema.create(DocWithAYZ.class);

    docSchema.getSchema(A.class).setSerial("writer.A");
    docSchema.getSchema(Y.class).setSerial("writer.Y");
    docSchema.getSchema(Z.class).setSerial("writer.Z");
    docSchema.getSchema(Z.class).getField("p").setSerial("zp");

    Writer writer = new Writer(actual, docSchema);

    DocWithAYZ d = new DocWithAYZ();
    writer.write(d);

    ByteArrayOutputStream expected = new ByteArrayOutputStream();
    DataOutputStream dos = new DataOutputStream(expected);

    dos.write((byte) 0x94);  // <klasses>: 4-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array

    dos.write((byte) 0xa8);  // <klass_name>: utf-8 encoded "__meta__"
    dos.write("__meta__".getBytes("UTF-8"));
    dos.write((byte) 0x90);  // <fields>: 0-element array

    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: utf-8 encoded "writer.A"
    dos.write("writer.A".getBytes("UTF-8"));
    dos.write((byte) 0x93);  // <fields>: 1-element array
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa5);  // utf-8 encoded "v_str"
    dos.write("v_str".getBytes("UTF-8"));
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa7);  // utf-8 encoded "v_uint8"
    dos.write("v_uint8".getBytes("UTF-8"));
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa6);  // utf-8 encoded "v_bool"
    dos.write("v_bool".getBytes("UTF-8"));

    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: utf-8 encoded "writer.Y"
    dos.write("writer.Y".getBytes("UTF-8"));
    dos.write((byte) 0x91);  // <fields>: 1-element array
    dos.write((byte) 0x82);  // <field>: 2-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa1);  // utf-8 encoded "p"
    dos.write("p".getBytes("UTF-8"));
    dos.write((byte) 0x01);  // 1: POINTER_TO
    dos.write((byte) 0x00);  // <store_id>: 0

    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: utf-8 encoded "writer.Z"
    dos.write("writer.Z".getBytes("UTF-8"));
    dos.write((byte) 0x92);  // <fields>: 2-element array
    dos.write((byte) 0x82);  // <field>: 2-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa2);  // utf-8 encoded "zp"
    dos.write("zp".getBytes("UTF-8"));
    dos.write((byte) 0x01);  // 1: POINTER_TO
    dos.write((byte) 0x00);  // <store_id>: 0
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa5);  // utf-8 encoded "value"
    dos.write("value".getBytes("UTF-8"));

    dos.write((byte) 0x93);  // <stores>: 3-element array

    dos.write((byte) 0x93);  // <store>: 3-element array
    dos.write((byte) 0xa2);  // <store_name>: utf-8 encoded "as"
    dos.write("as".getBytes("UTF-8"));
    dos.write((byte) 0x01);  // <klass_id>: 1
    dos.write((byte) 0x00);  // <store_nelem>: 0

    dos.write((byte) 0x93);  // <store>: 3-element array
    dos.write((byte) 0xa2);  // <store_name>: utf-8 encoded "ys"
    dos.write("ys".getBytes("UTF-8"));
    dos.write((byte) 0x02);  // <klass_id>: 2
    dos.write((byte) 0x00);  // <store_nelem>: 0

    dos.write((byte) 0x93);  // <store>: 3-element array
    dos.write((byte) 0xa2);  // <store_name>: utf-8 encoded "zs"
    dos.write("zs".getBytes("UTF-8"));
    dos.write((byte) 0x03);  // <klass_id>: 3
    dos.write((byte) 0x00);  // <store_nelem>: 0

    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this for the document
    dos.write((byte) 0x80);  // <instance>: 0-element map

    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this for the "as" store
    dos.write((byte) 0x90);  // <instance>: 0-element array

    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this for the "ys" store
    dos.write((byte) 0x90);  // <instance>: 0-element array

    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this for the "zs" store
    dos.write((byte) 0x90);  // <instance>: 0-element array


    assertArrayEquals(expected.toByteArray(), actual.toByteArray());
  }


  private static void assertArrayEquals(final byte[] e, final byte[] a) {
    boolean different = false;
    if (e.length != a.length)
      different = true;
    else
      for (int i = 0; i != e.length; i++)
        if (e[i] != a[i]) {
          different = true;
          break;
        }
    if (different)
      compareByteArrays(e, a);
    Assert.assertArrayEquals(e, a);
  }


  private static void compareByteArrays(final byte[] e, final byte[] a) {
    int ei = 0, ai = 0;
    System.out.print("          expected            |             actual            ||");
    System.out.println("           expected            |             actual            ");
    while (ei != e.length || ai != a.length) {
      final int ei_start = ei, ai_start = ai;
      for (int i = 0; i != 10; ++i) {
        if (ei != e.length)
          System.out.printf("%02x ", (int)(e[ei++] & 0xff));
        else
          System.out.print("   ");
      }
      System.out.print("| ");
      for (int i = 0; i != 10; ++i) {
        if (ai != a.length)
          System.out.printf("%02x ", (int)(a[ai++] & 0xff));
        else
          System.out.print("   ");
      }
      System.out.print("|| ");
      ei = ei_start;
      ai = ai_start;
      for (int i = 0; i != 10; ++i) {
        if (ei != e.length) {
          if (Character.isLetterOrDigit(e[ei]))
            System.out.printf("%2c ", (int)(e[ei++] & 0xff));
          else
            System.out.printf("%02x ", (int)(e[ei++] & 0xff));
        }
        else
          System.out.print("   ");
      }
      System.out.print("| ");
      for (int i = 0; i != 10; ++i) {
        if (ai != a.length) {
          if (Character.isLetterOrDigit(a[ai]))
            System.out.printf("%2c ", (int)(a[ai++] & 0xff));
          else
            System.out.printf("%02x ", (int)(a[ai++] & 0xff));
        }
        else
          System.out.print("   ");
      }
      System.out.println();
    }
    System.out.println();
  }
}
