package org.schwa.dr.runtime;

import org.schwa.dr.schemas.FieldSchema;


public final class RTFieldSchema {
  int fieldId;
  String serial;
  RTStoreSchema containingStore;
  boolean isSlice;
  FieldSchema def;

  RTFieldSchema(int fieldId, String serial, RTStoreSchema containingStore, boolean isSlice) {
    this(fieldId, serial, containingStore, isSlice, null);
  }

  RTFieldSchema(int fieldId, String serial, RTStoreSchema containingStore, boolean isSlice, FieldSchema def) {
    this.fieldId = fieldId;
    this.serial = serial;
    this.containingStore = containingStore;
    this.isSlice = isSlice;
    this.def = def;
  }

  public RTStoreSchema getContainingStore() {
    return containingStore;
  }

  public FieldSchema getDef() {
    return def;
  }

  public int getFieldId() {
    return fieldId;
  }

  public String getSerial() {
    return serial;
  }

  public boolean isLazy() {
    return def == null;
  }

  public boolean isPointer() {
    return containingStore != null;
  }

  public boolean isSlice() {
    return isSlice;
  }
}
