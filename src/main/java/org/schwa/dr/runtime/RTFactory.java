package org.schwa.dr.runtime;

import java.lang.reflect.Field;

import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.schwa.dr.Ann;
import org.schwa.dr.AnnSchema;
import org.schwa.dr.DocSchema;
import org.schwa.dr.FieldSchema;
import org.schwa.dr.FieldType;
import org.schwa.dr.StoreSchema;


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
    final RTAnnSchema rtDocSchema = rt.getDocSchema();

    // discover existing klasses
    int klassId = 0;
    Map<String, RTAnnSchema> knownKlasses = new HashMap<String, RTAnnSchema>();
    if (!rt.getSchemas().isEmpty()) {
      for (RTAnnSchema s : rt.getSchemas()) {
        if (!s.isLazy())
          knownKlasses.put(s.getDef().getName(), s);
        if (s.getKlassId() > klassId)
          klassId = s.getKlassId();
      }
      klassId++;
    }

    // discover existing stores
    int storeId = 0;
    Map<String, RTStoreSchema> knownStores = new HashMap<String, RTStoreSchema>();
    if (!rtDocSchema.getStores().isEmpty()) {
      for (RTStoreSchema s : rtDocSchema.getStores()) {
        if (!s.isLazy())
          knownStores.put(s.getDef().getName(), s);
        if (s.getStoreId() > storeId)
          storeId = s.getStoreId();
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
        rtStore.setDef(store);
    }
    final List<RTStoreSchema> rtStores = rtDocSchema.getStores();
    if (!rtStores.isEmpty()) {
      Collections.sort(rtStores, new Comparator<RTStoreSchema>() {
        public int compare(final RTStoreSchema a, final RTStoreSchema b) {
          return a.getStoreId() == b.getStoreId() ? 0 : (a.getStoreId() < b.getStoreId() ? -1 : 1);
        }
      });
      final RTStoreSchema last = rtStores.get(rtStores.size() - 1);
      if (last.getStoreId() + 1 != rtStores.size())
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
        rtAnn.setDef(ann);
      mergeFields(rtAnn, ann, knownStores);
      annKlassToRTAnn.put(ann.getKlass(), rtAnn);
    }
    {
      final List<RTAnnSchema> rtAnnSchemas = rt.getSchemas();
      Collections.sort(rtAnnSchemas, new Comparator<RTAnnSchema>() {
        public int compare(final RTAnnSchema a, final RTAnnSchema b) {
          return a.getKlassId() == b.getKlassId() ? 0 : (a.getKlassId() < b.getKlassId() ? -1 : 1);
        }
      });
      final RTAnnSchema last = rtAnnSchemas.get(rtAnnSchemas.size() - 1);
      if (last.getKlassId() + 1 != rtAnnSchemas.size())
        throw new AssertionError();
    }

    // back-fill the RTStoreDef's RTAnnSchema pointers now that they exist
    for (StoreSchema store : docSchema.getStores()) {
      RTStoreSchema rtStore = knownStores.get(store.getName());
      if (rtStore.getStoredKlass() == null)
        rtStore.setStoredKlass(annKlassToRTAnn.get(store.getStoredKlass()));
    }

    return rt;
  }

  private static void mergeFields(final RTAnnSchema rtSchema, final AnnSchema schema, final Map<String, RTStoreSchema> knownStores) {
    // discover existing fields
    int fieldId = 0;
    Map<String, RTFieldSchema> knownFields = new HashMap<String, RTFieldSchema>();
    if (!rtSchema.getFields().isEmpty()) {
      for (RTFieldSchema f : rtSchema.getFields()) {
        if (!f.isLazy())
          knownFields.put(f.getDef().getName(), f);
        if (f.getFieldId() > fieldId)
          fieldId = f.getFieldId();
      }
      fieldId++;
    }

    // construct the new RTFieldSchemas
    for (FieldSchema field : schema.getFields()) {
      RTFieldSchema rtField = knownFields.get(field.getName());
      if (rtField == null) {
        final boolean isSlice = field.isSlice();
        RTStoreSchema rtStore = null;
        if (field.isPointer())
          rtStore = knownStores.get(field.getStoreName());
        rtField = new RTFieldSchema(fieldId, field.getSerial(), rtStore, isSlice, field);
        rtSchema.addField(rtField);
        fieldId++;
      }
      else
        rtField.setDef(field);
    }
    {
      final List<RTFieldSchema> rtFields = rtSchema.getFields();
      if (!rtFields.isEmpty()) {
        Collections.sort(rtFields, new Comparator<RTFieldSchema>() {
          public int compare(final RTFieldSchema a, final RTFieldSchema b) {
            return a.getFieldId() == b.getFieldId() ? 0 : (a.getFieldId() < b.getFieldId() ? -1 : 1);
          }
        });
        final RTFieldSchema last = rtFields.get(rtFields.size() - 1);
        if (last.getFieldId() + 1 != rtFields.size())
          throw new AssertionError();
      }
    }
  }
}
