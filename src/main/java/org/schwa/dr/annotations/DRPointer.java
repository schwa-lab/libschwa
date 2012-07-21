package org.schwa.dr.annotations;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;


/**
 * DRPointer can annotate:
 * * org.schwa.dr.AnnSlice
 * * T, for T extends org.schwa.dr.Ann
 * * java.util.List&lt;T&gt;, for T extends org.schwa.dr.Ann
 *
 * @author Tim Dawborn
 */
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.FIELD})
public @interface DRPointer {
  String store();
  String serial() default "";
}
