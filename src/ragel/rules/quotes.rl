/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  # ASCII single and double quote characters are ambiguous
  # PTB and LaTeX convention of double single quotes cannot be
  # relied upon either so the '' quotes need to go here...
  single_quote = "'" | "&#39;" | "&apos;";
  double_quote = "\"" | "&#34;" | "&quot;" | "''";

  # PTB and LaTeX convention of a single back quote
  # unicode U+2018 left single quotation mark
  # windows 1250-1258 code pages 0x91 -> unicode U+2018
  # unicode U+201b single high-reversed-9 quotation mark
  # "has same semantic as U+2018, but differs in appearance"
  # unicode U+275b heavy single turned comma quotation mark ornament
  unicode_2018 = 0xe2 0x80 0x98 | "&#8216;" | "&#x2018;"i | "&lsquo;" | 0x91;
  unicode_201b = 0xe2 0x80 0x9b | "&#8219;" | "&#x201b;"i;
  unicode_275b = 0xe2 0x9d 0x9b | "&#10075;" | "&#x275b;"i;
  open_single_quote = "`" | unicode_2018 | unicode_201b | unicode_275b;

  # unicode U+2019 right single quotation mark
  # windows 1250-1258 code pages 0x92 -> unicode U+2019
  # unicode U+275c heavy single comma quotation mark ornament
  unicode_2019 = 0xe2 0x80 0x99 | "&#8217;" | "&#x2019;"i | "&rsquo;" | 0x92;
  unicode_275c = 0xe2 0x9d 0x9c | "&#10076;" | "&#x275c;"i;
  close_single_quote = unicode_2019 | unicode_275c;

  possessive = ("'s"i @s2 | "&#39;s"i @s6 | "&apos;" "s"i @s7 | 0xe2 0x80 0x99 "s"i @s4 | "&#8217;" "s"i @s8 | "&#x2019;s"i @s9 | "&rsquo;" "s"i @s8 | 0x91 "s"i @s2) @{n2="'s";};

  # PTB and LaTeX convention of double back quotes
  # unicode U+201C left double quotation mark
  # windows 1250-1258 code pages 0x93 -> unicode U+201c
  # unicode U+201F double high-reversed-9 quotation mark
  # "has same semantic as U+201C, but differs in appearance"
  # unicode U+275d heavy double turned comma quotation mark ornament
  unicode_201c = 0xe2 0x80 0x9c | "&#8220;" | "&#x201c;"i | "&ldquo;" | 0x93;
  unicode_201f = 0xe2 0x80 0x9f | "&#8223;" | "&#x201f"i;
  unicode_275d = 0xe2 0x9d 0x9d | "&#10077;" | "&#x275d;"i;

  # Fairfax appears to use 0x1c as an open double quote (may be an db/encoding error)
  # in the online rendering of these are translated 0x80 0x9c (almost UTF w/o 0xe2)
  # Fairfax also uses doubled single quotes (unicode U+2018)
  fairfax_open_double = 0x1c | unicode_2018 unicode_2018;

  open_double_quote = "``" | unicode_201c | unicode_201f | unicode_275d | fairfax_open_double when FAIRFAX;

  # unicode U+201D right double quotation mark
  # windows 1250-1258 code pages 0x94 -> unicode U+201d
  # unicode U+275e heavy double comma quotation mark ornament
  unicode_201d = 0xe2 0x80 0x9d | "&#8221;" | "&#x201d;"i | "&rdquo;" | 0x94;
  unicode_275e = 0xe2 0x9d 0x9e | "&#10078;" | "&#x275e;"i;

  # Fairfax appears to use 0x1d as an close double quote (may be an db/encoding error)
  # in the online rendering of these are translated 0x80 0x9d (almost UTF w/o 0xe2)
  # Fairfax also uses doubled single quotes (unicode U+2019)
  fairfax_close_double = 0x1d  | unicode_2019 unicode_2019;

  close_double_quote = unicode_201d | unicode_275e | fairfax_close_double when FAIRFAX;
}%%
