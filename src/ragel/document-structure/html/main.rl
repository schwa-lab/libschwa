// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine html;

  include "html.rl";

  main := |*
    comment;
    cdata;
    doctype;

    void_tag;
    start_tag;
    end_tag;

    text+;

  *|;

}%%
