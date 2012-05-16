/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  day_2digits = "0" [1-9] | [12] digit | "3" [01];
  day_digits = [1-9] | day_2digits;

  month_2digits = "0" [1-9] | "1" [0-2];
  month_digits = [1-9] | month_2digits;

  decade = digit "0";

	century = [1-9] | "1" digit | "2" [01];
  century_modern = "1" [7-9] | "2" [01];

	year = century digit{2};
  year_modern = century_modern digit{2};

  abbrev_year = single_quote digit{2};
  abbrev_decade = century? decade single_quote "s" | (century | single_quote) decade "s";

  illegal_30th = "0"? "2";
  illegal_31st = "0"? [469] | "11";

  date_uk_slash = day_digits "/" month_digits "/" year_modern;
  date_uk_dash = day_digits "-" month_digits "-" year_modern;
  date_uk_dot = day_digits "." month_digits "." year_modern;
  illegal_uk_dates = ("30" any illegal_30th | "31" any illegal_31st) any*;
  date_uk = (date_uk_slash | date_uk_dash | date_uk_dot) - illegal_uk_dates;

  date_us_slash = month_digits "/" day_digits "/" year_modern;
  date_us_dash = month_digits "-" day_digits "-" year_modern;
  date_us_dot = month_digits "." day_digits "." year_modern;
  illegal_us_dates = (illegal_30th any "30" | illegal_31st any "31") any*;
  date_us = (date_us_slash | date_us_dash | date_us_dot) - illegal_us_dates;

  # ISO 8601 dates only the full YYYY-MM-DD form
  #   http://en.wikipedia.org/wiki/ISO_8601
  #   http://www.cl.cam.ac.uk/~mgk25/iso-time.html
  # the dash is the only valid separator, but slash appears common too

  date_iso_slash = year_modern "/" month_2digits "/" day_2digits;
  date_iso_dash = year_modern "-" month_2digits "-" day_2digits;
  illegal_iso_dates = year_modern any (illegal_30th any "30" | illegal_31st any "31");
  date_iso = (date_iso_dash | date_iso_slash) - illegal_iso_dates;

  date = date_uk | date_us | date_iso | abbrev_year | abbrev_decade;

  hour_12 = [1-9] | "1" [0-2];
  hour_24 = "0"? [1-9] | "1" [0-9] | "2" [0-4];
  min_60 = [0-5][0-9];
  sec_60 = [0-5][0-9];

  time_colon = hour_12 | hour_24 ":" min_60;
  time_ambiguous = time_colon | hour_24 "." min_60;

  ante_meridian = (("am"|"AM") @s2 | ("a.m."|"A.M.") @s4) @{n2="am";};
  post_meridian = (("pm"|"PM") @s2 | ("p.m."|"P.M.") @s4) @{n2="pm";};
  meridian = ante_meridian | post_meridian;
  meridian_token = ("a.m."|"A.M.") @{n1="am";} | ("p.m."|"P.M.") @{n1="pm";};

  date_time = date | time_colon;

}%%
