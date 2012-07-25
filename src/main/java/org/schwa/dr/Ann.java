package org.schwa.dr;


public interface Ann {
  public Integer getDRIndex();
  public byte[] getDRLazy();
  public int getDRLazyNElem();

  public void setDRIndex(Integer index);
  public void setDRLazy(byte[] lazy);
  public void setDRLazyNElem(int lazyNElem);
}
