package org.schwa.dr;


public abstract class Ann {
  protected byte[] lazy;
  protected Integer drIndex;

  protected Ann() {
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
    final Ann a = (Ann) o;
    if (drIndex == null || a.drIndex == null)
      return false;
    return drIndex == a.drIndex;
  }

  public Integer getDrIndex() {
    return drIndex;
  }

  public byte[] getLazy() {
    return lazy;
  }

  void setDrIndex(int index) {
    drIndex = index;
  }
}
