// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  action ignore {}
  action word { _word(WORD, dest, s); }
  action punct { _punct(PUNCTUATION, dest, s); }
  action end { _end(PUNCTUATION, dest, s); }
  action contraction { _split(WORD, CONTRACTION, dest, s); }
  action catchall {
    switch(errors){
      case ERROR_SKIP:
        break;
      case ERROR_CALL:
        _error(dest, s);
        break;
      case ERROR_THROW:
        _die(msg << "stuck on character " << (int)*p << " at offset " << (p - s.offset));
        break;
      default:
        _die(msg << "illegal value " << errors << " for bad byte error handling");
    }
  }

  main := |*
    single_quote => { _single_quote(dest, s, eof); };
    double_quote => { _double_quote(dest, s, eof); };

    open_single_quote => { _open_single_quote(dest, s); };
    close_single_quote => { _close_single_quote(dest, s); };

    open_double_quote => { _open_double_quote(dest, s); };
    close_double_quote => { _close_double_quote(dest, s); };

    full_stop => { _terminator(dest, s, "."); };
    question_mark => { _terminator(dest, s, "?"); };
    exclamation_mark => { _terminator(dest, s, "!"); };
    ellipsis => { _terminator(dest, s, "..."); };

    dash => { _dash_or_item(dest, s); };
#    [1-9][0-9]* "." => { _number_or_item(dest, s); };

    (space - newline | other_ws)+ | newline => ignore;
    newline{2,} => { _sep_text_paragraph(dest, s); };

    open_p_tag => { _begin_html_paragraph(dest, s); };
    close_p_tag => { _end_html_paragraph(dest, s); };
    sep_p_tag => { _sep_html_paragraph(dest, s); };

    open_h_tag => { _begin_html_heading(dest, s); };
    close_h_tag => { _end_html_heading(dest, s); };

    open_ul_tag | open_ol_tag => { _begin_html_list(dest, s); };
    close_ul_tag | close_ol_tag => { _end_html_list(dest, s); };

    open_li_tag => { _begin_html_item(dest, s); };
    close_li_tag => { _end_html_item(dest, s); };

    html_tag | html_comment | script_tag | style_tag => ignore;

    neg => contraction;
    neg_error => contraction;
    letter+ cont_suffix => contraction;

    (letter+ "."? possessive) - abbrev_decade => { _split(WORD, POSSESSIVE, dest, s); };
    possessive => { _word(POSSESSIVE, dest, s); }; # always capture 's

    (numbers units) - abbrev_decade => { _split(NUMBER, UNIT, dest, s); };
    time_ambiguous meridian => { _split(NUMBER, UNIT, dest, s); };
    meridian_token | date_time => word;

    (integer | float) "-" alpha+ ("-" alpha+)* => word;

    "and/or" | "AND/OR" => word;

    cont_misc | acron | title => word;
    symbols => punct;
    end_punct => end;
#    emoticon => punct;
    date_abbrev | state | addr => word;

    org | abbrev | lines | dollars | numbers | date_time => word;
    uri | host_name | email_address | twitter_username | hash_tag => word;
    default => word;

    any => catchall;
  *|;

}%%
