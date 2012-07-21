package org.schwa.dr.schemas;

import java.lang.reflect.Field;

import org.schwa.dr.Ann;
import org.schwa.dr.annotations.DRStore;


public class StoreSchema extends FieldSchema {
  protected final Class<? extends Ann> storedKlass;

  protected StoreSchema(Class<? extends Ann> storedKlass, Field field, String name, String serial) {
    super(FieldType.STORE, field, name, serial);
    this.storedKlass = storedKlass;
  }

  public Class<? extends Ann> getStoredKlass() {
    return storedKlass;
  }

  public static StoreSchema create(Class<? extends Ann> storedKlass, final Field field, final DRStore drStore) {
    return new StoreSchema(storedKlass, field, field.getName(), drStore.serial());
  }
}
