// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  cont_misc_lc = "cont'd" @{n1="continued";} | "ma'am" @{n1="madame";} | "o'clock" | "w'end" @{n1="weekend";} | "gov't" @{n1="government";} | "int'l" @{n1="international";} | "qur'an" | "y'all" | "nat'l" @{n1="national";} | "req'd" @{n1="required";} | "c'mon" | "ya'll" | "o'er" @{n1="over";}|"y'know"|"add'l"|"dep't"|"rock'n'roll"|"'tis"|"readin'";
  cont_misc_tc = "Cont'd"|"Ma'am"|"O'clock"|"O'Clock"|"W'end"|"Gov't"|"Int'l"|"Qur'an"|"Y'all"|"Hawai'i"|"Nat'l"|"Req'd"|"C'mon"|"Ya'll"|"O'er"|"Y'know"|"Add'l"|"Dep't";

  cont_d = ("'d" @s2 | "&#39;d" @s6 | "&apos;d" @s7 | 0xe2 0x80 0x99 "d" @s4 | "&#8217;d" @s8 | "&#x2019;"i "d" @s9 | "&rsquo;d" @s8 | 0x92 "d" @s2 | 0x19 "d" @s2) @{n2="'d";};
  cont_m = ("'m" @s2 | "&#39;m" @s6 | "&apos;m" @s7 | 0xe2 0x80 0x99 "m" @s4 | "&#8217;m" @s8 | "&#x2019;"i "m" @s9 | "&rsquo;m" @s8 | 0x92 "m" @s2 | 0x19 "m" @s2) @{n2="'m";};

  cont_D = ("'D" @s2 | "&#39;D" @s6 | "&apos;D" @s7 | 0xe2 0x80 0x99 "D" @s4 | "&#8217;D" @s8 | "&#x2019;"i "D" @s9 | "&rsquo;D" @s8 | 0x92 "D" @s2 | 0x19 "D" @s2) @{n2="'D";};
  cont_M = ("'M" @s2 | "&#39;M" @s6 | "&apos;M" @s7 | 0xe2 0x80 0x99 "M" @s4 | "&#8217;M" @s8 | "&#x2019;"i "M" @s9 | "&rsquo;M" @s8 | 0x92 "M" @s2 | 0x19 "M" @s2) @{n2="'M";};

  cont_ll = ("'ll" @s3 | "&#39;ll" @s7 | "&apos;ll" @s8 | 0xe2 0x80 0x99 "ll" @s5 | "&#8217;ll" @s9 | "&#x2019;"i "ll" @s10 | "&rsquo;ll" @s9 | 0x92 "ll" @s2 | 0x19 "ll" @s2) @{n2="'ll";};
  cont_re = ("'re" @s3 | "&#39;re" @s7 | "&apos;re" @s8 | 0xe2 0x80 0x99 "re" @s5 | "&#8217;re" @s9 | "&#x2019;"i "re" @s10 | "&rsquo;re" @s9 | 0x92 "re" @s2 | 0x19 "re" @s2) @{n2="'re";};
  cont_ve = ("'ve" @s3 | "&#39;ve" @s7 | "&apos;ve" @s8 | 0xe2 0x80 0x99 "ve" @s5 | "&#8217;ve" @s9 | "&#x2019;"i "ve" @s10 | "&rsquo;ve" @s9 | 0x92 "ve" @s2 | 0x19 "ve" @s2) @{n2="'ve";};

  cont_LL = ("'LL" @s3 | "&#39;LL" @s7 | "&apos;LL" @s8 | 0xe2 0x80 0x99 "LL" @s5 | "&#8217;LL" @s9 | "&#x2019;"i "LL" @s10 | "&rsquo;LL" @s9 | 0x92 "LL" @s2 | 0x19 "LL" @s2) @{n2="'LL";};
  cont_RE = ("'RE" @s3 | "&#39;RE" @s7 | "&apos;RE" @s8 | 0xe2 0x80 0x99 "RE" @s5 | "&#8217;RE" @s9 | "&#x2019;"i "RE" @s10 | "&rsquo;RE" @s9 | 0x92 "RE" @s2 | 0x19 "RE" @s2) @{n2="'RE";};
  cont_VE = ("'VE" @s3 | "&#39;VE" @s7 | "&apos;VE" @s8 | 0xe2 0x80 0x99 "VE" @s5 | "&#8217;VE" @s9 | "&#x2019;"i "VE" @s10 | "&rsquo;VE" @s9 | 0x92 "VE" @s2 | 0x19 "VE" @s2) @{n2="'VE";};

  cont_suffix_lc = cont_d | cont_m | cont_ll | cont_re | cont_ve;
  cont_suffix_uc = cont_D | cont_M | cont_LL | cont_RE | cont_VE;
  cont_suffix = cont_suffix_lc | cont_suffix_uc;

  cont_misc = cont_misc_lc | cont_misc_tc;

  vb_aux_lc = "ai"|"are"|"ca"|"could"|"did"|"do"|"does"|"had"|"has"|"have"|"is"|"should"|"was"|"were"|"wo"|"would";
  vb_aux_tc = "Ai"|"Are"|"Ca"|"Could"|"Did"|"Do"|"Does"|"Had"|"Has"|"Have"|"Is"|"Should"|"Was"|"Were"|"Wo"|"Would";
  vb_aux_uc = "AI"|"ARE"|"CA"|"COULD"|"DID"|"DO"|"DOES"|"HAD"|"HAS"|"HAVE"|"IS"|"SHOULD"|"WAS"|"WERE"|"WO"|"WOULD";

  cont_not = ("n't" @s3 | "n&#39;t" @s7 | "n&apos;t" @s8 | "n" 0xe2 0x80 0x99 "t" @s5 | "n&#8217;t" @s9 | "n&#x2019;"i "t" @s10 | "n&rsquo;t" @s9 | "n" 0x92 "t" @s3 | "n" 0x19 "t" @s3) @{n2="n't";};
  cont_NOT = ("N'T" @s3 | "N&#39;T" @s7 | "N&apos;T" @s8 | "N" 0xe2 0x80 0x99 "T" @s5 | "N&#8217;T" @s9 | "N&#x2019;"i "T" @s10 | "N&rsquo;T" @s9 | "N" 0x92 "T" @s3 | "n" 0x19 "t" @s3) @{n2="N'T";};

#  cont_NT = ("'X" @s3 | "&#39;X" @s7 | "&apos;X" @s8 | 0xe2 0x80 0x99 "X" @s5 | "&#8217;X" @s9 | "&#x2019;"i "X" @s10 | "&rsquo;X" @s9) @{n2="'X";};

  neg_lc_tc = (vb_aux_lc | vb_aux_tc) cont_not;
  neg_uc = vb_aux_uc cont_NOT;
  neg = neg_lc_tc | neg_uc;

  error_not = ("nt" @s2 | "n?t" @s3) @{n2="n't";};
  error_NOT = ("NT" @s2 | "N?T" @s3) @{n2="N'T";};

  neg_lc_tc_error = (vb_aux_lc | vb_aux_tc) error_not;
  neg_uc_error = vb_aux_uc error_NOT;

  neg_error = neg_lc_tc_error | neg_uc_error;

}%%
