package org.schwa.dr;


public abstract class BaseAnn {
  byte[] lazy;
  Integer drIndex;

  public BaseAnn() {
    lazy = null;
    drIndex= null;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o)
      return true;
    else if (o == null)
      return false;
    else if (!(o instanceof BaseAnn))
      return false;
    final BaseAnn a = (BaseAnn) o;
    if (drIndex == null || a.drIndex == null)
      return false;
    return drIndex == a.drIndex;
  }

  public Integer getDrIndex() {
    return drIndex;
  }
}
