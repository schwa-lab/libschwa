package org.schwa.dr.runtime;

import java.lang.reflect.Field;

import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

import org.schwa.dr.Ann;
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
    final RTAnnSchema rtDocSchema = rt.docSchema;

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
    if (!rtDocSchema.stores.isEmpty()) {
      for (RTStoreSchema s : rtDocSchema.stores) {
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
      RTStoreSchema rtStore = knownStores.get(store.getName());
      if (rtStore == null) {
        rtStore = new RTStoreSchema(storeId, store.getSerial(), null, store);
        rtDocSchema.addStore(rtStore);
        knownStores.put(store.getName(), rtStore);
        storeId++;
      }
      else
        rtStore.def = store;
      storeFields.put(rtStore.def.getField(), rtStore);
    }
    if (!rtDocSchema.stores.isEmpty()) {
      Collections.sort(rtDocSchema.stores, new Comparator<RTStoreSchema>() {
        public int compare(final RTStoreSchema a, final RTStoreSchema b) {
          return a.storeId == b.storeId ? 0 : (a.storeId < b.storeId ? -1 : 1);
        }
      });
      if (rtDocSchema.stores.get(rtDocSchema.stores.size() - 1).storeId + 1 != rtDocSchema.stores.size())
        throw new AssertionError();
    }

    // construct the documents RTFieldDef's
    mergeFields(rtDocSchema, docSchema, storeFields);

    // construct each of the klasses
    Map<Class<? extends Ann>, RTAnnSchema> annKlassToRTAnn = new HashMap<Class<? extends Ann>, RTAnnSchema>();
    for (AnnSchema ann : docSchema.getAnns()) {
      RTAnnSchema rtAnn = knownKlasses.get(ann.getName());
      if (rtAnn == null) {
        rtAnn = new RTAnnSchema(klassId, ann.getSerial(), ann);
        rt.addAnn(rtAnn);
        knownKlasses.put(ann.getName(), rtAnn);
        klassId++;
      }
      else
        rtAnn.def = ann;
      mergeFields(rtAnn, ann, storeFields);
      annKlassToRTAnn.put(ann.getKlass(), rtAnn);
    }
    Collections.sort(rt.annSchemas, new Comparator<RTAnnSchema>() {
      public int compare(final RTAnnSchema a, final RTAnnSchema b) {
        return a.klassId == b.klassId ? 0 : (a.klassId < b.klassId ? -1 : 1);
      }
    });
    if (rt.annSchemas.get(rt.annSchemas.size() - 1).klassId + 1 != rt.annSchemas.size())
      throw new AssertionError();

    // back-fill the RTStoreDef's RTAnnSchema pointers now that they exist
    for (StoreSchema store : docSchema.getStores()) {
      RTStoreSchema rtStore = knownStores.get(store.getName());
      if (rtStore.storedKlass == null)
        rtStore.storedKlass = annKlassToRTAnn.get(store.getStoredKlass());
    }

    return rt;
  }

  private static void mergeFields(final RTAnnSchema rtSchema, final AnnSchema schema, final Map<Field, RTStoreSchema> storeFields) {
    // TODO
  }
}
