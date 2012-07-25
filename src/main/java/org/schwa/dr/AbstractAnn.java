package org.schwa.dr;


public abstract class AbstractAnn implements Ann {
  private byte[] lazy;
  private int lazyNElem;
  private Integer drIndex;

  protected AbstractAnn() { }

  @Override
  public boolean equals(Object o) {
    if (this == o)
      return true;
    else if (o == null)
      return false;
    else if (!(o instanceof AbstractAnn))
      return false;
    final AbstractAnn a = (AbstractAnn) o;
    if (drIndex == null || a.drIndex == null)
      return false;
    return drIndex == a.drIndex;
  }

  @Override
  public final Integer getDRIndex() {
    return drIndex;
  }

  @Override
  public final byte[] getDRLazy() {
    return lazy;
  }

  @Override
  public final int getDRLazyNElem() {
    return lazyNElem;
  }

  @Override
  public final void setDRIndex(Integer index) {
    drIndex = index;
  }

  @Override
  public final void setDRLazy(byte[] lazy) {
    this.lazy = lazy;
  }

  @Override
  public final void setDRLazyNElem(int lazyNElem) {
    this.lazyNElem = lazyNElem;
  }
}
