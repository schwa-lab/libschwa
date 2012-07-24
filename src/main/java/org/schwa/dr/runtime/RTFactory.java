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
import org.schwa.dr.schemas.FieldType;
import org.schwa.dr.schemas.StoreSchema;


public final class RTFactory {
  public static RTManager create() {
    return new RTManager(null);
  }

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

    // construct the documents RTFieldSchema's
    mergeFields(rtDocSchema, docSchema, knownStores);

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
      mergeFields(rtAnn, ann, knownStores);
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

  private static void mergeFields(final RTAnnSchema rtSchema, final AnnSchema schema, final Map<String, RTStoreSchema> knownStores) {
    // discover existing fields
    int fieldId = 0;
    Map<String, RTFieldSchema> knownFields = new HashMap<String, RTFieldSchema>();
    if (!rtSchema.fields.isEmpty()) {
      for (RTFieldSchema f : rtSchema.fields) {
        if (!f.isLazy())
          knownFields.put(f.def.getName(), f);
        if (f.fieldId > fieldId)
          fieldId = f.fieldId;
      }
      fieldId++;
    }

    // construct the new RTFieldSchemas
    for (FieldSchema field : schema.getFields()) {
      RTFieldSchema rtField = knownFields.get(field.getName());
      if (rtField == null) {
        final FieldType type = field.getFieldType();
        final boolean isSlice = type == FieldType.SLICE || type == FieldType.ANN_SLICE;
        RTStoreSchema rtStore = null;
        if (type == FieldType.ANN_SLICE || type == FieldType.POINTER || type == FieldType.POINTERS)
          rtStore = knownStores.get(field.getStoreName());
        rtField = new RTFieldSchema(fieldId, field.getSerial(), rtStore, isSlice, field);
        rtSchema.addField(rtField);
        fieldId++;
      }
      else
        rtField.def = field;
    }
    if (!rtSchema.fields.isEmpty()) {
      Collections.sort(rtSchema.fields, new Comparator<RTFieldSchema>() {
        public int compare(final RTFieldSchema a, final RTFieldSchema b) {
          return a.fieldId == b.fieldId ? 0 : (a.fieldId < b.fieldId ? -1 : 1);
        }
      });
      if (rtSchema.fields.get(rtSchema.fields.size() - 1).fieldId + 1 != rtSchema.fields.size())
        throw new AssertionError();
    }
  }
}
