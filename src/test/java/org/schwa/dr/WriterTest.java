package org.schwa.dr;

import java.io.*;

import org.junit.Assert;
import org.junit.Test;

import org.schwa.dr.*;
import org.schwa.dr.annotations.*;
import org.schwa.dr.schemas.DocSchema;


public class WriterTest {

  @DRDoc
  public static class DocWithField extends Doc {
    @DRField public String name;
  }

  @DRDoc
  public static class DocWithFieldSerial extends Doc {
    @DRField(serial="filename")
    public String name;
  }


  @Test
  public void test_DocWithField__name_isnull() throws IOException {
    ByteArrayOutputStream actual = new ByteArrayOutputStream();
    DocSchema schema = DocSchema.create(DocWithField.class);
    Writer writer = new Writer(actual, schema);
    byte[] str;

    DocWithField d = new DocWithField();
    writer.write(d);

    ByteArrayOutputStream expected = new ByteArrayOutputStream();
    DataOutputStream dos = new DataOutputStream(expected);

    dos.write((byte) 0x91);  // <klasses>: 1-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
    str = "__meta__".getBytes("UTF-8"); dos.write(str, 0, str.length);
    dos.write((byte) 0x91);  // <fields>: 1-element array
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa4);  // 4-bytes of utf-8 encoded "name"
    str = "name".getBytes("UTF-8"); dos.write(str, 0, str.length);
    dos.write((byte) 0x90);  // <stores>: 0-element array
    dos.write((byte) 0x01);  // <instance_nbytes>: 1 byte after this
    dos.write((byte) 0x80);  // <instance>: 0-element map

    Assert.assertArrayEquals(expected.toByteArray(), actual.toByteArray());
  }


  @Test
  public void test_DocWithField__name() throws IOException {
    ByteArrayOutputStream actual = new ByteArrayOutputStream();
    DocSchema schema = DocSchema.create(DocWithField.class);
    Writer writer = new Writer(actual, schema);
    byte[] str;

    DocWithField d = new DocWithField();
    d.name = "/etc/passwd";
    writer.write(d);

    ByteArrayOutputStream expected = new ByteArrayOutputStream();
    DataOutputStream dos = new DataOutputStream(expected);

    dos.write((byte) 0x91);  // <klasses>: 1-element array
    dos.write((byte) 0x92);  // <klass>: 2-element array
    dos.write((byte) 0xa8);  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
    str = "__meta__".getBytes("UTF-8"); dos.write(str, 0, str.length);
    dos.write((byte) 0x91);  // <fields>: 1-element array
    dos.write((byte) 0x81);  // <field>: 1-element map
    dos.write((byte) 0x00);  // 0: NAME
    dos.write((byte) 0xa4);  // 4-bytes of utf-8 encoded "name"
    str = "name".getBytes("UTF-8"); dos.write(str, 0, str.length);
    dos.write((byte) 0x90);  // <stores>: 0-element array
    dos.write((byte) 0x0e);  // <instance_nbytes>: 14 bytes after this
    dos.write((byte) 0x81);  // <instance>: 1-element map
    dos.write((byte) 0x00);  // 0: field number 0 (=> name;
    dos.write((byte) 0xab);  // utf-8 encoded "/etc/passwd"
    str = "/etc/passwd".getBytes("UTF-8"); dos.write(str, 0, str.length);

    Assert.assertArrayEquals(expected.toByteArray(), actual.toByteArray());
  }


  private static void compareByteArrays(final byte[] e, final byte[] a) {
    int ei = 0, ai = 0;
    while (ei != e.length || ai != a.length) {
      for (int i = 0; i != 16; ++i) {
        if (ei != e.length)
          System.out.printf("%02x ", (int)(e[ei++] & 0xff));
        else
          System.out.print("   ");
      }
      System.out.print("| ");
      for (int i = 0; i != 16; ++i) {
        if (ai != a.length)
          System.out.printf("%02x ", (int)(a[ai++] & 0xff));
        else
          System.out.print("   ");
      }
      System.out.println();
    }
    System.out.println();
  }
}
