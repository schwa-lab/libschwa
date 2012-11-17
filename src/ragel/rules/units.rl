/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  # we must subtract byte counts for units with prefixes since they get counted twice
  # one letter prefixes are "ckmsCFKMT"
  # "m" (metres)
  # "s" (seconds)
  # "c" (cents)
  # "C", "F", "K" (Celsius, Fahrenheit and Kelvin)
  # "T" (tonne)
  # "K", "k", "M" (kilo, mega, scalar multipliers)

  length1 = "m" @s1;
  length2 = ("in"|"ft"|"yd") @s2 | ("mi"|"mm"|"cm"|"km") @s1;
  length = length1 | length2;

  area_vol0 = length (("2"|"3") @s1 | ("^2"|"^3") @s2);
  area_vol1 = ("sq " @s2 |"cu " @s3) length;
  area_vol2 = ("sq. " @s3 |"cu. " @s4) length;
  area_vol3 = "cc" @s1;

  size = length | area_vol0 | area_vol1 | area_vol2 | area_vol3;

  speed_ms = "m/s" @s2 | "ms-1" @s2;
  speed_kmh = ("kmh"|"km/hr") @s1 | ("km/h"|"kmph"|"kmh-1") @s2;
  speed_mih = ("mih"|"mi/hr") @s1 | "mi/h" @s2 | "mph" @s2 | "mih-1" @s2;
  speed = speed_ms | speed_kmh | speed_mih;

  vol = ("L"|"mL"|"mil") @s1 | ("pt"|"qt") @s2 | "gal" @s2 |"fl oz" @s5;
  weight = ("g"|"T"|"mg"|"lbs"|"dr"|"kg") @s1 | ("cwt"|"lb"|"oz") @s2 | "ton" @s3;

  computer_size = ("kb"|"kB"|"KB"|"mb"|"Mb"|"MB"|"gb"|"TB"|"Tb") @s1 | ("kbit"|"mbit"|"Mbit"|"MBit"|"Gb"|"GB"|"gbit"|"Gbit"|"GBit") @s2;
  computer_rate = computer_size "/s" @s2 | ("bps"|"dpi") @s3;
  computer = computer_size | computer_rate;

  # unicode U+00b0 degree sign
  unicode_00b0 = 0xc2 0xb0 @s2 | "&#176" @s5 | "&#xb0;"i @s6 | "&deg;" @s5;
  temperature = ("C"|"F"|"K") @s1 | unicode_00b0 ("C"|"F") @s1;

  frequency = ("Hz"|"kHz"|"KHz"|"MHz") @s2 | "GHz" @s3;
  pressure = ("Pa"|"kPa"|"hPa") @s2 | "psi" @s3;
  other = ("db"|"dB") @s2 | ("rpm"|"RPM") @s3;

  period_si = ("s"|"ms"|"min"|"h") @s1 | ("us"|"ns") @s2 | (0xce 0xbc "s") @s3;
  period_expanded = "nanosec" @s7 | ("microsec"|"millisec") @s8;
  period_non_si = "hr" @s1 | ("yr"|"sec"|"mth") @s2 | "mnth" @s3;
  period_non_si_plural = period_non_si "s" @s1 | "mins" @s1;
  period = period_si | period_expanded | period_non_si | period_non_si_plural;

  scalars = ("k"|"M") @s1;

  # unicode U+00a2 cent sign
  unicode_00a2 = 0xC2 0xA2 @s2 | "&#162;" @s6 | "&#xa2;"i @s6 | "&cent;" @s6;
  cents = "c" @s1 | unicode_00a2;

  post_currency = cents;

  units = size | speed | vol | weight | computer | temperature | frequency | pressure | other | period | scalars | post_currency;

  dollars = "$" | [A-Z]+ "$" | "$" [A-Z]+ | 0xe2 0x82 0xac | 0xe2 0x82 0xa4 | 0xe2 0x82 0xa3 | 0xc2 0xa2| 0xc2 0xa3 | 0xc2 0xa5;

  height = [1-9][0-9]* single_quote [0-9]+ double_quote;
}%%
