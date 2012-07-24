package org.schwa.dr;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;


/**
 * {@link dr} is a container / namespace-emulator for housing the annotation's
 *
 * @author Tim Dawborn
 */
public class dr {
  /**
   * {@link Doc} can annotate:
   * <ul>
   * <li>{@code T}, for {@code T extends org.schwa.dr.Doc}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.TYPE})
  public static @interface Doc {
  }

  /**
   * {@link Ann} can annotate:
   * <ul>
   * <li>{@code T}, for {@code T extends org.schwa.dr.Ann}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.TYPE})
  public static @interface Ann {
    String serial() default "";
  }

  /**
   * {@link Store} can annotate:
   * <ul>
   * <li>{@link org.schwa.dr.Store}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.FIELD})
  public static @interface Store {
    String serial() default "";
  }

  /**
   * {@link Field} can annotate:
   * <ul>
   * <li>{@literal {byte,char,short,int,long,float,double,boolean,String}}</li>
   * <li>{@link org.schwa.dr.Slice}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.FIELD})
  public static @interface Field {
    String serial() default "";
  }

  /**
   * Pointer can annotate:
   * <ul>
   * <li>{@link org.schwa.dr.AnnSlice}</li>
   * <li>{@code T}, for {@code T extends org.schwa.dr.Ann}</li>
   * <li>{@link java.util.List<T>}, for {@code T extends org.schwa.dr.Ann}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.FIELD})
  public @interface Pointer {
    String store();
    String serial() default "";
  }
}
