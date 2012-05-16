/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  open_p_tag = "<p" any* :>> ">";
  close_p_tag = "</p" any* :>> ">";
  sep_p_tag = "<p" space* "/>";

  open_h_tag = "<h" [1-9] any* :>> ">";
  close_h_tag = "</h" [1-9] any* :>> ">";

  open_ul_tag = "<ul" any* :>> ">";
  close_ul_tag = "</ul" any* :>> ">";

  open_ol_tag = "<ol" any* :>> ">";
  close_ol_tag = "</ol" any* :>> ">";

  open_li_tag = "<li" any* :>> ">";
  close_li_tag = "</li" any* :>> ">";

  html_tag = "<" any* :>> ">";
  script_tag = "<script"i any* :>> ">" any* :>> "</script>"i;
  style_tag = "<style"i any* :>> ">" any* :>> "</style>"i;
  html_comment = "<!--" any* :>> "-->";

}%%
