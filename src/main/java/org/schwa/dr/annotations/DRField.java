package org.schwa.dr.annotations;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;


/**
 * DRField can annotate:
 * * {byte,char,short,int,long,float,double,boolean,String}
 * * org.schwa.dr.Slice
 *
 * @author Tim Dawborn
 */
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.FIELD})
public @interface DRField {
  String serial() default "";
}
