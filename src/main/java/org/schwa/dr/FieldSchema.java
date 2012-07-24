package org.schwa.dr;

import java.lang.reflect.Field;


public final class FieldSchema {
  private final FieldType fieldType;
  private final Field field;
  private final String name;
  private final Class<? extends Ann> pointedToKlass;
  private final String storeName;
  private String serial;

  private FieldSchema(FieldType fieldType, Field field, String name, String serial, Class<? extends Ann> pointedToKlass, String storeName) {
    this.fieldType = fieldType;
    this.field = field;
    this.name = name;
    this.pointedToKlass = pointedToKlass;
    this.storeName = storeName;
    serial = serial.trim();
    this.serial = serial.isEmpty() ? name : serial;
  }

  public Field getField() {
    return field;
  }

  public FieldType getFieldType() {
    return fieldType;
  }

  public Object getFieldValue(Ann ann) {
    try {
      return field.get(ann);
    }
    catch (IllegalAccessException e) {
      throw new RuntimeException(e);
    }
  }

  public String getName() {
    return name;
  }

  public Class<? extends Ann> getPointedToKlass() {
    return pointedToKlass;
  }

  public String getSerial() {
    return serial;
  }

  public String getStoreName() {
    return storeName;
  }

  public boolean isPointer() {
    return fieldType == FieldType.POINTER || fieldType == FieldType.POINTERS || fieldType == FieldType.SLICE;
  }

  public boolean isSlice() {
    return fieldType == FieldType.BYTE_SLICE || fieldType == FieldType.SLICE;
  }

  public void setSerial(String serial) {
    this.serial = serial;
  }

  public static FieldSchema createByteSlice(Field field, dr.Field drField) {
    return new FieldSchema(FieldType.BYTE_SLICE, field, field.getName(), drField.serial(), null, null);
  }

  public static FieldSchema createPointer(Field field, dr.Pointer drPointer, Class<? extends Ann> pointedToKlass) {
    return new FieldSchema(FieldType.POINTER, field, field.getName(), drPointer.serial(), pointedToKlass, drPointer.store());
  }

  public static FieldSchema createPointers(Field field, dr.Pointer drPointer, Class<? extends Ann> pointedToKlass) {
    return new FieldSchema(FieldType.POINTERS, field, field.getName(), drPointer.serial(), pointedToKlass, drPointer.store());
  }

  public static FieldSchema createPrimitive(Field field, dr.Field drField) {
    return new FieldSchema(FieldType.PRIMITIVE, field, field.getName(), drField.serial(), null, null);
  }

  public static FieldSchema createSlice(Field field, dr.Pointer drPointer, Class<? extends Ann> pointedToKlass) {
    return new FieldSchema(FieldType.SLICE, field, field.getName(), drPointer.serial(), pointedToKlass, drPointer.store());
  }
}
