package org.schwa.dr.schemas;

import java.lang.IllegalArgumentException;
import java.lang.RuntimeException;
import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.schwa.dr.AnnSlice;
import org.schwa.dr.Ann;
import org.schwa.dr.Doc;
import org.schwa.dr.Slice;
import org.schwa.dr.Store;
import org.schwa.dr.annotations.DRAnn;
import org.schwa.dr.annotations.DRDoc;
import org.schwa.dr.annotations.DRField;
import org.schwa.dr.annotations.DRPointer;
import org.schwa.dr.annotations.DRStore;
import org.schwa.dr.annotations.IllegalAnnotationException;


public class DocSchema extends AnnSchema {
  protected Set<AnnSchema> annSchemas;
  protected Set<StoreSchema> storeSchemas;

  private DocSchema(Class<?> klass, String name) {
    super(klass, name, "__meta__");
    annSchemas = new HashSet<AnnSchema>();
    storeSchemas = new HashSet<StoreSchema>();
    traverseDocKlass();
  }

  public void addAnn(final AnnSchema annSchema) {
    annSchemas.add(annSchema);
  }

  public void addStore(final StoreSchema storeSchema) {
    storeSchemas.add(storeSchema);
  }

  public Set<AnnSchema> getAnns() {
    return annSchemas;
  }

  public Set<StoreSchema> getStores() {
    return storeSchemas;
  }

  private boolean hasStore(final String name) {
    for (StoreSchema s : storeSchemas)
      if (s.getName().equals(name))
        return true;
    return false;
  }

  private void traverseDocKlass() {
    for (Field field : klass.getFields()) {
      // identify the Store instances annotated with DRStore
      final Class<?> fieldKlass = field.getType();
      final DRField drField = field.getAnnotation(DRField.class);
      final DRPointer drPointer = field.getAnnotation(DRPointer.class);
      final DRStore drStore = field.getAnnotation(DRStore.class);
      if (fieldKlass.equals(Store.class) && drStore != null) {
        if (drPointer != null || drField != null)
          throw new IllegalAnnotationException("Field '" + field + "' cannot be annotated with DRStore as well as DRField or DRPointer");

        // find the generic argument to the store
        final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
        assert types.length == 1;
        final Class<? extends Ann> storedKlass = (Class<? extends Ann>) types[0];

        // ensure the generic argument is annotated with DRAnn
        final DRAnn drAnn = storedKlass.getAnnotation(DRAnn.class);
        if (drAnn == null)
          throw new IllegalAnnotationException("The stored class '" + storedKlass + "' in field '" + field + "' is not annotated with DRAnn");

        // create the StoreSchema object
        final StoreSchema storeSchema = StoreSchema.create(storedKlass, field, drStore);
        addStore(storeSchema);

        // create the AnnSchema object
        AnnSchema annSchema;
        if (drAnn.serial().isEmpty())
          annSchema = new AnnSchema(storedKlass, storedKlass.getName());
        else
          annSchema = new AnnSchema(storedKlass, storedKlass.getName(), drAnn.serial());
        addAnn(annSchema);
      }
    }

    // discover all the DRField's and DRPointer's for all of the classes
    traverseAnnSchema(this);
    for (AnnSchema s : annSchemas)
      traverseAnnSchema(s);
  }

  private void traverseAnnSchema(final AnnSchema annSchema) {
    for (Field field : annSchema.getKlass().getFields()) {
      final Class<?> fieldKlass = field.getType();
      final DRField drField = field.getAnnotation(DRField.class);
      final DRPointer drPointer = field.getAnnotation(DRPointer.class);
      final DRStore drStore = field.getAnnotation(DRStore.class);

      if (drField != null) {
        if (drPointer != null || drStore != null)
          throw new IllegalAnnotationException("Field '" + field + "' cannot be annotated with DRField as well as DRStore or DRPointer");
        checkDRFieldField(field, drField, annSchema);
      }
      else if (drPointer != null) {
        if (drStore != null)
          throw new IllegalAnnotationException("Field '" + field + "' cannot be annotated with DRPointer as well as DRStore");
        if (!hasStore(drPointer.store()))
          throw new IllegalAnnotationException("Store name '" + drPointer.store() + "' on field '" + field + "' is unknown");
        checkDRPointerField(field, drPointer, annSchema);
      }
    }
  }

  private void checkDRFieldField(final Field field, final DRField drField, final AnnSchema annSchema) {
    // DRField can annotate:
    // * {char,short,int,long,float,double,boolean,String}
    // * org.schwa.dr.Slice
    final Class<?>[] ALLOWED_KLASSES = {char.class, short.class, int.class, long.class, float.class, double.class, boolean.class, String.class, Slice.class};
    final Class<?> fieldKlass = field.getType();
    FieldSchema fieldSchema = null;
    for (Class<?> k : ALLOWED_KLASSES) {
      if (fieldKlass.equals(k)) {
        if (k == Slice.class)
          fieldSchema = FieldSchema.createSliceField(field, drField);
        else
          fieldSchema = FieldSchema.createPrimitiveField(field, drField);
        break;
      }
    }
    if (fieldSchema == null)
      throw new IllegalAnnotationException("Field '" + field + "' which is annotated with DRField is of an invalid type");
    annSchema.addField(fieldSchema);
  }

  private void checkDRPointerField(final Field field, final DRPointer drPointer, final AnnSchema annSchema) {
    // DRPointer can annotate:
    // * org.schwa.dr.AnnSlice
    // * T, for T extends org.schwa.dr.Ann
    // * java.util.List<T>, for T extends org.schwa.dr.Ann
    FieldSchema fieldSchema;
    final Class<?> fieldKlass = field.getType();
    if (fieldKlass.equals(AnnSlice.class)) {
      final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
      final Class<? extends Ann> pointedToKlass = (Class<? extends Ann>) types[0];
      fieldSchema = PointerSchema.createSlice(field, drPointer, pointedToKlass);
    }
    else if (Ann.class.isAssignableFrom(fieldKlass)) {
      final Class<? extends Ann> pointedToKlass = (Class<? extends Ann>) fieldKlass;
      fieldSchema = PointerSchema.createPointer(field, drPointer, pointedToKlass);
    }
    else if (List.class.isAssignableFrom(fieldKlass)) {
      // ensure the generic of the List is a Ann subclass
      final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
      final Class<?> listKlass = (Class<?>) types[0];
      if (!Ann.class.isAssignableFrom(listKlass))
        throw new IllegalAnnotationException("Field '" + field + "' cannot be annotated with DRPointer when the generic type T of List<T> is not a org.schwa.dr.Ann subclass");
      final Class<? extends Ann> pointedToKlass = (Class<? extends Ann>) listKlass;
      fieldSchema = PointerSchema.createPointers(field, drPointer, pointedToKlass);
    }
    else
      throw new IllegalAnnotationException("Field '" + field + "' which is annotated with DRPointer is of an invalid type");
    annSchema.addField(fieldSchema);
  }

  public static <T extends Doc> DocSchema create(final Class<T> klass) {
    if (!klass.isAnnotationPresent(DRDoc.class))
      throw new IllegalArgumentException("The provided class is not annotated with DRDoc");
    return new DocSchema(klass, klass.getName());
  }
}
