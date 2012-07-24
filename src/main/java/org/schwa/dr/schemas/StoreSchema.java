package org.schwa.dr.schemas;

import java.lang.reflect.Field;

import org.schwa.dr.Ann;
import org.schwa.dr.Doc;
import org.schwa.dr.dr;
import org.schwa.dr.Store;


public class StoreSchema extends FieldSchema {
  protected final Class<? extends Ann> storedKlass;

  protected StoreSchema(Class<? extends Ann> storedKlass, Field field, String name, String serial) {
    super(FieldType.STORE, field, name, serial);
    this.storedKlass = storedKlass;
  }

  public static StoreSchema create(Class<? extends Ann> storedKlass, final Field field, final dr.Store drStore) {
    return new StoreSchema(storedKlass, field, field.getName(), drStore.serial());
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
}
