package org.schwa.dr.schemas;

import java.lang.reflect.Field;

import org.schwa.dr.BaseAnn;


public class StoreSchema extends AbstractFieldSchema {
  protected final Class<? extends BaseAnn> storedKlass;

  StoreSchema(Class<? extends BaseAnn> storedKlass, Field field, String name) {
    super(FieldType.STORE, field, name);
    this.storedKlass = storedKlass;
  }

  StoreSchema(Class<? extends BaseAnn> storedKlass, Field field, String name, String serial) {
    super(FieldType.STORE, field, name, serial);
    this.storedKlass = storedKlass;
  }

  public Class<? extends BaseAnn> getStoredKlass() {
    return storedKlass;
  }

}
