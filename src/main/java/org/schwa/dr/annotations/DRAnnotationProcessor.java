package org.schwa.dr.annotations;

import java.util.Set;

import javax.annotation.processing.*;
import javax.lang.model.*;
import javax.lang.model.element.*;


@SupportedAnnotationTypes(value={"*"})
@SupportedSourceVersion(SourceVersion.RELEASE_6)
class DRAnnotationProcessor extends AbstractProcessor {
  @Override
  public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnv) {
    System.out.println("process(" + annotations + ", " + roundEnv + ")");
    for (TypeElement element : annotations) {
      System.out.println(element + " " + element.getQualifiedName());
    }
    return true;
  }
}
