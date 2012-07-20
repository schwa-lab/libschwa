import org.schwa.dr.*;
import org.schwa.dr.annotations.*;


@DRDoc
public class Doc extends BaseDoc {
  @DRStore
  public Store<Token> tokens;
  @DRStore
  public Store<Chunk> chunks;

  @DRField
  public String filename;
}
