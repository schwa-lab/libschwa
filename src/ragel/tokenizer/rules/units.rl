// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  # we must subtract byte counts for units with prefixes since they get counted twice
  # one letter prefixes are 'ckmsCFKMT'
  # 'm' (metres)
  # 's' (seconds)
  # 'c' (cents)
  # 'C', 'F', 'K' (Celsius, Fahrenheit and Kelvin)
  # 'T' (tonne)
  # 'K', 'k', 'M' (kilo, mega, scalar multipliers)

  # Length.
  unicode_212b = 0xe2 0x84 0xab ;  # U+212b angstrom sign (Å)
  length1 = 'm' @s1 ;
  length2 = ('in'|'ft'|'yd') @s2 | ('mi'|'mm'|'cm'|'km') @s1 ;
  length3 = unicode_212b ;
  length = length1 | length2 | length3 ;

  # Area/volume.
  area_vol0 = length (('2'|'3') @s1 | ('^2'|'^3') @s2);
  area_vol1 = ('sq ' @s2 |'cu ' @s3) length;
  area_vol2 = ('sq. ' @s3 |'cu. ' @s4) length;
  area_vol3 = 'cc' @s1;

  size = length | area_vol0 | area_vol1 | area_vol2 | area_vol3;

  # Speed.
  speed_ms = 'm/s' @s2 | 'ms-1' @s2;
  speed_kmh = ('kmh'|'km/hr') @s1 | ('km/h'|'kmph'|'kmh-1') @s2;
  speed_mih = ('mih'|'mi/hr') @s1 | 'mi/h' @s2 | 'mph' @s2 | 'mih-1' @s2;
  speed = speed_ms | speed_kmh | speed_mih;

  vol = ('L'|'mL'|'mil') @s1 | ('pt'|'qt') @s2 | 'gal' @s2 |'fl oz' @s5;
  weight = ('g'|'T'|'mg'|'lbs'|'dr'|'kg') @s1 | ('cwt'|'lb'|'oz') @s2 | 'ton' @s3;

  computer_size = ('kb'|'kB'|'KB'|'mb'|'Mb'|'MB'|'gb'|'TB'|'Tb') @s1 | ('kbit'|'mbit'|'Mbit'|'MBit'|'Gb'|'GB'|'gbit'|'Gbit'|'GBit') @s2;
  computer_rate = computer_size '/s' @s2 | ('bps'|'dpi') @s3;
  computer = computer_size | computer_rate;

  # Temperature.
  unicode_00b0 = 0xc2 0xb0 ;       # U+00b0 degree sign (°)
  unicode_2103 = 0xe2 0x84 0x83 ;  # U+2103 degree celsius (℃)
  unicode_2109 = 0xe2 0x84 0x89 ;  # U+2109 degree fahrenheit (℉)
  unicode_212a = 0xe2 0x84 0xaa ;  # U+212a kelvin sign (K)
  temperature =
      ( 'C' | 'F' | 'K' | unicode_2103 | unicode_2109 | unicode_212a ) @s1
    | unicode_00b0 ( 'C' | 'F' ) @s1
    ;

  frequency = ('Hz'|'kHz'|'KHz'|'MHz') @s2 | 'GHz' @s3;
  pressure = ('Pa'|'kPa'|'hPa') @s2 | 'psi' @s3;
  other = ('db'|'dB') @s2 | ('rpm'|'RPM') @s3;

  period_si = ('s'|'ms'|'min'|'h') @s1 | ('us'|'ns') @s2 | (0xce 0xbc 's') @s3;
  period_expanded = 'nanosec' @s7 | ('microsec'|'millisec') @s8;
  period_non_si = 'hr' @s1 | ('yr'|'sec'|'mth') @s2 | 'mnth' @s3;
  period_non_si_plural = period_non_si 's' @s1 | 'mins' @s1;
  period = period_si | period_expanded | period_non_si | period_non_si_plural;

  scalars = ('k'|'M') @s1;

  # Cents.
  unicode_00a2 = 0xc2 0xa2 ;  # U+00a2 cent sign (¢)
  cents = 'c' @s1 | unicode_00a2;
  post_currency = cents;

  # Any units that may appear after a number.
  units =
      computer
    | frequency
    | other
    | period
    | post_currency
    | pressure
    | scalars
    | size
    | speed
    | temperature
    | vol
    | weight
    ;

  # Any currency sumbol.
  unicode_00a3 = 0xc2 0xa3 ;       # U+00a3 pound sign (£)
  unicode_00a5 = 0xc2 0xa5 ;       # U+00a5 yen sign (¥)
  unicode_20a0 = 0xe2 0x82 0xa0 ;  # U+20a0 euro-currency sign (₠)
  unicode_20a1 = 0xe2 0x82 0xa1 ;  # U+20a1 colon sign (₡)
  unicode_20a2 = 0xe2 0x82 0xa2 ;  # U+20a2 cruzeiro sign (₢)
  unicode_20a3 = 0xe2 0x82 0xa3 ;  # U+20a3 french franc sign (₣)
  unicode_20a4 = 0xe2 0x82 0xa4 ;  # U+20a4 lira sign (₤)
  unicode_20a5 = 0xe2 0x82 0xa5 ;  # U+20a5 mill sign (₥)
  unicode_20a6 = 0xe2 0x82 0xa6 ;  # U+20a6 naira sign (₦)
  unicode_20a7 = 0xe2 0x82 0xa7 ;  # U+20a7 peseta sign (₧)
  unicode_20a8 = 0xe2 0x82 0xa8 ;  # U+20a8 rupee sign (₨)
  unicode_20a9 = 0xe2 0x82 0xa9 ;  # U+20a9 won sign (₩)
  unicode_20aa = 0xe2 0x82 0xaa ;  # U+20aa new sheqel sign (₪)
  unicode_20ab = 0xe2 0x82 0xab ;  # U+20ab dong sign (₫)
  unicode_20ac = 0xe2 0x82 0xac ;  # U+20ac euro sign (€)
  unicode_20ad = 0xe2 0x82 0xad ;  # U+20ad kip sign (₭)
  unicode_20ae = 0xe2 0x82 0xae ;  # U+20ae tugrik sign (₮)
  unicode_20af = 0xe2 0x82 0xaf ;  # U+20af drachma sign (₯)
  unicode_20b0 = 0xe2 0x82 0xb0 ;  # U+20b0 german penny sign (₰)
  unicode_20b1 = 0xe2 0x82 0xb1 ;  # U+20b1 peso sign (₱)
  unicode_20b2 = 0xe2 0x82 0xb2 ;  # U+20b2 guarani sign (₲)
  unicode_20b3 = 0xe2 0x82 0xb3 ;  # U+20b3 austral sign (₳)
  unicode_20b4 = 0xe2 0x82 0xb4 ;  # U+20b4 hryvnia sign (₴)
  unicode_20b5 = 0xe2 0x82 0xb5 ;  # U+20b5 cedi sign (₵)
  unicode_20b6 = 0xe2 0x82 0xb6 ;  # U+20b6 livre tournois sign (₶)
  unicode_20b7 = 0xe2 0x82 0xb7 ;  # U+20b7 spesmilo sign (₷)
  unicode_20b8 = 0xe2 0x82 0xb8 ;  # U+20b8 tenge sign (₸)
  unicode_20b9 = 0xe2 0x82 0xb9 ;  # U+20b9 indian rupee sign (₹)
  unicode_20ba = 0xe2 0x82 0xba ;  # U+20ba turkish lira sign (₺)
  unicode_20bb = 0xe2 0x82 0xbb ;  # U+20bb  (₻)
  unicode_20bc = 0xe2 0x82 0xbc ;  # U+20bc  (₼)
  unicode_20bd = 0xe2 0x82 0xbd ;  # U+20bd  (₽)
  currency_symbol =
      '$'
    | [A-Z]+ '$'
    | '$' [A-Z]+
    | unicode_00a2 | unicode_00a3 | unicode_00a5 | unicode_20a0 | unicode_20a1 | unicode_20a2 | unicode_20a3 | unicode_20a4 | unicode_20a5 | unicode_20a6 | unicode_20a7 | unicode_20a8 | unicode_20a9 | unicode_20aa | unicode_20ab | unicode_20ac | unicode_20ad | unicode_20ae | unicode_20af | unicode_20b0 | unicode_20b1 | unicode_20b2 | unicode_20b3 | unicode_20b4 | unicode_20b5 | unicode_20b6 | unicode_20b7 | unicode_20b8 | unicode_20b9 | unicode_20ba | unicode_20bb | unicode_20bc | unicode_20bd
    ;

}%%
