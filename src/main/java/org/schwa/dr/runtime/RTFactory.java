package org.schwa.dr.runtime;

import java.lang.reflect.Field;

import java.util.HashMap;
import java.util.Map;

import org.schwa.dr.schemas.AnnSchema;
import org.schwa.dr.schemas.DocSchema;
import org.schwa.dr.schemas.FieldSchema;
import org.schwa.dr.schemas.PointerSchema;
import org.schwa.dr.schemas.StoreSchema;


public final class RTFactory {
  public static RTManager build(final DocSchema docSchema) {
    final RTAnnSchema rtDocSchema = new RTAnnSchema(0, "__meta__", docSchema);
    final RTManager rt = new RTManager(rtDocSchema);
    rt.addAnn(rtDocSchema);
    return merge(rt, docSchema);
  }

  public static RTManager merge(final RTManager rt, final DocSchema docSchema) {
    // discover existing klasses
    int klassId = 0;
    Map<String, RTAnnSchema> knownKlasses = new HashMap<String, RTAnnSchema>();
    if (!rt.annSchemas.isEmpty()) {
      for (RTAnnSchema s : rt.annSchemas) {
        if (!s.isLazy())
          knownKlasses.put(s.def.getName(), s);
        if (s.klassId > klassId)
          klassId = s.klassId;
      }
      klassId++;
    }

    // discover existing stores
    int storeId = 0;
    Map<String, RTStoreSchema> knownStores = new HashMap<String, RTStoreSchema>();
    if (!rt.docSchema.stores.isEmpty()) {
      for (RTStoreSchema s : rt.docSchema.stores) {
        if (!s.isLazy())
          knownStores.put(s.def.getName(), s);
        if (s.storeId > storeId)
          storeId = s.storeId;
      }
      storeId++;
    }

    // construct the new RTStoreSchema's
    Map<Field, RTStoreSchema> storeFields = new HashMap<Field, RTStoreSchema>();
    for (StoreSchema store : docSchema.getStores()) {
      RTStoreSchema rtStore = knownStores.get(store.getSerial());
      if (rtStore == null) {
        rtStore = new RTStoreSchema(storeId, store.getSerial(), null, store);
        rt.docSchema.addStore(rtStore);
        knownStores.put(store.getSerial(), rtStore);
        storeId++;
      }
      else
        rtStore.def = store;
      storeFields.put(rtStore.def.getField(), rtStore);
    }

    // TODO
    return rt;
  }
}
