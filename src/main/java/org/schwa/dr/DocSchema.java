package org.schwa.dr;

import java.util.List;

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

  /**
   * Constructs a Document Schema, given the Class object of the Document
   * you wish to use as the document on the stream.
   *
   * @param klass The Class of the class to be used as the document on the stream
   * @throws IllegalArgumentException if the provided class is not annotated with DRDoc
   */
  public <T extends BaseDoc> DocSchema(final Class<T> klass) {
    if (!isKlassADoc(klass))
      throw new IllegalArgumentException("The provided class is not annotated with DRDoc");
    documentKlass = klass;
    traverseDocKlass(klass);
  }


  private static boolean isKlassADoc(final Class klass) {
    return klass.getAnnotation(DRDoc.class) != null;
  }


  private static void traverseDocKlass(final Class docKlass) {
    for (Field field : docKlass.getFields()) {
      // identify the Store instances annotated with DRStore
      final Class<?> fieldKlass = field.getType();
      final DRField drField = field.getAnnotation(DRField.class);
      final DRPointer drPointer = field.getAnnotation(DRPointer.class);
      final DRStore drStore = field.getAnnotation(DRStore.class);
      if (fieldKlass.equals(Store.class) && drStore != null) {
        if (drPointer != null || drField != null)
          throw new IllegalAnnotationExecption("Field " + field + " cannot be annotated with DRStore as well as DRField or DRPointer");

        // find the generic argument to the store
        final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
        assert types.length == 1;
        final Class<?> storeKlass = (Class<?>) types[0];
        System.out.println("found store: " + fieldKlass + "<" + storeKlass + "> ");
        traverseAnnKlass(storeKlass);
      }
    }
  }

  private static void traverseAnnKlass(final Class annKlass) {
    for (Field field : annKlass.getFields()) {
      final Class<?> fieldKlass = field.getType();
      final DRField drField = field.getAnnotation(DRField.class);
      final DRPointer drPointer = field.getAnnotation(DRPointer.class);
      final DRStore drStore = field.getAnnotation(DRStore.class);

      if (drField != null) {
        if (drPointer != null || drStore != null)
          throw new IllegalAnnotationExecption("Field " + field + " cannot be annotated with DRField as well as DRStore or DRPointer");
        System.out.println("found field: " + field);
        checkDRFieldField(field);
      }
      else if (drPointer != null) {
        if (drStore != null)
          throw new IllegalAnnotationExecption("Field " + field + " cannot be annotated with DRPointer as well as DRStore");
        System.out.println("found pointer: " + field);
        checkDRPointerField(field);
      }
    }
  }


  private static void checkDRFieldField(final Field field) {
    // DRField can annotate:
    // * {char,short,int,long,float,double,boolean,String}
    // * org.schwa.dr.Slice
    final Class<?>[] allowedKlasses = {char.class, short.class, int.class, long.class, float.class, double.class, boolean.class, String.class, Slice.class};
    final Class<?> fieldKlass = field.getType();
    Class<?> klass = null;
    for (Class<?> k : allowedKlasses) {
      if (fieldKlass.equals(k)) {
        klass = k;
        break;
      }
    }
    if (klass == null)
      throw new IllegalAnnotationExecption("Field " + field + " which is annotated with DRField is of an invalid type");
  }


  private static void checkDRPointerField(final Field field) {
    // DRPointer can annotate:
    // * org.schwa.dr.AnnSlice
    // * T, for T extends org.schwa.dr.BaseAnn
    // * java.util.List<T>, for T extends org.schwa.dr.BaseAnn
    final Class<?> fieldKlass = field.getType();
    if (fieldKlass.equals(AnnSlice.class)) {
      System.out.println("is AnnSlice");
    }
    else if (BaseAnn.class.isAssignableFrom(fieldKlass)) {
      System.out.println("is a BaseAnn subclass");
    }
    else if (List.class.isAssignableFrom(fieldKlass)) {
      // ensure the generic of the List is a BaseAnn subclass
      final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
      assert types.length == 1;
      final Class<?> listKlass = (Class<?>) types[0];
      if (!BaseAnn.class.isAssignableFrom(listKlass))
        throw new IllegalAnnotationExecption("Field " + field + " cannot be annotated with DRPointer when the generic type T of List<T> is not a org.schwa.dr.BaseAnn subclass");
      System.out.println("is List<? extends BaseAnn>");
    }
    else
      throw new IllegalAnnotationExecption("Field " + field + " which is annotated with DRPointer is of an invalid type");
  }
}
