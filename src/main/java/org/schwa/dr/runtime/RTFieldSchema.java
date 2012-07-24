package org.schwa.dr.runtime;

import org.schwa.dr.FieldSchema;


public final class RTFieldSchema {
  private int fieldId;
  private String serial;
  private RTStoreSchema containingStore;
  private boolean isSlice;
  private FieldSchema def;

  public RTFieldSchema(int fieldId, String serial, RTStoreSchema containingStore, boolean isSlice) {
    this(fieldId, serial, containingStore, isSlice, null);
  }

  public RTFieldSchema(int fieldId, String serial, RTStoreSchema containingStore, boolean isSlice, FieldSchema def) {
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

  public void setContainingStore(RTStoreSchema containingStore) {
    this.containingStore = containingStore;
  }

  public void setDef(FieldSchema def) {
    this.def = def;
  }
}
