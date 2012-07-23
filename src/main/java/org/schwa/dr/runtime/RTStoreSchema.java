package org.schwa.dr.runtime;

import org.schwa.dr.schemas.StoreSchema;


public final class RTStoreSchema {
  int storeId;
  String serial;
  RTAnnSchema storedKlass;
  StoreSchema def;
  byte[] lazyData;
  int nLazyElements;

  RTStoreSchema(int storeId, String serial, RTAnnSchema storedKlass, StoreSchema def) {
    this.storeId = storeId;
    this.serial = serial;
    this.storedKlass = storedKlass;
    this.def = def;
  }

  RTStoreSchema(int storeId, String serial, RTAnnSchema storedKlass, byte[] lazyData, int nLazyElements) {
    this.storeId = storeId;
    this.serial = serial;
    this.storedKlass = storedKlass;
    this.lazyData = lazyData;
    this.nLazyElements = nLazyElements;
  }

  public StoreSchema getDef() {
    return def;
  }

  public byte[] getLazyData() {
    return lazyData;
  }

  public int getLazyNElem() {
    return nLazyElements;
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
}
