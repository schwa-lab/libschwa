package org.schwa.dr.runtime;

import java.util.ArrayList;
import java.util.List;


public final class RTManager {
  RTAnnSchema docSchema;
  List<RTAnnSchema> annSchemas;

  RTManager(RTAnnSchema docSchema) {
    this.docSchema = docSchema;
    this.annSchemas = new ArrayList<RTAnnSchema>();
  }

  void addAnn(RTAnnSchema annSchema) {
    annSchemas.add(annSchema);
  }
}
