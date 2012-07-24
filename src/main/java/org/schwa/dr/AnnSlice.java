package org.schwa.dr;


public class AnnSlice <T extends Ann> {
  public T start;
  public T stop;

  public AnnSlice() { }
  public AnnSlice(T start, T stop) {
    this.start = start;
    this.stop = stop;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o)
      return true;
    else if (o == null)
      return false;
    else if (!(o instanceof AnnSlice))
      return false;
    final AnnSlice<T> s = (AnnSlice<T>) o;
    return s.start.equals(start) && s.stop.equals(stop);
  }

  @Override
  public int hashCode() {
    return 31*start.hashCode() + stop.hashCode();
  }
}
