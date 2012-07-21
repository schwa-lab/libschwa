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

  public boolean isLazy() {
    return def == null;
  }
}
