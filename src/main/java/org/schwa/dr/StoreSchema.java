package org.schwa.dr;

import java.lang.reflect.Field;


public final class StoreSchema {
  private final Field field;
  private final String name;
  private final Class<? extends Ann> storedKlass;
  private String serial;

  private StoreSchema(Field field, Class<? extends Ann> storedKlass, String name, String serial) {
    this.field = field;
    this.name = name;
    this.storedKlass = storedKlass;
    serial = serial.trim();
    this.serial = serial.isEmpty() ? name : serial;
  }

  public Field getField() {
    return field;
  }

  public String getName() {
    return name;
  }

  public String getSerial() {
    return serial;
  }

  public Store<? extends Ann> getStore(final Doc doc) {
    try {
      return (Store<? extends Ann>) field.get(doc);
    }
    catch (IllegalAccessException e) {
      throw new RuntimeException(e);
    }
  }

  public Class<? extends Ann> getStoredKlass() {
    return storedKlass;
  }

  public void resize(final int size, final Doc doc) {
    try {
      Store<Ann> store = (Store<Ann>) field.get(doc);
      for (int i = 0; i != size; i++) {
        Ann ann = (Ann) storedKlass.newInstance();
        store.add(ann);
      }
    }
    catch (IllegalAccessException e) {
      throw new RuntimeException(e);
    }
    catch (InstantiationException e) {
      throw new RuntimeException(e);
    }
  }

  public int size(final Doc doc) {
    try {
      return ((Store<?>) field.get(doc)).size();
    }
    catch (IllegalAccessException e) {
      throw new RuntimeException(e);
    }
  }

  public static StoreSchema create(Field field, Class<? extends Ann> storedKlass, dr.Store drStore) {
    return new StoreSchema(field, storedKlass, field.getName(), drStore.serial());
  }
}
