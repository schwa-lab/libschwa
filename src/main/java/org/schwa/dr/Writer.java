package org.schwa.dr;

import java.io.OutputStream;

import org.msgpack.MessagePack;
import org.msgpack.packer.Packer;

import org.schwa.dr.runtime.RTManager;
import org.schwa.dr.schemas.DocSchema;


public class Writer {
  private final OutputStream out;
  private final DocSchema docSchema;
  private final MessagePack msgpack;
  private final Packer packer;

  public Writer(OutputStream out, DocSchema docSchema) {
    this.out = out;
    this.docSchema = docSchema;
    this.msgpack = new MessagePack();
    this.packer = msgpack.createPacker(out);
  }

  public void write(final Doc doc) {
    final RTManager rt = doc.createOrMergeRT(docSchema);
  }
}
