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
public final class dr {
  private dr() { }

  /**
   * {@link dr.Doc} can annotate public classes of the form:
   * <ul>
   * <li>Any subclass of {@link Doc}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.TYPE})
  public static @interface Doc {
  }

  /**
   * {@link dr.Ann} can annotate public classes of the form:
   * <ul>
   * <li>Any subclass of {@link Ann}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.TYPE})
  public static @interface Ann {
    String serial() default "";
  }

  /**
   * {@link dr.Store} can annotate public member fields of the form:
   * <ul>
   * <li>{@link Store}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.FIELD})
  public static @interface Store {
    String serial() default "";
  }

  /**
   * {@link dr.Field} can annotate public member fields of the form:
   * <ul>
   * <li>Any primitive: {@code byte}, {@code char}, {@code short}, {@code int}, {@code long}, {@code float}, {@code double}, {@code boolean}, {@code String}</li>
   * <li>{@link ByteSlice}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.FIELD})
  public static @interface Field {
    String serial() default "";
  }

  /**
   * {@link dr.Pointer} can annotate public member fields of the form:
   * <ul>
   * <li>{@link AnnSlice}</li>
   * <li>Any object which is a subclass of {@link Ann}</li>
   * <li>A {@link java.util.List} of subclasses of {@link Ann}</li>
   * </ul>
   */
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.FIELD})
  public @interface Pointer {
    String store();
    String serial() default "";
  }
}
