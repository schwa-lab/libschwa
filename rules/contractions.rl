/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  cont_misc_lc = "cont'd" @{n1="continued";} | "ma'am" @{n1="madame";} | "o'clock" | "w'end" @{n1="weekend";} | "gov't" @{n1="government";} | "int'l" @{n1="international";} | "qur'an" | "y'all" | "nat'l" @{n1="national";} | "req'd" @{n1="required";} | "c'mon" | "ya'll" | "o'er" @{n1="over";}|"y'know"|"add'l"|"dep't"|"rock'n'roll"|"'tis"|"readin'";
  cont_misc_tc = "Cont'd"|"Ma'am"|"O'clock"|"O'Clock"|"W'end"|"Gov't"|"Int'l"|"Qur'an"|"Y'all"|"Hawai'i"|"Nat'l"|"Req'd"|"C'mon"|"Ya'll"|"O'er"|"Y'know"|"Add'l"|"Dep't";

	cont_1_lc = apostrophe_s ("d" @{n2="'d";} | "m" @{n2="'m";}) @s1;
	cont_1_uc = apostrophe_s ("D" @{n2="'D";} | "M" @{n2="'M";}) @s1;

	cont_2_lc = apostrophe_s ("ll" @{n2="'ll";} | "re" @{n2="'re";} | "ve" @{n2="'ve";}) @s2;
	cont_2_uc = apostrophe_s ("LL" @{n2="'LL";} | "RE" @{n2="'RE";} | "VE" @{n2="'VE";}) @s2;

  cont = cont_misc_lc | cont_misc_tc;
  
  vb_aux_lc = "ai"|"are"|"ca"|"could"|"did"|"do"|"does"|"had"|"has"|"have"|"is"|"should"|"was"|"were"|"wo"|"would";
  vb_aux_tc = "Ai"|"Are"|"Ca"|"Could"|"Did"|"Do"|"Does"|"Had"|"Has"|"Have"|"Is"|"Should"|"Was"|"Were"|"Wo"|"Would";
  vb_aux_uc = "AI"|"ARE"|"CA"|"COULD"|"DID"|"DO"|"DOES"|"HAD"|"HAS"|"HAVE"|"IS"|"SHOULD"|"WAS"|"WERE"|"WO"|"WOULD";

  not_lc = "n" apostrophe_s "t" @{n2="n't";} @s2;
  not_uc = "N" apostrophe_s "T" @{n2="N'T";} @s2;

  neg_lc = vb_aux_lc not_lc;
  neg_tc = vb_aux_tc not_lc;
  neg_uc = vb_aux_uc not_uc;

  neg = neg_lc | neg_tc | neg_uc;

  not_lc_error = "nt" @{n2="n't";} @s2;
  not_uc_error = "NT" @{n2="N'T";} @s2;

  neg_lc_error = vb_aux_lc not_lc_error;
  neg_tc_error = vb_aux_tc not_lc_error;
  neg_uc_error = vb_aux_uc not_uc_error;

}%%
