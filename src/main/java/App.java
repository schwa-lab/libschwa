import java.io.ByteArrayOutputStream;
import java.util.List;

import org.schwa.dr.AnnSlice;
import org.schwa.dr.Slice;
import org.schwa.dr.Writer;
import org.schwa.dr.annotations.*;
import org.schwa.dr.schemas.DocSchema;


// need a place for lazy
// need _dr_index or something like that
@DRAnn(serial="MyChunk")
class Chunk extends org.schwa.dr.Ann {
  @DRPointer(store="tokens")
  public AnnSlice<Token> span;
  @DRPointer(store="chunks")
  public Chunk parent;
  @DRPointer(store="chunks")
  public List<Chunk> children;
  @DRField(serial="gold_tag")
  public String tag;
}


public class App {
  public static void main(String[] args) {
    DocSchema docSchema = DocSchema.create(Doc.class);
    System.out.println(docSchema);

    Doc doc = new Doc();

    Token t = new Token();
    t.span = new Slice(0, 5);
    t.raw = "Hello";
    doc.tokens.add(t);

    t = new Token();
    t.span = new Slice(6, 10);
    t.raw = "world";
    doc.tokens.add(t);

    ByteArrayOutputStream bos = new ByteArrayOutputStream();
    Writer writer = new Writer(bos, docSchema);
    writer.write(doc);
    final byte[] bytes = bos.toByteArray();
    for (int i = 0; i != bytes.length; ++i) {
      if (i != 0 && i % 16 == 0)
        System.out.print('\n');
      System.out.print(Integer.toString(bytes[i], 16));
      System.out.print(' ');
    }
    System.out.flush();

  }
}
