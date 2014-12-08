// vim: ft=ragel:
/* -*- Mode: C++ ; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer ;
  alphtype unsigned char ;

  month_name =
      'January'i
    | 'February'i
    | 'March'i
    | 'April'i
    | 'May'i
    | 'June'i
    | 'July'i
    | 'August'i
    | 'September'i
    | 'October'i
    | 'November'i
    | 'December'i
    ;

  day_2digits = '0' [1-9] | [12] digit | '3' [01] ;
  day_digits = [1-9] | day_2digits ;

  month_2digits = '0' [1-9] | '1' [0-2] ;
  month_digits = [1-9] | month_2digits ;

  decade = digit '0' ;

  century = [1-9] | '1' digit | '2' [01] ;
  century_modern = '1' [7-9] | '2' [01] ;

  year = century digit{2} ;
  year_modern = century_modern digit{2} ;
  year_modern_digits = digit{2} | year_modern ;

  abbrev_year = single_quote digit{2} ;
  abbrev_decade = century? decade single_quote 's' | (century | single_quote) decade 's' ;

  date_uk_slash = day_digits '/' month_digits '/' year_modern_digits ;
  date_uk_dash = day_digits '-' month_digits '-' year_modern_digits ;
  date_uk_dot = day_digits '.' month_digits '.' year_modern_digits ;
  date_uk = date_uk_slash | date_uk_dash | date_uk_dot ;

  date_us_slash = month_digits '/' day_digits '/' year_modern_digits ;
  date_us_dash = month_digits '-' day_digits '-' year_modern_digits ;
  date_us_dot = month_digits '.' day_digits '.' year_modern_digits ;
  date_us = date_us_slash | date_us_dash | date_us_dot ;

  # ISO 8601 dates only the full YYYY-MM-DD form
  #   http://en.wikipedia.org/wiki/ISO_8601
  #   http://www.cl.cam.ac.uk/~mgk25/iso-time.html
  # the dash is the only valid separator, but slash appears common too
  date_iso_slash = year_modern '/' month_2digits '/' day_2digits ;
  date_iso_dash = year_modern '-' month_2digits '-' day_2digits ;
  date_iso = date_iso_dash | date_iso_slash ;

  date = date_uk | date_us | date_iso | abbrev_year | abbrev_decade ;

  hour_12 = [1-9] | '1' [0-2] ;
  hour_24 = '0'? [1-9] | '1' [0-9] | '2' [0-4] ;
  min_60 = [0-5][0-9] ;
  sec_60 = [0-5][0-9] ;
  time_colon = ( hour_12 | hour_24 ) ':' min_60 ( ':' sec_60 )? ;
  time_ambiguous = time_colon | hour_24 '.' min_60 ( '.' sec_60 )? ;

  ante_meridian =
      ( ( 'am' @s2 | 'a.m.' @s4 ) @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"a.m."); } )
    | ( ( 'AM' @s2 | 'A.M.' @s4 ) @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"A.M."); } )
    ;
  post_meridian =
      ( ( 'pm' @s2 | 'p.m.' @s4 ) @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"p.m."); } )
    | ( ( 'PM' @s2 | 'P.M.' @s4 ) @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"P.M."); } )
    ;
  meridian = ante_meridian | post_meridian ;
  meridian_token =
      ('a.m.' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"a.m."); } )
    | ('A.M.' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"A.M."); } )
    | ('p.m.' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"p.m."); } )
    | ('P.M.' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"P.M."); } )
    ;

  date_time = date | time_colon ;

}%%
