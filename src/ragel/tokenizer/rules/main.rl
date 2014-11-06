// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  action ignore { }
  action word { _word(); }
  action punctuation { _punctuation(); }
  action contraction { _contraction(); }
  action catchall { throw std::runtime_error("Stuck :("); }

  main := |*
    single_quote => { _single_quote(); };
    double_quote => { _double_quote(); };

    open_single_quote => { _open_single_quote(); };
    close_single_quote => { _close_single_quote(); };

    open_double_quote => { _open_double_quote(); };
    close_double_quote => { _close_double_quote(); };

    full_stop => { _terminator(reinterpret_cast<const uint8_t *>(u8".")); };
    question_mark => { _terminator(reinterpret_cast<const uint8_t *>(u8"?")); };
    inverted_question_mark => { _punctuation(reinterpret_cast<const uint8_t *>(u8"¿")); };
    exclamation_mark => { _terminator(reinterpret_cast<const uint8_t *>(u8"!")); };
    inverted_exclamation_mark => { _punctuation(reinterpret_cast<const uint8_t *>(u8"¡")); };
    ellipsis => { _terminator(reinterpret_cast<const uint8_t *>(u8"...")); };

    dash => { _punctuation(reinterpret_cast<const uint8_t *>(u8"--")); };

    unicode_space+ | unicode_line_space => ignore;
    # unicode_line_space{2,} | unicode_paragraph_space => { _sep_text_paragraph(); };

    contractions_neg => contraction;
    contractions_neg_error => contraction;
    letter+ contractions_suffix => contraction;

    (letter+ '.'? possessive) - abbrev_decade => { _split(); };
    # possessive => { _word(); }; # always capture 's

    (numbers units) - abbrev_decade => { _split(); };
    time_ambiguous meridian => { _split(); };
    meridian_token | date_time => word;

    (integer | float) '-' alpha+ ('-' alpha+)* => word;

    'and/or' | 'AND/OR' => word;

    contractions_misc | acronym | title => word;
    symbols => punctuation;
    emoticon => punctuation;
    date_abbrev | state | address_suffix => word;

    org | abbreviation | lines | currency_symbol | numbers | date_time => word;
    uri | email_address | twitter_username | hash_tag => word;
    default => word;

    unicode => catchall;
  *|;

}%%
