package org.schwa.dr.runtime;

import org.schwa.dr.schemas.StoreSchema;


public final class RTStoreSchema {
  int storeId;
  String serial;
  RTAnnSchema storedKlass;
  StoreSchema def;
  byte[] lazy;
  int lazyNElem;

  public RTStoreSchema(int storeId, String serial, RTAnnSchema storedKlass, StoreSchema def) {
    this.storeId = storeId;
    this.serial = serial;
    this.storedKlass = storedKlass;
    this.def = def;
  }

  public RTStoreSchema(int storeId, String serial, RTAnnSchema storedKlass, byte[] lazy, int lazyNElem) {
    this.storeId = storeId;
    this.serial = serial;
    this.storedKlass = storedKlass;
    this.lazy = lazy;
    this.lazyNElem = lazyNElem;
  }

  public StoreSchema getDef() {
    return def;
  }

  public byte[] getLazyData() {
    return lazy;
  }

  public int getLazyNElem() {
    return lazyNElem;
  }

  public String getSerial() {
    return serial;
  }

  public RTAnnSchema getStoredKlass() {
    return storedKlass;
  }

  public int getStoreId() {
    return storeId;
  }

  public boolean isLazy() {
    return def == null;
  }

  public void setLazy(byte[] lazy) {
    this.lazy = lazy;
  }
}
