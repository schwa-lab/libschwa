package org.schwa.dr;

import org.schwa.dr.runtime.RTManager;


public interface Doc extends Ann {
  public RTManager getRT();
  public void setRT(RTManager rt);
}
