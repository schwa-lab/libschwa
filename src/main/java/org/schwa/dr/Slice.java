package org.schwa.dr;


public class Slice <T extends Ann> {
  public T start;
  public T stop;

  public Slice() { }
  public Slice(T start, T stop) {
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
    final Slice<T> s = (Slice<T>) o;
    return s.start.equals(start) && s.stop.equals(stop);
  }

  @Override
  public int hashCode() {
    return 31*start.hashCode() + stop.hashCode();
  }
}
