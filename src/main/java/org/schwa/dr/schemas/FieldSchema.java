package org.schwa.dr.schemas;

import java.lang.reflect.Field;

import org.schwa.dr.Ann;
import org.schwa.dr.dr;


public class FieldSchema {
  protected final FieldType fieldType;
  protected final Field field;
  protected final String name;
  protected String serial;

  protected FieldSchema(FieldType fieldType, Field field, String name, String serial) {
    this.fieldType = fieldType;
    this.field = field;
    this.name = name;
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

  public FieldType getFieldType() {
    return fieldType;
  }

  public boolean isPointer() {
    return fieldType == FieldType.ANN_SLICE || fieldType == FieldType.POINTER || fieldType == FieldType.POINTERS;
  }

  public boolean isSlice() {
    return fieldType == FieldType.ANN_SLICE || fieldType == FieldType.SLICE;
  }

  public void setSerial(String serial) {
    this.serial = serial;
  }

  public Object getFieldValue(Ann ann) {
    try {
      return field.get(ann);
    }
    catch (IllegalAccessException e) {
      throw new RuntimeException(e);
    }
  }

  public static FieldSchema createPrimitiveField(final Field field, final dr.Field drField) {
    return new FieldSchema(FieldType.PRIMITIVE, field, field.getName(), drField.serial());
  }

  public static FieldSchema createSliceField(final Field field, final dr.Field drField) {
    return new FieldSchema(FieldType.SLICE, field, field.getName(), drField.serial());
  }
}
