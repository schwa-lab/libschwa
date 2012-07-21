package org.schwa.dr.runtime;

import java.util.ArrayList;
import java.util.List;

import org.schwa.dr.schemas.AnnSchema;


public final class RTAnnSchema {
  int klassId;
  String serial;
  AnnSchema def;
  List<RTFieldSchema> fields;
  List<RTStoreSchema> stores;

  RTAnnSchema(int klassId, String serial) {
    this(klassId, serial, null);
  }

  RTAnnSchema(int klassId, String serial, AnnSchema def) {
    this.klassId = klassId;
    this.serial = serial;
    this.def = def;
    this.fields = new ArrayList<RTFieldSchema>();
    this.stores = new ArrayList<RTStoreSchema>();
  }

  void addField(RTFieldSchema field) {
    fields.add(field);
  }

  void addStore(RTStoreSchema store) {
    stores.add(store);
  }

  boolean isLazy() {
    return def == null;
  }

}
