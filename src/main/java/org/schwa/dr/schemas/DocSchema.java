package org.schwa.dr.schemas;

import java.util.HashSet;
import java.util.Set;

import org.schwa.dr.BaseDoc;
import org.schwa.dr.annotations.DRDoc;


public class DocSchema extends BaseAnnSchema {
  protected Set<BaseAnnSchema> anns;
  protected Set<BaseStoreSchema> stores;

  protected DocSchema(Class<?> klass, String name) {
    super(klass, name, "__meta__");
    anns = new HashSet();
    stores = new HashSet();
  }


  public static <T extends BaseDoc> DocSchema create(final Class<T> klass) {
    final DRDoc ann = klass.getAnnotation(DRDoc.class);
    if (ann == null)
      throw new IllegalArgumentException("The provided class is not annotated with DRDoc");
    return new DocSchema(klass, klass.getName());
  }
}
