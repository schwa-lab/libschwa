package org.schwa.dr;

import org.schwa.dr.runtime.RTManager;
import org.schwa.dr.runtime.RTFactory;
import org.schwa.dr.schemas.DocSchema;


public abstract class Doc extends Ann {
  private RTManager rt;

  RTManager createOrMergeRT(DocSchema docSchema) {
    if (rt == null)
      return RTFactory.build(docSchema);
    else
      return RTFactory.merge(rt, docSchema);
  }

  void setRT(RTManager rt) {
    this.rt = rt;
  }
}
