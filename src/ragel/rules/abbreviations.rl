/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  acron1 = alpha ("." alpha) ".";
  acron2 = upper /[bcdfghj-np-tvxz]+\./;
  acron3 = upper (upper | "&")* upper+;

  initial = /[A-Z]\./;

  acron = acron1 | acron2 | acron3 | initial;

  month_abbrev = ("Jan"|"Feb"|"Mar"|"Apr"|"Jun"|"Jul"|"Aug"|"Sep"|"Sept"|"Oct"|"Nov"|"Dec") ".";
  day_abbrev = ("Mon"|"Tue"|"Wed"|"Thurs"|"Fri"|"Sat"|"Sun") ".";

  date_abbrev = month_abbrev | day_abbrev;

  title0 = ("Mr"|"Mrs"|"Ms"|"Dr"|"Drs"|"Fr"|"Sr"|"Br"|"Rev"|"Prof"|"Mssr"|"Mssrs") ".";
  title1 = ("Sr"|"Snr"|"Jr"|"Jnr"|"Hon"|"Bros"|"Esq") ".";
  title2 = ("Rep"|"Sen"|"Gov"|"Pres"|"Adm"|"Capt"|"Crpl"|"Gen"|"Lt"|"Maj"|"Sgt") ".";
  title3 = ("Brig"|"Cmdr"|"Cdr"|"Sec"|"Rtd"|"Rt") ".";
  title4 = ("Ed"|"Md") ".";

  title = title0 | title1 | title2 | title3 | title4;

  usstate0 = ("Ala"|"Ariz"|"Ark"|"Ca"|"Calif"|"Colo"|"Conn"|"Del"|"Fla"|"Ga"|"Ill") ".";
  usstate1 = ("Ind"|"Kan"|"Ky"|"La"|"Mass"|"Md"|"Mich"|"Minn"|"Miss"|"Mo"|"Mont") ".";
  usstate2 = ("Neb"|"Nev"|"Okla"|"Ore"|"Pa"|"Penn"|"Penna"|"Tenn"|"Tex"|"Va") ".";
  usstate3 = ("Vt"|"Wash"|"Wis"|"Wisc"|"Wyo") ".";
  usstate4 = (/[SN]\./ | /[SN]o\./) space+ ("Carol."|"Dak.") | "W." space+ "Va.";

  canada1 = ("Manit"|"Ont"|"Que") ".";
  places1 = ("Prov") ".";

  state = usstate0 | usstate1 | usstate2 | usstate3 | usstate4 | canada1 | places1;

  addr0 = ("Aly"|"Av"|"Ave"|"Bch"|"Blvd"|"Boul") ".";
  addr1 = ("Cct"|"Cmn"|"Ct"|"Cts"|"Cr"|"Crk"|"Cres"|"Dr"|"Drv") ".";
  addr2 = ("Hbr"|"Hgts"|"Hts"|"Hwy"|"Is"|"Jctn"|"La"|"Lks"|"Ln"|"Lndg") ".";
  addr3 = ("Mnr"|"Mdw"|"Mdws"|"Mls"|"Mt"|"Mtn"|"Mnt"|"Mntn"|"Mntns") ".";
  addr4 = ("Pk"|"Pkwy"|"Pl"|"Pln"|"Plns"|"Plz"|"Rd"|"Rds") ".";
  addr5 = ("St"|"Sta"|"Stn"|"Str"|"Te"|"Tr"|"Trk"|"Trl"|"Tpk"|"Tpke"|"Trpk") ".";
  addr6 = ("Vl"|"Vlg"|"Vly"|"Via"|"Vdct"|"Wl"|"Wls") ".";

  building = ("Apt"|"Bsmt"|"Bldg"|"Lbby"|"Lvl"|"Rm"|"Fl") ".";

  addr = addr0 | addr1 | addr2 | addr3 | addr4 | addr5 | addr6 | building;

  corp = ("Co"|"Corp"|"Coop"|"Co-op"|"Inc"|"Ltd"|"Pty"|"Plc") ".";
  biz0 = ("Admin"|"Dept"|"Ind"|"Mkts"|"Est"|"est") ".";
  biz1 = ("Manufac"|"Svcs") ".";
  group = ("Assn"|"Assoc"|"Assocs"|"Educ"|"Dist"|"Twp"|"Govt"|"Int"|"Intl"|"Uni"|"Univ") ".";

  org = corp | biz0 | biz1 | group;

  # problem cases: "cent" (per cent appears often at the end of a sentence)

  abbrev0 = ("ed"|"eg"|"e.g"|"etc"|"cf"|"i.e"|"ie"|"viz"|"vol") ".";
  abbrev1 = ("eds"|"repr"|"trans"|"vols"|"rev"|"pell") ".";
  abbrev2 = ("accd"|"amort"|"approx"|"avg"|"cert"|"cont"|"devel"|"disc"|"discont"|"equiv") ".";
  abbrev3 = ("excl"|"expell"|"fin"|"imp"|"incl"|"ins"|"inv"|"invest"|"ord") ".";

  abbrev = abbrev0 | abbrev1 | abbrev2 | abbrev3;

}%%
