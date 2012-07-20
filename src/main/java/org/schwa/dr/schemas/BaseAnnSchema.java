package org.schwa.dr.schemas;

import java.util.HashSet;
import java.util.Set;


public class BaseAnnSchema {
  protected final Class<?> klass;
  protected final String name;
  protected String serial;
  protected Set<AbstractFieldSchema> fields;

  protected BaseAnnSchema(Class<?> klass, String name) {
    BaseSchema(klass, name, name);
  }

  protected BaseAnnSchema(Class<?> klass, String name, String serial) {
    this.klass = klass;
    this.name = name;
    this.serial = serial;
    this.fields = new HashSet();
  }

  public Class<?> getKlass() {
    return klass;
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
