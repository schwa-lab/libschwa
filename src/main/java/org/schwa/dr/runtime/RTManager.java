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

  public void addAnn(RTAnnSchema annSchema) {
    annSchemas.add(annSchema);
  }

  public RTAnnSchema getDocSchema() {
    return docSchema;
  }

  public RTAnnSchema getSchema(int index) {
    return annSchemas.get(index);
  }

  public List<RTAnnSchema> getSchemas() {
    return annSchemas;
  }

  public void setDocSchema(RTAnnSchema docSchema) {
    this.docSchema = docSchema;
  }
}
