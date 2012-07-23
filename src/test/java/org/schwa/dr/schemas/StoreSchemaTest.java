package org.schwa.dr.schemas;

import java.util.*;

import org.junit.Assert;
import org.junit.Test;

import org.schwa.dr.Ann;
import org.schwa.dr.Doc;
import org.schwa.dr.Store;
import org.schwa.dr.annotations.DRAnn;
import org.schwa.dr.annotations.DRDoc;
import org.schwa.dr.annotations.DRStore;


public class StoreSchemaTest {
  @DRAnn
  static class A extends Ann {
  }

  @Test
  public void testStores() {
    @DRDoc
    class TD extends Doc {
      @DRStore
      public Store<A> storeOfAs;
      @DRStore(serial="foo")
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
    Assert.assertEquals(FieldType.STORE, s.getFieldType());
    Assert.assertEquals("anotherStoreOfAs", s.getName());
    Assert.assertEquals("foo", s.getSerial());
    Assert.assertEquals(A.class, s.getStoredKlass());

    s = schemas.get(1);
    Assert.assertEquals(FieldType.STORE, s.getFieldType());
    Assert.assertEquals("storeOfAs", s.getName());
    Assert.assertEquals("storeOfAs", s.getSerial());
    Assert.assertEquals(A.class, s.getStoredKlass());
  }
}
