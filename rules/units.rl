/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  # we must subtract byte counts for units with prefixes since they get counted twice
  # one letter prefixes are "ckmsCFKM"
  # "m" (metres)
  # "s" (seconds)
  # "c" (cents)
  # "C", "F", "K" (Celsius, Fahrenheit and Kelvin)
  # "K", "k", "M" (kilo, mega, scalar multipliers)

	length1 = "m" @s1;
  length2 = ("in"|"ft"|"yd") @s2 | ("mi"|"mm"|"cm"|"km") @s1;
	length = length1 | length2;

  area_vol0 = length (("2"|"3") @s1 | ("^2"|"^3") @s2);
  area_vol1 = ("sq"|"cu") " " @s2 length;
  area_vol2 = ("sq."|"cu.") " " @s3 length;

  size = length | area_vol0 | area_vol1 | area_vol2;

  speed = ("m/s"|"mph") @s2 | ("kph"|"rpm") @s3 | "kmh" @s1 | "km/h" @s2;
  vol = ("L"|"mL") @s1 | ("pt"|"qt"|"mil") @s2 | "gal" @s3 |"fl oz" @s5;
  weight = ("g"|"mg"|"lbs") @s1 | ("cwt"|"kg"|"lb"|"oz"|"dr") @s2 | "ton" @s3;

  computer = ("kb"|"kB"|"Mb"|"MB"|"Mbit"|"MBit"|"Gb"|"GB"|"Gbit"|"GBit") @s2 | ("bps"|"dpi"|"kbit"|"mbit") @s3 | "gbit" @s4;

  # unicode U+00b0 degree sign
  unicode_00b0 = 0xc2 0xb0 @s2 | "&#176" @s5 | "&#xb0;"i @s6 | "&deg;" @s5;
  temperature = unicode_00b0? ("C"|"F"|"K") @s1;

  frequency = ("Hz"|"kHz"|"KHz"|"MHz") @s2 | "GHz" @s3;
  pressure = ("Pa"|"kPa") @s2 | ("hPa"|"psi") @s3;
  other = ("dB"|"no") @s2;

  period0 = ("nanosec"|"microsec"|"millisec"|"sec"|"min"|"hr"|"mo"|"mth"|"mnth"|"yr") "s"?;
  scalars = ("k"|"M") @s1;

	# unicode U+00a2 cent sign
	unicode_00a2 = 0xC2 0xA2 @s2 | "&#162;" @s6 | "&#xa2;"i @s6 | "&cent;" @s6;
	cents = "c" @s1 | unicode_00a2;

  post_currency = cents;

  ante_meridian = (("am"|"AM") @s2 | ("a.m."|"A.M.") @s4) @{n2="am";};
  post_meridian = (("pm"|"PM") @s2 | ("p.m."|"P.M.") @s4) @{n2="pm";};
  meridian = ante_meridian | post_meridian;

  units = size | speed | vol | weight | computer | other | period0 | scalars | post_currency | meridian;

  dollars = "$" | /[A-Z]+$/ | /$[A-Z]+/ | 0xe2 0x82 0xac | 0xe2 0x82 0xa4 | 0xe2 0x82 0xa3 | 0xc2 0xa2| 0xc2 0xa3 | 0xc2 0xa5;

  height = /[1-9][0-9]*/ single_quote /[0-9]+/ double_quote;
}%%
