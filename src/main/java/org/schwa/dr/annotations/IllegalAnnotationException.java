package org.schwa.dr.annotations;


public class IllegalAnnotationException extends RuntimeException {
  public IllegalAnnotationException(String msg) {
    super(msg);
  }
}
