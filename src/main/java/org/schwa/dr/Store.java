package org.schwa.dr;

import java.util.ArrayList;
import java.util.List;


public class Store<T extends Ann> {
  private final List<T> items;

  public Store() {
    items = new ArrayList<T>();
  }

  public void add(T obj) {
    if (obj.getDrIndex() != null)
      throw new IllegalArgumentException("Cannot insert an object into a Store which is already in a store (drIndex=" + obj.getDrIndex() + ")");
    obj.setDrIndex(items.size());
    items.add(obj);
  }

  public T get(int index) {
    return items.get(index);
  }

  public int size() {
    return items.size();
  }
}
