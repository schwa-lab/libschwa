import org.schwa.dr.*;


@dr.Ann
public class Token extends AbstractAnn {
  @dr.Field public ByteSlice span;
  @dr.Field public String raw;
  @dr.Field public String lemma;
  @dr.Field public String pos;

  @Override
  public String toString() {
    return "Token(<" + span.start + "," + span.stop + "> '" + raw + "')";
  }
}
