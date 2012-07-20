import org.schwa.dr.*;
import org.schwa.dr.annotations.*;


@DRAnn
public class Token extends BaseAnn {
  @DRField
  public String raw;
  @DRField
  public String lemma;
  @DRField
  public String pos;

  public Token() {
    super();
  }
}
