package org.schwa.dr;

import org.schwa.dr.runtime.RTManager;


public abstract class AbstractDoc extends AbstractAnn implements Doc {
  private RTManager rt;

  @Override
  public RTManager getRT() {
    return rt;
  }

  @Override
  public void setRT(RTManager rt) {
    this.rt = rt;
  }
}
