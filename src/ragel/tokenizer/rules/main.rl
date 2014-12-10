// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  action abbreviation { _abbreviation(); }
  action bigram { _bigram(); }
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

    open_bracket => { _open_bracket(); };
    close_bracket => { _close_bracket(); };

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

    (default '.'? possessive) -- abbrev_decade => split;
    (acronym1 | acronym2) possessive => split;

    (numbers cardinal_suffix) -- abbrev_decade => split;
    (numbers units) -- abbrev_decade => split;
    (numbers units '.') -- abbrev_decade => { _split(true); };
    (numbers %b1 unicode_space+ %b2 cardinal_suffix) -- abbrev_decade => bigram;
    (numbers %b1 unicode_space+ %b2 units '.') -- abbrev_decade => { _bigram(true); };
    time_ambiguous meridian => split;
    meridian_token | date_time => word;

    'and/or' | 'AND/OR' => word;
    /[Cc]an/ %b1 %b2 'not' | 'CAN' %b1 %b2 'NOT' => bigram;
    (abbreviation_date | month_name) %b1 unicode_space+ %b2 ('0'? [1-9] | [12] digit | '3' [01]) '.' %b3 unicode_space => { _month_day(); };
    abbreviation_bigram => bigram;
    (initial1 | initial2) %b1 unicode_space+ %b2 default_title => bigram;

    contractions_misc | title_prefix => word;
    symbols => punctuation;
    emoticon => punctuation;
    acronym1 | acronym2 | abbreviation | abbreviation_date | abbreviation_org | abbreviation_state | century_modern '.' | address_suffix | title_suffix => abbreviation;
    non_eos_abbreviation | initial1 | acronym3 => word;

    lines | currency_symbol | numbers | date_time => word;
    uri | email_address | twitter_username | hash_tag => word;
    default => word;

    unicode => ignore;
  *|;

}%%
