/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  action ignore {}
  action word { word_(WORD, dest, s); }
  action punct { punct_(PUNCTUATION, dest, s); }
  action end { end_(PUNCTUATION, dest, s); }
  action contraction { split_(WORD, CONTRACTION, dest, s); }
  action catchall {
    switch(errors){
      case ERROR_SKIP:
        break;
      case ERROR_CALL:
        error_(dest, s);
        break;
      case ERROR_THROW:
        die_(msg << "stuck on character " << (int)*p
                 << " at offset " << (p - s.offset));
        break;
      default:
        die_(msg << "illegal value " << errors << " for bad byte error handling");
    }
  }

  main := |*
    single_quote => { single_quote_(dest, s, eof); };
    double_quote => { double_quote_(dest, s, eof); };

    open_single_quote => { open_single_quote_(dest, s); };
    close_single_quote => { close_single_quote_(dest, s); };

    open_double_quote => { open_double_quote_(dest, s); };
    close_double_quote => { close_double_quote_(dest, s); };

    full_stop => { terminator_(dest, s, "."); };
    question_mark => { terminator_(dest, s, "?"); };
    exclamation_mark => { terminator_(dest, s, "!"); };
    ellipsis => { terminator_(dest, s, "..."); };

    dash => { dash_or_item_(dest, s); };
#    [1-9][0-9]* "." => { number_or_item_(dest, s); };

    (space - newline | other_ws)+ | newline => ignore;
    newline{2,} => { sep_text_paragraph_(dest, s); };

    open_p_tag => { begin_html_paragraph_(dest, s); };
    close_p_tag => { end_html_paragraph_(dest, s); };
    sep_p_tag => { sep_html_paragraph_(dest, s); };

    open_ul_tag | open_ol_tag => { begin_html_list_(dest, s); };
    close_ul_tag | close_ol_tag => { end_html_list_(dest, s); };

    open_li_tag => { begin_html_item_(dest, s); };
    close_li_tag => { end_html_item_(dest, s); };

    html_tag | html_comment | script_tag | style_tag => ignore;

    neg => contraction;
    neg_error => contraction;
    letter+ cont_suffix => contraction;

    (letter+ "."? possessive) - abbrev_decade => { split_(WORD, POSSESSIVE, dest, s); };

    (numbers units) - abbrev_decade => { split_(NUMBER, UNIT, dest, s); };
    time_ambiguous meridian => { split_(NUMBER, UNIT, dest, s); };
    meridian_token | date_time => word;

    (integer | float) "-" alpha+ ("-" alpha+)* => word;

    "and/or" | "AND/OR" => word;

    cont_misc | acron | title => word;
    symbols => punct;
    end_punct => end;
    date_abbrev | state | addr => word;

    org | abbrev | lines | dollars | numbers | date_time => word;
    uri | host_name | email_address | twitter_username | hash_tag => word;
    default => word;

    any => catchall;
  *|;

}%%
