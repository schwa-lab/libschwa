import org.schwa.dr.*;
import org.schwa.dr.annotations.*;


// need a place for lazy
// need _dr_index or something like that
@DRAnn(serial="MyChunk")
class Chunk extends BaseAnn {
  @DRPointer(store="tokens")
  public Slice<Token> span;
  @DRField(serial="gold_tag")
  public String tag;
}


public class App {
  public static void main(String[] args) {
    System.out.println("Hello World!");
  }
}
