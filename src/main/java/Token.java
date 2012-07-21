import org.schwa.dr.Ann;
import org.schwa.dr.Slice;
import org.schwa.dr.annotations.DRAnn;
import org.schwa.dr.annotations.DRField;


@DRAnn
public class Token extends Ann {
  @DRField
  public Slice span;
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
