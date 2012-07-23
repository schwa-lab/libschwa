package org.schwa.dr;


public abstract class Ann {
  private byte[] lazy;
  private int lazyNElem;
  private Integer drIndex;

  protected Ann() { }

  @Override
  public boolean equals(Object o) {
    if (this == o)
      return true;
    else if (o == null)
      return false;
    else if (!(o instanceof Ann))
      return false;
    final Ann a = (Ann) o;
    if (drIndex == null || a.drIndex == null)
      return false;
    return drIndex == a.drIndex;
  }

  public final Integer getDRIndex() {
    return drIndex;
  }

  public final byte[] getLazy() {
    return lazy;
  }

  public final int getLazyNElem() {
    return lazyNElem;
  }

  final void setDRIndex(Integer index) {
    drIndex = index;
  }
}
