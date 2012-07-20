package org.schwa.dr.schemas;

import java.lang.reflect.Field;


public class AbstractFieldSchema {
  protected final FieldType fieldType;
  protected final Field field;
  protected final String name;
  protected String serial;

  public AbstractFieldSchema(FieldType fieldType, Field field, String name) {
    this(fieldType, field, name, name);
  }

  public AbstractFieldSchema(FieldType fieldType, Field field, String name, String serial) {
    this.fieldType = fieldType;
    this.field = field;
    this.name = name;
    this.serial = serial;
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

  public void setSerial(String serial) {
    this.serial = serial;
  }

}
