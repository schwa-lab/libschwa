// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  action ignore {}
  action word { _word(WORD, dest, s); }
  action punct { _punct(PUNCTUATION, dest, s); }
  action end { _end(PUNCTUATION, dest, s); }
  action contraction { _split(WORD, CONTRACTION, dest, s); }
  action catchall {
    switch (onerror) {
      case OnError::SKIP:
        break;
      case OnError::CALL:
        _error(dest, s);
        break;
      case OnError::THROW: {
          std::ostringstream msg;
          msg << "stuck on character " << (int)*p << " at offset " << (p - s.offset);
          throw TokenError(msg.str());
        }
        break;
    }
  }

  main := |*
    single_quote => { _single_quote(dest, s, eof); };
    double_quote => { _double_quote(dest, s, eof); };

    open_single_quote => { _open_single_quote(dest, s); };
    close_single_quote => { _close_single_quote(dest, s); };

    open_double_quote => { _open_double_quote(dest, s); };
    close_double_quote => { _close_double_quote(dest, s); };

    full_stop => { _terminator(dest, s, reinterpret_cast<const uint8_t *>(u8".")); };
    question_mark => { _terminator(dest, s, reinterpret_cast<const uint8_t *>(u8"?")); };
    inverted_question_mark => { _punct(PUNCTUATION, dest, s, reinterpret_cast<const uint8_t *>(u8"¿")); };
    exclamation_mark => { _terminator(dest, s, reinterpret_cast<const uint8_t *>(u8"!")); };
    inverted_exclamation_mark => { _punct(PUNCTUATION, dest, s, reinterpret_cast<const uint8_t *>(u8"¡")); };
    ellipsis => { _terminator(dest, s, reinterpret_cast<const uint8_t *>(u8"...")); };

    dash => { _dash_or_item(dest, s); };
#    [1-9][0-9]* "." => { _number_or_item(dest, s); };

    unicode_space+ | unicode_line_space => ignore;
    unicode_line_space{2,} | unicode_paragraph_space => { _sep_text_paragraph(dest, s); };

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

    cont_misc | acronym | title => word;
    symbols => punct;
    end_punct => end;
    emoticon => punct;
    date_abbrev | state | address_suffix => word;

    org | abbreviation | lines | currency_symbol | numbers | date_time => word;
    uri | email_address | twitter_username | hash_tag => word;
    default => word;

    unicode => catchall;
  *|;

}%%
