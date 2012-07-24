package org.schwa.dr;

import java.lang.IllegalArgumentException;
import java.lang.RuntimeException;
import java.lang.reflect.Field;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;

import java.util.ArrayList;
import java.util.List;


public class DocSchema extends AnnSchema {
  protected List<AnnSchema> annSchemas;
  protected List<StoreSchema> storeSchemas;

  private DocSchema(Class<? extends Ann> klass, String name) {
    super(klass, name, "__meta__");
    annSchemas = new ArrayList<AnnSchema>();
    storeSchemas = new ArrayList<StoreSchema>();
    traverseDocKlass();
  }

  public void addSchema(final AnnSchema annSchema) {
    annSchemas.add(annSchema);
  }

  public void addStore(final StoreSchema storeSchema) {
    storeSchemas.add(storeSchema);
  }

  public AnnSchema getSchema(Class<? extends Ann> klass) {
    for (AnnSchema ann : annSchemas)
      if (ann.klass.equals(klass))
        return ann;
    return null;
  }

  public List<AnnSchema> getSchemas() {
    return annSchemas;
  }

  public StoreSchema getStore(String name) {
    for (StoreSchema store : storeSchemas)
      if (store.getName().equals(name))
        return store;
    return null;
  }

  public List<StoreSchema> getStores() {
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
      // identify the Store instances annotated with dr.Store
      final Class<?> fieldKlass = field.getType();
      final dr.Field drField = field.getAnnotation(dr.Field.class);
      final dr.Pointer drPointer = field.getAnnotation(dr.Pointer.class);
      final dr.Store drStore = field.getAnnotation(dr.Store.class);
      if (fieldKlass.equals(Store.class) && drStore != null) {
        if (drPointer != null || drField != null)
          throw new IllegalAnnotationException("Field '" + field + "' cannot be annotated with dr.Store as well as dr.Field or dr.Pointer");

        // find the generic argument to the store
        final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
        assert types.length == 1;
        final Class<? extends Ann> storedKlass = (Class<? extends Ann>) types[0];

        // ensure the generic argument is annotated with dr.Ann
        final dr.Ann drAnn = storedKlass.getAnnotation(dr.Ann.class);
        if (drAnn == null)
          throw new IllegalAnnotationException("The stored class '" + storedKlass + "' in field '" + field + "' is not annotated with dr.Ann");

        // create the StoreSchema object
        final StoreSchema storeSchema = StoreSchema.create(field, storedKlass, drStore);
        addStore(storeSchema);

        // create the AnnSchema object
        AnnSchema annSchema;
        if (drAnn.serial().isEmpty())
          annSchema = new AnnSchema(storedKlass, storedKlass.getSimpleName());
        else
          annSchema = new AnnSchema(storedKlass, storedKlass.getSimpleName(), drAnn.serial());
        addSchema(annSchema);
      }
    }

    // discover all the dr.Field's and dr.Pointer's for all of the classes
    traverseAnnSchema(this);
    for (AnnSchema s : annSchemas)
      traverseAnnSchema(s);
  }

  private void traverseAnnSchema(final AnnSchema annSchema) {
    for (Field field : annSchema.getKlass().getFields()) {
      final Class<?> fieldKlass = field.getType();
      final dr.Field drField = field.getAnnotation(dr.Field.class);
      final dr.Pointer drPointer = field.getAnnotation(dr.Pointer.class);
      final dr.Store drStore = field.getAnnotation(dr.Store.class);

      if (drField != null) {
        if (drPointer != null || drStore != null)
          throw new IllegalAnnotationException("Field '" + field + "' cannot be annotated with dr.Field as well as dr.Store or dr.Pointer");
        checkDRFieldField(field, drField, annSchema);
      }
      else if (drPointer != null) {
        if (drStore != null)
          throw new IllegalAnnotationException("Field '" + field + "' cannot be annotated with dr.Pointer as well as dr.Store");
        if (!hasStore(drPointer.store()))
          throw new IllegalAnnotationException("Store name '" + drPointer.store() + "' on field '" + field + "' is unknown");
        checkDRPointerField(field, drPointer, annSchema);
      }
    }
  }

  private void checkDRFieldField(final Field field, final dr.Field drField, final AnnSchema annSchema) {
    // dr.Field can annotate:
    // * {byte,char,short,int,long,float,double,boolean,String}
    // * org.schwa.dr.ByteSlice
    final Class<?>[] ALLOWED_KLASSES = {byte.class, char.class, short.class, int.class, long.class, float.class, double.class, boolean.class, String.class, ByteSlice.class};
    final Class<?> fieldKlass = field.getType();
    FieldSchema fieldSchema = null;
    for (Class<?> k : ALLOWED_KLASSES) {
      if (fieldKlass.equals(k)) {
        if (k == ByteSlice.class)
          fieldSchema = FieldSchema.createByteSlice(field, drField);
        else
          fieldSchema = FieldSchema.createPrimitive(field, drField);
        break;
      }
    }
    if (fieldSchema == null)
      throw new IllegalAnnotationException("Field '" + field + "' which is annotated with dr.Field is of an invalid type");
    annSchema.addField(fieldSchema);
  }

  private void checkDRPointerField(final Field field, final dr.Pointer drPointer, final AnnSchema annSchema) {
    // dr.Pointer can annotate:
    // * org.schwa.dr.Slice
    // * T, for T extends org.schwa.dr.Ann
    // * java.util.List<T>, for T extends org.schwa.dr.Ann
    FieldSchema fieldSchema;
    final Class<?> fieldKlass = field.getType();
    if (fieldKlass.equals(Slice.class)) {
      final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
      final Class<? extends Ann> pointedToKlass = (Class<? extends Ann>) types[0];
      fieldSchema = FieldSchema.createSlice(field, drPointer, pointedToKlass);
    }
    else if (Ann.class.isAssignableFrom(fieldKlass)) {
      final Class<? extends Ann> pointedToKlass = (Class<? extends Ann>) fieldKlass;
      fieldSchema = FieldSchema.createPointer(field, drPointer, pointedToKlass);
    }
    else if (List.class.isAssignableFrom(fieldKlass)) {
      // ensure the generic of the List is a Ann subclass
      final Type[] types = ((ParameterizedType) field.getGenericType()).getActualTypeArguments();
      final Class<?> listKlass = (Class<?>) types[0];
      if (!Ann.class.isAssignableFrom(listKlass))
        throw new IllegalAnnotationException("Field '" + field + "' cannot be annotated with dr.Pointer when the generic type T of List<T> is not a org.schwa.dr.Ann subclass");
      final Class<? extends Ann> pointedToKlass = (Class<? extends Ann>) listKlass;
      fieldSchema = FieldSchema.createPointers(field, drPointer, pointedToKlass);
    }
    else
      throw new IllegalAnnotationException("Field '" + field + "' which is annotated with dr.Pointer is of an invalid type");
    annSchema.addField(fieldSchema);
  }

  public static <T extends Doc> DocSchema create(final Class<T> klass) {
    if (!klass.isAnnotationPresent(dr.Doc.class))
      throw new IllegalArgumentException("The provided class is not annotated with dr.Doc");
    return new DocSchema(klass, klass.getName());
  }
}
