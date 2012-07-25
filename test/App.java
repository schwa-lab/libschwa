import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;

import java.util.List;

import org.schwa.dr.*;


@dr.Ann(serial="MyChunk")
class Chunk extends AbstractAnn {
  @dr.Pointer(store="tokens")  public Slice<Token> span;
  @dr.Pointer(store="chunks")  public Chunk parent;
  @dr.Pointer(store="chunks")  public List<Chunk> children;
  @dr.Field(serial="gold_tag") public String tag;
}


public class App {
  public static void main(String[] args) throws IOException {
    DocSchema docSchema = DocSchema.create(Doc.class);
    System.out.println(docSchema);

    Doc doc = new Doc();

    Token t = new Token();
    t.span = new ByteSlice(0, 5);
    t.raw = "Hello";
    doc.tokens.add(t);

    t = new Token();
    t.span = new ByteSlice(6, 10);
    t.raw = "world";
    doc.tokens.add(t);

    ByteArrayOutputStream bos = new ByteArrayOutputStream();
    Writer writer = new Writer(bos, docSchema);
    writer.write(doc);
    final byte[] bytes = bos.toByteArray();
    for (int i = 0; i != bytes.length; ++i) {
      if (i != 0 && i % 16 == 0)
        System.out.println();
      System.out.printf("%02x ", (int)(bytes[i] & 0xff));
    }
    System.out.println();
    System.out.println();

    ByteArrayInputStream bis = new ByteArrayInputStream(bytes);
    Reader<Doc> reader = new Reader<Doc>(bis, docSchema);
    for (Doc d : reader) {
      System.out.println("read in " + d);
      System.out.println("tokens = " + d.tokens.size());
      for (Token x : d.tokens) {
        System.out.println("  token = " + x);
      }
    }
  }
}
