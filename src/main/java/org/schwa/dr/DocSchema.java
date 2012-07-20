package org.schwa.dr;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import java.lang.IllegalArgumentException;
import java.lang.RuntimeException;
import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;

import org.schwa.dr.annotations.DRDoc;
import org.schwa.dr.annotations.DRField;
import org.schwa.dr.annotations.DRPointer;
import org.schwa.dr.annotations.DRStore;


public class DocSchema {
  private final Class documentKlass;
  private final Map<String, Class<?>> stores;

  /**
   * Constructs a Document Schema, given the Class object of the Document
   * you wish to use as the document on the stream.
   *
   * @param klass The Class of the class to be used as the document on the stream
   */
  public <T extends BaseDoc> DocSchema(final Class<T> klass) {
    if (klass.getAnnotation(DRDoc.class) == null)
      throw new IllegalArgumentException("The provided class is not annotated with DRDoc");

    documentKlass = klass;
    stores = new HashMap();
    traverseDocKlass();
  }


  private void traverseDocKlass() {
    for (Field field : documentKlass.getFields()) {
      // identify the Store instances annotated with DRStore
      final Class<?> fieldKlass = field.getType();
      final DRField drField = field.getAnnotation(DRField.class);
      final DRPointer drPointer = field.getAnnotation(DRPointer.class);
      final DRStore drStore = field.getAnnotation(DRStore.class);
      if (fieldKlass.equals(Store.class) && drStore != null) {
        if (drPointer != null || drField != null)
          throw new IllegalAnnotationExecption("Field '" + field + "' cannot be annotated with DRStore as well as DRField or DRPointer");

        // find the generic argument to the store
        final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
        assert types.length == 1;
        final Class<?> storeKlass = (Class<?>) types[0];
        System.out.println("found store: " + fieldKlass + "<" + storeKlass + "> ");
        stores.put(field.getName(), storeKlass);
      }
    }

    traverseAnnKlass(documentKlass);
    for (Class<?> klass : stores.values())
      traverseAnnKlass(klass);
  }

  private void traverseAnnKlass(final Class annKlass) {
    for (Field field : annKlass.getFields()) {
      final Class<?> fieldKlass = field.getType();
      final DRField drField = field.getAnnotation(DRField.class);
      final DRPointer drPointer = field.getAnnotation(DRPointer.class);
      final DRStore drStore = field.getAnnotation(DRStore.class);

      if (drField != null) {
        if (drPointer != null || drStore != null)
          throw new IllegalAnnotationExecption("Field '" + field + "' cannot be annotated with DRField as well as DRStore or DRPointer");
        System.out.println("found field: " + field);
        checkDRFieldField(field, drField);
      }
      else if (drPointer != null) {
        if (drStore != null)
          throw new IllegalAnnotationExecption("Field '" + field + "' cannot be annotated with DRPointer as well as DRStore");
        System.out.println("found pointer: " + field);

        if (!stores.containsKey(drPointer.store()))
          throw new IllegalAnnotationExecption("Store name '" + drPointer.store() + "' on field '" + field + "' is unknown");

        checkDRPointerField(field, drPointer);
      }
    }
  }


  private void checkDRFieldField(final Field field, final DRField drField) {
    // DRField can annotate:
    // * {char,short,int,long,float,double,boolean,String}
    // * org.schwa.dr.Slice
    final Class<?>[] allowedKlasses = {char.class, short.class, int.class, long.class, float.class, double.class, boolean.class, String.class, Slice.class};
    final Class<?> fieldKlass = field.getType();
    Class<?> klass = null;
    for (Class<?> k : allowedKlasses) {
      if (fieldKlass.equals(k)) {
        klass = k;
        // TODO
        break;
      }
    }
    if (klass == null)
      throw new IllegalAnnotationExecption("Field '" + field + "' which is annotated with DRField is of an invalid type");
  }


  private void checkDRPointerField(final Field field, final DRPointer drPointer) {
    // DRPointer can annotate:
    // * org.schwa.dr.AnnSlice
    // * T, for T extends org.schwa.dr.BaseAnn
    // * java.util.List<T>, for T extends org.schwa.dr.BaseAnn
    final Class<?> fieldKlass = field.getType();
    if (fieldKlass.equals(AnnSlice.class)) {
      // TODO
    }
    else if (BaseAnn.class.isAssignableFrom(fieldKlass)) {
      // TODO
    }
    else if (List.class.isAssignableFrom(fieldKlass)) {
      // ensure the generic of the List is a BaseAnn subclass
      final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
      assert types.length == 1;
      final Class<?> listKlass = (Class<?>) types[0];
      if (!BaseAnn.class.isAssignableFrom(listKlass))
        throw new IllegalAnnotationExecption("Field '" + field + "' cannot be annotated with DRPointer when the generic type T of List<T> is not a org.schwa.dr.BaseAnn subclass");
      // TODO
    }
    else
      throw new IllegalAnnotationExecption("Field '" + field + "' which is annotated with DRPointer is of an invalid type");
  }
}
