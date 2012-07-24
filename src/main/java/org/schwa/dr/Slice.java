package org.schwa.dr;


public class Slice {
  public long start;
  public long stop;

  public Slice() { }
  public Slice(long start, long stop) {
    this.start = start;
    this.stop = stop;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o)
      return true;
    else if (o == null)
      return false;
    else if (!(o instanceof Slice))
      return false;
    final Slice s = (Slice)o;
    return s.start == start && s.stop == stop;
  }

  @Override
  public int hashCode() {
    return (int)(31*start + stop);
  }
}
