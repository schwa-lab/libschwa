package org.schwa.dr.schemas;

import java.lang.reflect.Field;

import org.schwa.dr.BaseAnn;
import org.schwa.dr.annotations.DRPointer;


public class PointerSchema extends FieldSchema {
  protected final Class<? extends BaseAnn> pointedToKlass;
  protected final String storeName;

  protected PointerSchema(FieldType fieldType, Field field, String name, String serial, String storeName, Class<? extends BaseAnn> pointedToKlass) {
    super(fieldType, field, name, serial);
    this.pointedToKlass = pointedToKlass;
    this.storeName = storeName;
  }

  public Class<? extends BaseAnn> getPointedToKlass() {
    return pointedToKlass;
  }

  public String getStoreName() {
    return storeName;
  }

  public static PointerSchema createSlice(final Field field, final DRPointer drPointer, Class<? extends BaseAnn> pointedToKlass) {
    return new PointerSchema(FieldType.ANN_SLICE, field, field.getName(), drPointer.serial(), drPointer.store(), pointedToKlass);
  }

  public static PointerSchema createPointer(final Field field, final DRPointer drPointer, Class<? extends BaseAnn> pointedToKlass) {
    return new PointerSchema(FieldType.POINTER, field, field.getName(), drPointer.serial(), drPointer.store(), pointedToKlass);
  }

  public static PointerSchema createPointers(final Field field, final DRPointer drPointer, Class<? extends BaseAnn> pointedToKlass) {
    return new PointerSchema(FieldType.POINTERS, field, field.getName(), drPointer.serial(), drPointer.store(), pointedToKlass);
  }
}
