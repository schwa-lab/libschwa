import java.util.List;

import org.schwa.dr.AnnSlice;
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
    System.out.println("Hello World!");

    DocSchema docSchema0 = DocSchema.create(Doc.class);
    System.out.println(docSchema0);

    Doc doc = new Doc();
    System.out.println(doc);
  }
}
