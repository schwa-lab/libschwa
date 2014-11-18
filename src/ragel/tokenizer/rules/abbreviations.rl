// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  # Things that look like acronyms.
  acronym1 = alpha ( '.' alpha )+ '.' ;
  acronym2 = upper /[bcdfghj-np-tvxz]+\./ ;
  acronym3 = upper ( upper | '&' )* upper+ ;
  initial = /[A-Z]\./ ;
  acronym = acronym1 | acronym2 | acronym3 | initial;

  # Date abbreviations.
  abbreviation_month =
      'Jan'i
    | 'Feb'i
    | 'Mar'i
    | 'Apr'i
    | 'Jun'i
    | 'Jul'i
    | 'Aug'i
    | 'Sep'i | 'Sept'i
    | 'Oct'i
    | 'Nov'i
    | 'Dec'i
    ;
  abbreviation_day =
      'Mon'i
    | 'Tue'i
    | 'Wed'i
    | 'Thu'i | 'Thurs'i
    | 'Fri'i
    | 'Sat'i
    | 'Sun'i
    ;
  abbreviation_date = ( abbreviation_month | abbreviation_day ) '.' ;

  # Personal titles.
  title_prefix =
      'Amb'i
    | 'Adm'i
    | 'Atty'i
    | 'Br'i
    | 'Brig'i
    | 'Capt'i
    | 'Cdr'i
    | 'Cmdr'i
    | 'Col'i
    | 'Cpl'i
    | 'Crpl'i
    | 'Dr'i
    | 'Drs'i
    | 'Ed'i
    | 'Fr'i
    | 'Gen'i
    | 'Gov'i
    | 'Hon'i
    | 'Lt'i
    | 'Maj'i
    | 'Md'i
    | 'Messrs'i
    | 'Mr'i
    | 'Mrs'i
    | 'Ms'i
    | 'Msgr'i
    | 'Mssr'i
    | 'Mssrs'i
    | 'Ofc'i
    | 'Pres'i
    | 'Prof'i
    | 'Pvt'i
    | 'Rep'i
    | 'Rev'i
    | 'Rt'i
    | 'Rtd'i
    | 'Sec'i
    | 'Sen'i
    | 'Sgt'i
    | 'Sr'i
    | 'Supt'i
    | 'Treas'i
    ;
  title_suffix =
      'Bros'i
    | 'Esq'i
    | 'Hon'i
    | 'Jnr'i
    | 'Jr'i
    | 'PhD'i | 'Ph.D'i
    | 'Ret'i
    | 'Snr'i
    | 'Sr'i
    ;
  title = ( title_prefix | title_suffix ) '.' ;

  # https://en.wikipedia.org/wiki/List_of_U.S._state_abbreviations
  us_state_1 =
      'Ala'i
    | 'Alas'i
    | 'Ariz'i | 'Az'i
    | 'Ark'i
    | 'Calif'i | 'Ca'i | 'Cal'i
    | 'Colo'i | 'Col'i
    | 'Conn'i | 'Ct'i
    | 'Del'i | 'De'i
    | 'Fla'i | 'Fl'i | 'Flor'i
    | 'Ga'i
    | 'Id'i | 'Ida'i
    | 'Ill'i | 'Il'i | 'Ills'i
    | 'Ind'i | 'In'i
    | 'Ia'i | 'Ioa'i
    | 'Kans'i | 'Kan'i | 'Ka'i | 'Ks'i
    | 'Ky'i | 'Ken'i | 'Kent'i
    | 'La'i
    | 'Me'i
    | 'Md'i
    | 'Mass'i
    | 'Mich'i
    | 'Minn'i | 'Mn'i
    | 'Miss'i
    | 'Mo'i
    | 'Mont'i
    | 'Nebr'i | 'Neb'i
    | 'Nev'i | 'Nv'i
    | 'Mex'i
    | 'Car'i
    | 'Dak'i
    | 'O'i | 'Oh'i
    | 'Okla'i | 'Ok'i
    | 'Oreg'i | 'Ore'i | 'Or'i
    | 'Pa'i | 'Penn'i | 'Penna'i
    | 'Tenn'i
    | 'Tex'i | 'Tx'i
    | 'Ut'i
    | 'Vt'i
    | 'Va'i | 'Virg'i
    | 'Wash'i | 'Wa'i | 'Wn'i
    | 'Va'i | 'Virg'i
    | 'Wis'i | 'Wi'i | 'Wisc'i
    | 'Wyo'i | 'Wy'i
    ;
  us_state_2 = 'N' | 'S' | 'W' ;
  canada_state =
      'Alta'i
    | 'Man'i
    | 'Nfld'i
    | 'Nun'i | 'Nvt'i
    | 'Ont'i
    | 'Que'i
    | 'Sask'i
    | 'Yuk'i
    ;
  administrative_division = 'Prov'i ;
  abbreviation_state = ( us_state_1 | us_state_2 '.' us_state_1 | canada_state | administrative_division ) '.' ;

  # https://www.usps.com/send/official-abbreviations.htm#2
  street_suffix =
      'Aly'i
    | 'Anx'i
    | 'Arc'i
    | 'Av'i | 'Ave'i | 'Avn'i
    | 'Bch'i
    | 'Bg'i | 'Bgs'i
    | 'Blf'i | 'Blfs'i
    | 'Blvd'i | 'Boul'i | 'Boulv'i
    | 'Bnd'i
    | 'Bot'i | 'Btm'i
    | 'Br'i
    | 'Brg'i
    | 'Brk'i
    | 'Byp'i
    | 'Byu'i
    | 'Cct'i
    | 'Cir'i
    | 'Cmn'i
    | 'Cp'i
    | 'Cpe'i
    | 'Cr'i
    | 'Crcl'i
    | 'Crk'i
    | 'Cres'i
    | 'Cswy'i
    | 'Ct'i
    | 'Ctr'i
    | 'Cts'i
    | 'Cyn'i
    | 'Dr'i
    | 'Drv'i
    | 'Hbr'i
    | 'Hgts'i
    | 'Hts'i
    | 'Hwy'i
    | 'Is'i
    | 'Isl'i
    | 'Jctn'i
    | 'La'i
    | 'Lks'i
    | 'Ln'i
    | 'Lndg'i
    | 'Mnr'i
    | 'Mdw'i
    | 'Mdws'i
    | 'Mls'i
    | 'Mt'i
    | 'Mtn'i
    | 'Mnt'i
    | 'Mntn'i
    | 'Mntns'i
    | 'Pk'i
    | 'Pkwy'i
    | 'Pl'i
    | 'Pln'i
    | 'Plns'i
    | 'Plz'i
    | 'Rd'i
    | 'Rds'i
    | 'St'i
    | 'Sta'i
    | 'Stn'i
    | 'Str'i
    | 'Te'i
    | 'Tr'i
    | 'Trk'i
    | 'Trl'i
    | 'Tpk'i
    | 'Tpke'i
    | 'Trpk'i
    | 'Vl'i
    | 'Vlg'i
    | 'Vly'i
    | 'Via'i
    | 'Vdct'i
    | 'Wl'i
    | 'Wls'i
    ;
  building_suffix =
      'Apt'i
    | 'Bldg'i
    | 'Bsmt'i
    | 'Dept'i
    | 'Fl'i
    | 'Frnt'i
    | 'Hngr'i
    | 'Lbby'i
    | 'Lowr'i
    | 'Lvl'i
    | 'Ofc'i
    | 'Ph'i
    | 'Rm'i
    | 'Spc'i
    | 'Ste'i
    | 'Trlr'i
    | 'Uppr'i
    ;
  address_suffix = ( street_suffix | building_suffix ) '.' ;

  # Business/organisation names.
  abbreviation_corp =
      'Bancorp'i
    | 'Bhd'i  # Sdn. Bhd. is Malay for Inc.
    | 'Cie'i  # Abbrevation for company in French.
    | 'Co'i | 'Cos'i
    | 'Corp'i
    | 'Coop'i
    | 'Co-op'i
    | 'Group'i
    | 'Inc'i
    | 'Llp'i
    | 'Ltd'i
    | 'Plc'i
    | 'Pty'i
    | 'Sdn'i  # Sdn. Bhd. is Malay for Inc.
    ;
  abbreviation_business =
      'Admin'i
    | 'Dept'i
    | 'Ind'i
    | 'Mkts'i
    | 'Est'i
    | 'Manufac'i
    | 'Svcs'i
    ;
  abbreviation_group =
      'Assn'i
    | 'Assoc'i | 'Assocs'i
    | 'Educ'i
    | 'Dist'i
    | 'Twp'i
    | 'Govt'i
    | 'Int'i | 'Intl'i
    | 'Nati' | 'Natl'i
    | 'Soc'
    | 'Uni'i | 'Univ'i
    ;
  abbreviation_org = ( abbreviation_corp | abbreviation_business | abbreviation_group ) '.' ;

  # problem cases: 'cent' (per cent appears often at the end of a sentence)
  abbreviation = (
        'accd'i
      | 'al'i  # et al.
      | 'amort'i
      | 'approx'i
      | 'avg'i
      | 'cert'i
      | 'cf'i
      | 'cont'i
      | 'devel'i
      | 'disc'i
      | 'discont'i
      | 'ed'i
      | 'eds'i
      | 'eg'i | 'e.g'i
      | 'equiv'i
      | 'etc'i
      | 'excl'i
      | 'expell'i
      | 'fin'i
      | 'ie'i | 'i.e'i
      | 'imp'i
      | 'incl'i
      | 'ins'i
      | 'inv'i
      | 'invest'i
      | 'no'i
      | 'ord'i
      | 'pell'i
      | 'repr'i
      | 'rev'i
      | 'trans'i
      | 'viz'i
      | 'vol'i
      | 'vols'i
      | 'vs'i | 'v.s'i
   ) '.' ;

}%%
