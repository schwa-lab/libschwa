package org.schwa.dr.schemas;

import java.util.HashSet;
import java.util.Set;

import org.schwa.dr.Ann;


public class AnnSchema {
  protected final Class<? extends Ann> klass;
  protected final String name;
  protected String serial;
  protected Set<FieldSchema> fieldSchemas;

  public AnnSchema(Class<? extends Ann> klass, String name) {
    this(klass, name, name);
  }

  public AnnSchema(Class<? extends Ann> klass, String name, String serial) {
    this.klass = klass;
    this.name = name;
    this.serial = serial;
    this.fieldSchemas = new HashSet<FieldSchema>();
  }

  public void addField(final FieldSchema fieldSchema) {
    fieldSchemas.add(fieldSchema);
  }

  public Class<? extends Ann> getKlass() {
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
