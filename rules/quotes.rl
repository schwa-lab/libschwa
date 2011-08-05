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
  # unicode U+201b single high-reversed-9 quotation mark
  # "has same semantic as U+2018, but differs in appearance"
  # unicode U+275b heavy single turned comma quotation mark ornament
  unicode_2018 = 0xe2 0x80 0x98 | "&#8216;" | "&#x2018;"i | "&lsquo;";
  unicode_201b = 0xe2 0x80 0x9b | "&#8219;" | "&#x201b;"i;
  unicode_275b = 0xe2 0x9d 0x9b | "&#10075;" | "&#x275b;"i;
  open_single_quote = "`" | unicode_2018 | unicode_201b | unicode_275b;

  # unicode U+2019 right single quotation mark
  # unicode U+275c heavy single comma quotation mark ornament
  unicode_2019 = 0xe2 0x80 0x99 | "&#8217;" | "&#x2019;"i | "&rsquo;";
  unicode_275c = 0xe2 0x9d 0x9c | "&#10076;" | "&#x275c;"i;
  close_single_quote = unicode_2019 | unicode_275c;

  apostrophe_s = "'" @s1 | "&#39;" @s5 | "&apos;" @s6 | 0xe2 0x80 0x99 @s3 | "&#8217;" @s7 | "&#x2019;"i @s8 | "&rsquo;" @s7;

  # PTB and LaTeX convention of double back quotes
  # unicode U+201C left double quotation mark
  # unicode U+201F double high-reversed-9 quotation mark
  # "has same semantic as U+201C, but differs in appearance"
  # unicode U+275d heavy double turned comma quotation mark ornament
  unicode_201c = 0xe2 0x80 0x9c | "&#8220;" | "&#x201c;"i | "&ldquo;";
  unicode_201f = 0xe2 0x80 0x9f | "&#8223;" | "&#x201f"i;
  unicode_275d = 0xe2 0x9d 0x9d | "&#10077;" | "&#x275d;"i;

  # Fairfax also uses doubled single quotes (unicode U+2018)
  open_double_quote = "``" | unicode_201c | unicode_201f | unicode_275d | unicode_2018 unicode_2018;

  # unicode U+201D right double quotation mark
  # unicode U+275e heavy double comma quotation mark ornament
  unicode_201d = 0xe2 0x80 0x9d | "&#8221;" | "&#x201d;"i | "&rdquo;";
  unicode_275e = 0xe2 0x9d 0x9e | "&#10078;" | "&#x275e;"i;

  # Fairfax also uses doubled single quotes (unicode U+2019)
  close_double_quote = unicode_201d | unicode_275e | unicode_2019 unicode_2019;
}%%
