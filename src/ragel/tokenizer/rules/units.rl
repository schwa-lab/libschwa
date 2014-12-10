// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  # Metric prefixes (https://en.wikipedia.org/wiki/Metric_prefix)
  unicode_00b5 = 0xc2 0xb5 ;  # U+00b5 micro sign (µ)
  unicode_03bc = 0xce 0xbc ;  # U+03bc greek small letter mu (μ)
  metric_prefix_big = [hkMGTPEZY] ;
  metric_prefix_small = [dcmunpfazy] | unicode_00b5 | unicode_03bc ;
  metric_prefix = metric_prefix_big | metric_prefix_small ;

  unicode_212b = 0xe2 0x84 0xab ;  # U+212b angstrom sign (Å)
  length_unit =
      'mm' %s2
    | 'cm' %s2
    | 'm' %s1
    | 'km' %s2
    | 'in' %s2
    | 'ft' %s2
    | 'yd' %s2
    | 'mi' %s2
    | unicode_212b %s1
    ;

  size_unit =
      length_unit
    | length_unit ('2'|'3') %s1
    | length_unit ('^2'|'^3') %s2
    ;

  speed_unit =
      'm/s' %s3
    | 'ms-1' %s4
    | 'kmh' %s3
    | 'km/h' %s4
    | 'kmph' %s4
    | 'km/hr' %s5
    | 'kmh-1' %s5
    | 'mih' %s3
    | 'mph' %s3
    | 'mi/h' %s4
    | 'mi/hr' %s5
    | 'mih-1' %s5
    ;

  volume_unit =
      'cc' %s2
    | 'L' %s1
    | 'fl oz' %s5
    | 'gal' %s4
    | 'mL' %s2
    | 'mil' %s3
    | 'pt' %s2
    | 'qt' %s2
    ;

  weight_unit =
      'T' %s1
    | 'cwt' %s3
    | 'dr' %s2
    | 'g' %s1
    | 'kg' %s2
    | 'lb' %s2
    | 'lbs' %s3
    | 'mg' %s2
    | 'oz' %s2
    | 'ton' %s3
    ;

  computer_size_unit =
      'B' %s1
    | metric_prefix_big 'B' %s2
    | metric_prefix_big 'Bit' %s4
    | [MGTPEZY] 'b' %s2
    | [MGTPEZY] 'bit' %s4
    | [kmgtpezy] 'b' %s2
    | [kmgtpezy] 'bit' %s4
    | [KMGTPEZY] 'iB' %s3
    | [KMGTPEZY] 'iBit' %s5
    ;

  computer_rate_unit =
      computer_size_unit '/s' %s2
    | 'bps' %s3
    | 'dpi' %s3
    ;

  unicode_00b0 = 0xc2 0xb0 ;       # U+00b0 degree sign (°)
  unicode_2103 = 0xe2 0x84 0x83 ;  # U+2103 degree celsius (℃)
  unicode_2109 = 0xe2 0x84 0x89 ;  # U+2109 degree fahrenheit (℉)
  unicode_212a = 0xe2 0x84 0xaa ;  # U+212a kelvin sign (K)
  temperature_unit =
      ( 'C' | 'F' | 'K' | unicode_2103 | unicode_2109 | unicode_212a ) %s1
    | unicode_00b0 ( 'C' | 'F' ) %s2
    ;

  frequency_unit =
      'Hz' %s2
    | metric_prefix 'Hz' %s3
    ;

  pressure_unit =
      'Pa' %s2
    | metric_prefix 'Pa' %s3
    | 'psi'
    ;

  other_unit =
      'db'
    | 'dB'
    | 'rpm'
    | 'RPM'
    ;

  # Time.
  period_unit_si =
      's' %s1
    | ('m' | 'u' | unicode_00b5 | unicode_03bc | 'n' ) 's' %s2
    | 'min' %s3
    | 'h' %s1
    ;
  period_unit_expanded =
      'millisec' %s8
    | 'microsec' %s8
    | 'nanosec' %s7
    ;
  period_unit_non_si =
      'hr' %s2
    | 'mth' %s3
    | 'mnth' %s4
    | 'sec' %s3
    | 'yr' %s2
    ;
  period_unit_non_si_plural =
      period_unit_non_si 's' %s1
    | 'mins' %s4
    ;
  period_unit = period_unit_si | period_unit_expanded | period_unit_non_si | period_unit_non_si_plural ;

  post_integer_scalar = [kMB] %s1 ;

  cardinal_suffix =
      'thousand'i %s8
    | 'million'i %s7
    | 'billion'i %s7
    | 'trillion'i %s8
    ;

  unicode_00a2 = 0xc2 0xa2 ;  # U+00a2 cent sign (¢)
  cents_unit =
      unicode_00a2 %s1
    ;

  currency_symbol =
      [A-Z]+ '$'
    | '$' [A-Z]+
    | unicode_symbol_currency
    ;
  post_currency_unit = cents_unit;

  # Any units that may appear after a number.
  units =
      computer_rate_unit
    | computer_size_unit
    | frequency_unit
    | other_unit
    | period_unit
    | post_currency_unit
    | post_integer_scalar
    | pressure_unit
    | speed_unit
    | size_unit
    | temperature_unit
    | volume_unit
    | weight_unit
    ;

}%%
