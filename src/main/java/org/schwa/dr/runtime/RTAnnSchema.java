package org.schwa.dr.runtime;

import java.util.ArrayList;
import java.util.List;

import org.schwa.dr.AnnSchema;


public final class RTAnnSchema {
  private int klassId;
  private String serial;
  private AnnSchema def;
  private List<RTFieldSchema> fields;
  private List<RTStoreSchema> stores;

  public RTAnnSchema(int klassId, String serial) {
    this(klassId, serial, null);
  }

  public RTAnnSchema(int klassId, String serial, AnnSchema def) {
    this.klassId = klassId;
    this.serial = serial;
    this.def = def;
    this.fields = new ArrayList<RTFieldSchema>();
    this.stores = new ArrayList<RTStoreSchema>();
  }

  public void addField(RTFieldSchema field) {
    fields.add(field);
  }

  public void addStore(RTStoreSchema store) {
    stores.add(store);
  }

  public AnnSchema getDef() {
    return def;
  }

  public RTFieldSchema getField(int index) {
    return fields.get(index);
  }

  public List<RTFieldSchema> getFields() {
    return fields;
  }

  public int getKlassId() {
    return klassId;
  }

  public String getSerial() {
    return serial;
  }

  public RTStoreSchema getStore(int index) {
    return stores.get(index);
  }

  public List<RTStoreSchema> getStores() {
    return stores;
  }

  public boolean isLazy() {
    return def == null;
  }

  public void setDef(AnnSchema def) {
    this.def = def;
  }
}
