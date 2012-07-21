package org.schwa.dr.schemas;

import java.util.HashSet;
import java.util.Set;


public class AnnSchema {
  protected final Class<?> klass;
  protected final String name;
  protected String serial;
  protected Set<FieldSchema> fieldSchemas;

  public AnnSchema(Class<?> klass, String name) {
    this(klass, name, name);
  }

  public AnnSchema(Class<?> klass, String name, String serial) {
    this.klass = klass;
    this.name = name;
    this.serial = serial;
    this.fieldSchemas = new HashSet();
  }

  public void addField(final FieldSchema fieldSchema) {
    fieldSchemas.add(fieldSchema);
  }

  public Class<?> getKlass() {
    return klass;
  }

  public Set<FieldSchema> getFields() {
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
