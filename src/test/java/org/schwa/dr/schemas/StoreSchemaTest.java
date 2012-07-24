package org.schwa.dr.schemas;

import java.util.*;

import org.junit.Assert;
import org.junit.Test;

import org.schwa.dr.Ann;
import org.schwa.dr.Doc;
import org.schwa.dr.dr;
import org.schwa.dr.Store;


public class StoreSchemaTest {
  @dr.Ann
  static class A extends Ann {
  }

  @Test
  public void testStores() {
    @dr.Doc
    class TD extends Doc {
      @dr.Store
      public Store<A> storeOfAs;
      @dr.Store(serial="foo")
      public Store<A> anotherStoreOfAs;
    }

    final DocSchema d = DocSchema.create(TD.class);
    Assert.assertEquals(0, d.getFields().size());
    Assert.assertEquals(2, d.getAnns().size());
    Assert.assertEquals(2, d.getStores().size());

    final List<StoreSchema> schemas = new ArrayList(d.getStores());
    Collections.sort(schemas, new Comparator<StoreSchema>() {
      public int compare(StoreSchema a, StoreSchema b) {
        return a.getName().compareTo(b.getName());
      }
    });

    StoreSchema s = schemas.get(0);
    Assert.assertEquals("anotherStoreOfAs", s.getName());
    Assert.assertEquals("foo", s.getSerial());
    Assert.assertEquals(A.class, s.getStoredKlass());

    s = schemas.get(1);
    Assert.assertEquals("storeOfAs", s.getName());
    Assert.assertEquals("storeOfAs", s.getSerial());
    Assert.assertEquals(A.class, s.getStoredKlass());
  }
}
