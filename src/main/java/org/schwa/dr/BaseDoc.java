package org.schwa.dr;


public abstract class BaseDoc {
  byte[] lazy;
  Integer drIndex;

  public BaseDoc() {
    lazy = null;
    drIndex= null;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o)
      return true;
    else if (o == null)
      return false;
    else if (!(o instanceof BaseDoc))
      return false;
    final BaseDoc a = (BaseDoc) o;
    if (drIndex == null || a.drIndex == null)
      return false;
    return drIndex == a.drIndex;
  }

  public Integer getDrIndex() {
    return drIndex;
  }
}
