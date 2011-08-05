/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  action ignore {}
  action word { word_(WORD, dest, s); }
  action punct { punct_(PUNCTUATION, dest, s); }

  main := |*
    single_quote => { single_quote_(dest, s); };
    double_quote => { double_quote_(dest, s); };

    open_single_quote => { open_single_quote_(dest, s); };
    close_single_quote => { close_single_quote_(dest, s); };

    open_double_quote => { open_double_quote_(dest, s); };
    close_double_quote => { close_double_quote_(dest, s); };

    full_stop => { terminator_(dest, s, "."); };
    question_mark => { terminator_(dest, s, "?"); };
    exclamation_mark => { terminator_(dest, s, "!"); };
    ellipsis => { terminator_(dest, s, "..."); };

    dash => { dash_or_item_(dest, s); };

    (space | other_ws)+ | newline => ignore;
    newline {2,} => { sep_text_paragraph_(dest, s); };

    open_p_tag => { begin_html_paragraph_(dest, s); };
    close_p_tag => { end_html_paragraph_(dest, s); };
    sep_p_tag => { sep_html_paragraph_(dest, s); };

    open_ul_tag | open_ol_tag => { begin_html_list_(dest, s); };
    close_ul_tag | close_ol_tag => { end_html_list_(dest, s); };

    open_li_tag => { begin_html_item_(dest, s); };
    close_li_tag => { end_html_item_(dest, s); };

    html_tag | html_comment | script_tag | style_tag => ignore;

    neg => { split_(WORD, CONTRACTION, dest, s); };
    neg_lc_error | neg_tc_error => { split_(WORD, CONTRACTION, dest, s); };
    neg_uc_error => { split_(WORD, CONTRACTION, dest, s); };

    letter+ (cont_1_lc | cont_1_uc) => { split_(WORD, CONTRACTION, dest, s); };
    letter+ (cont_2_lc | cont_2_uc) => { split_(WORD, CONTRACTION, dest, s); };

    letter+ "."? apostrophe_s "s"i @s1 @{n2="'s";} => { split_(WORD, POSSESSIVE, dest, s); };

    numbers units => { split_(NUMBER, UNIT, dest, s); };
    time meridian => { split_(NUMBER, UNIT, dest, s); };

    ("a.m."|"A.M.") @{n1="am";} | ("p.m."|"P.M.") @{n1="pm";} => word;

    (integer | float) "-" alpha+ ("-" alpha+)* => word;

    "and/or" | "AND/OR" => word;

    cont => word;

    acron | title => word;
    symbols => punct;
    date_abbrev | state | addr => word;

    org | abbrev | lines | dollars | numbers => word;
    uri | host_name | email_address => word;

    default => word;
  *|;

}%%
