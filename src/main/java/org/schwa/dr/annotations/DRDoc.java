package org.schwa.dr.annotations;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;


/**
 * DRDoc can annotate:
 * * T, for T extends org.schwa.dr.BaseDoc
 *
 * @author Tim Dawborn
 */
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE})
public @interface DRDoc {
}
