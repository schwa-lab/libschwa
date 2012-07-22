package org.schwa.dr;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;


public class Store<T extends Ann> implements List<T> {
  private final List<T> items;

  public Store() {
    items = new ArrayList<T>();
  }

  @Override
  public boolean add(T obj) {
    if (obj.getDrIndex() != null)
      throw new IllegalArgumentException("Cannot insert an object into a Store which is already in a store (drIndex=" + obj.getDrIndex() + ")");
    obj.setDrIndex(items.size());
    return items.add(obj);
  }

  @Override
  public void add(int index, T o) {
    throw new UnsupportedOperationException();
  }

  @Override
  public boolean addAll(Collection<? extends T> c) {
    for (T obj : c)
      if (obj.getDrIndex() != null)
        throw new IllegalArgumentException("Cannot insert an object into a Store which is already in a store (drIndex=" + obj.getDrIndex() + ")");
    for (T obj : c)
      obj.setDrIndex(items.size());
    return items.addAll(c);
  }

  @Override
  public boolean addAll(int index, Collection<? extends T> c) {
    throw new UnsupportedOperationException();
  }

  @Override
  public void clear() {
    for (T obj : items)
      obj.setDrIndex(null);
    items.clear();
  }

  @Override
  public boolean contains(Object o) {
    return items.contains(o);
  }

  @Override
  public boolean containsAll(Collection<?> c) {
    return items.containsAll(c);
  }

  @Override
  public boolean equals(Object o) {
    return items.equals(o);
  }

  @Override
  public T get(int index) {
    return items.get(index);
  }

  @Override
  public int hashCode() {
    return items.hashCode();
  }

  @Override
  public int indexOf(Object o) {
    return items.indexOf(o);
  }

  @Override
  public boolean isEmpty() {
    return items.isEmpty();
  }

  @Override
  public Iterator<T> iterator() {
    return items.iterator();
  }

  @Override
  public int lastIndexOf(Object o) {
    return items.lastIndexOf(o);
  }

  @Override
  public ListIterator<T> listIterator() {
    return items.listIterator();
  }

  @Override
  public ListIterator<T> listIterator(int index) {
    return items.listIterator(index);
  }

  @Override
  public T remove(int index) {
    throw new UnsupportedOperationException();
  }

  @Override
  public boolean remove(Object o) {
    throw new UnsupportedOperationException();
  }

  @Override
  public boolean removeAll(Collection<?> c) {
    throw new UnsupportedOperationException();
  }

  @Override
  public boolean retainAll(Collection<?> c) {
    throw new UnsupportedOperationException();
  }

  @Override
  public T set(int index, T o) {
    throw new UnsupportedOperationException();
  }

  @Override
  public int size() {
    return items.size();
  }

  @Override
  public List<T> subList(int fromIndex, int toIndex) {
    throw new UnsupportedOperationException();
  }

  @Override
  public Object[] toArray() {
    return items.toArray();
  }

  @Override
  public <T> T[] toArray(T[] a) {
    return items.toArray(a);
  }
}
