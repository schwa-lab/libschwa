package org.schwa.dr.runtime;

import java.util.HashSet;
import java.util.Set;


public class RTManager {
  RTAnnSchema docSchema;
  Set<RTAnnSchema> annSchemas;

  public RTManager() {
    annSchemas = new HashSet<RTAnnSchema>();
  }
}
