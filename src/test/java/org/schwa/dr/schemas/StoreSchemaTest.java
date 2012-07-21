package org.schwa.dr.schemas;

import java.util.*;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

import org.schwa.dr.Ann;
import org.schwa.dr.Doc;
import org.schwa.dr.Store;
import org.schwa.dr.annotations.DRAnn;
import org.schwa.dr.annotations.DRDoc;
import org.schwa.dr.annotations.DRStore;


public class StoreSchemaTest extends TestCase {
  @DRAnn
  static class A extends Ann {
  }

  public void testStores() {
    @DRDoc
    class TD extends Doc {
      @DRStore
      public Store<A> storeOfAs;
      @DRStore(serial="foo")
      public Store<A> anotherStoreOfAs;
    }

    final DocSchema d = DocSchema.create(TD.class);
    assertEquals(0, d.getFields().size());
    assertEquals(2, d.getAnns().size());
    assertEquals(2, d.getStores().size());

    final List<StoreSchema> schemas = new ArrayList(d.getStores());
    Collections.sort(schemas, new Comparator<StoreSchema>() {
      public int compare(StoreSchema a, StoreSchema b) {
        return a.getName().compareTo(b.getName());
      }
    });

    StoreSchema s = schemas.get(0);
    assertEquals(FieldType.STORE, s.getFieldType());
    assertEquals("anotherStoreOfAs", s.getName());
    assertEquals("foo", s.getSerial());
    assertEquals(A.class, s.getStoredKlass());

    s = schemas.get(1);
    assertEquals(FieldType.STORE, s.getFieldType());
    assertEquals("storeOfAs", s.getName());
    assertEquals("storeOfAs", s.getSerial());
    assertEquals(A.class, s.getStoredKlass());
  }

}
