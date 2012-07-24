package org.schwa.dr.schemas;

import java.util.ArrayList;
import java.util.List;

import org.schwa.dr.Ann;


public class AnnSchema {
  protected final Class<? extends Ann> klass;
  protected final String name;
  protected String serial;
  protected List<FieldSchema> fieldSchemas;

  public AnnSchema(Class<? extends Ann> klass, String name) {
    this(klass, name, name);
  }

  public AnnSchema(Class<? extends Ann> klass, String name, String serial) {
    this.klass = klass;
    this.name = name;
    this.serial = serial;
    this.fieldSchemas = new ArrayList<FieldSchema>();
  }

  public void addField(final FieldSchema fieldSchema) {
    fieldSchemas.add(fieldSchema);
  }

  public Class<? extends Ann> getKlass() {
    return klass;
  }

  public FieldSchema getField(int index) {
    return fieldSchemas.get(index);
  }

  public FieldSchema getField(String name) {
    for (FieldSchema field : fieldSchemas)
      if (field.getName().equals(name))
        return field;
    return null;
  }

  public List<FieldSchema> getFields() {
    return fieldSchemas;
  }

  public String getName() {
    return name;
  }

  public String getSerial() {
    return serial;
  }

  public void setSerial(String serial) {
    this.serial = serial;
  }
}
