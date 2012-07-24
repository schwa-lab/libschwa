package org.schwa.dr;


public class ByteSlice {
  public long start;
  public long stop;

  public ByteSlice() { }
  public ByteSlice(long start, long stop) {
    this.start = start;
    this.stop = stop;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o)
      return true;
    else if (o == null)
      return false;
    else if (!(o instanceof ByteSlice))
      return false;
    final ByteSlice s = (ByteSlice) o;
    return s.start == start && s.stop == stop;
  }

  @Override
  public int hashCode() {
    return (int)(31*start + stop);
  }
}
