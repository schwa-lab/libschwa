package org.schwa.dr.schemas;

import java.lang.reflect.Field;

import org.schwa.dr.BaseAnn;
import org.schwa.dr.annotations.DRStore;


public class StoreSchema extends FieldSchema {
  protected final Class<? extends BaseAnn> storedKlass;

  protected StoreSchema(Class<? extends BaseAnn> storedKlass, Field field, String name, String serial) {
    super(FieldType.STORE, field, name, serial);
    this.storedKlass = storedKlass;
  }

  public Class<? extends BaseAnn> getStoredKlass() {
    return storedKlass;
  }

  public static StoreSchema create(Class<? extends BaseAnn> storedKlass, final Field field, final DRStore drStore) {
    return new StoreSchema(storedKlass, field, field.getName(), drStore.serial());
  }
}
