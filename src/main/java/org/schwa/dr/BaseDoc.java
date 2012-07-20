package org.schwa.dr;


public class BaseDoc {
  byte[] lazy;
  Integer drIndex;

  protected BaseDoc() {
    lazy = null;
    drIndex= null;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o)
      return true;
    else if (o == null)
      return false;
    else if (!(o instanceof Ann))
      return false;
    final Ann a = (Ann)o;
    if (drIndex == null || a.drIndex == null)
      return false;
    return drIndex == a.drIndex;
  }

  public Integer getDrIndex() {
    return drIndex;
  }
}
