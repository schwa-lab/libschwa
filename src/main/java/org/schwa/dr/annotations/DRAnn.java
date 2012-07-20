package org.schwa.dr.annotations;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;


/**
 * DRAnn can annotate:
 * * T, for T extends org.schwa.dr.BaseAnn
 *
 * @author Tim Dawborn
 */
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE})
public @interface DRAnn {
  String serial() default "";
}
