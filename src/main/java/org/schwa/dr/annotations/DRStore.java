package org.schwa.dr.annotations;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;


/**
 * DRStore can annotate:
 * * org.schwa.dr.Store
 *
 * @author Tim Dawborn
 */
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.FIELD})
public @interface DRStore {
  String serial() default "";
}
