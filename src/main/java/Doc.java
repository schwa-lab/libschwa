import org.schwa.dr.*;
import org.schwa.dr.annotations.*;


@DRDoc
public class Doc extends org.schwa.dr.Doc {
  @DRStore
  public Store<Token> tokens;
  @DRStore
  public Store<Chunk> chunks;

  @DRField
  public String filename;

  public Doc() {
    tokens = new Store<Token>();
    chunks = new Store<Chunk>();
  }
}
