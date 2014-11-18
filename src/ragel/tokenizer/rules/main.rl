// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  action contraction { _contraction(); }
  action ignore { _ignore(); }
  action punctuation { _punctuation(); }
  action split { _split(); }
  action word { _word(); }

  main := |*
    single_quote => { _single_quote(); };
    double_quote => { _double_quote(); };

    open_single_quote => { _open_single_quote(); };
    close_single_quote => { _close_single_quote(); };

    open_double_quote => { _open_double_quote(); };
    close_double_quote => { _close_double_quote(); };

    full_stop => { _terminator(NORMALISED_PERIOD); };
    question_mark => { _terminator(NORMALISED_QUESTION_MARK); };
    inverted_question_mark => { _punctuation(NORMALISED_INVERTED_QUESTION_MARK); };
    exclamation_mark => { _terminator(NORMALISED_EXCLAMATION_MARK); };
    inverted_exclamation_mark => { _punctuation(NORMALISED_INVERTED_EXCLAMATION_MARK); };
    ellipsis => { _punctuation(NORMALISED_ELLIPSIS); };

    dash => { _punctuation(NORMALISED_DASH); };

    unicode_space+ | unicode_line_space => ignore;

    contractions_neg => contraction;
    contractions_neg_error => contraction;
    letter+ contractions_suffix => contraction;

    (letter+ '.'? possessive) - abbrev_decade => split;

    (numbers units) - abbrev_decade => split;
    time_ambiguous meridian => split;
    meridian_token | date_time => word;

    (integer | float) '-' alpha+ ('-' alpha+)* => word;

    'and/or' | 'AND/OR' => word;

    contractions_misc | acronym | title => word;
    symbols => punctuation;
    emoticon => punctuation;
    abbreviation_date | state | address_suffix => word;

    abbreviation_org | abbreviation | lines | currency_symbol | numbers | date_time => word;
    uri | email_address | twitter_username | hash_tag => word;
    default => word;

    unicode => ignore;
  *|;

}%%
