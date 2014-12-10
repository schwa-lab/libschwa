// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  # Things that look like acronyms.
  acronym1 = alpha ( '.' alpha )+ '.' ;
  acronym2 = upper /[bcdfghj-np-tvxz]+\./ ;
  acronym3 = upper+ '&' upper+ ;
  initial = /[A-HJ-Z]\./ ;

  # Date abbreviations.
  abbreviation_month =
      'Jan' | 'JAN'
    | 'Feb' | 'FEB'
    | 'Mar' | 'MAR'
    | 'Apr' | 'APR'
    | 'Jun' | 'JUN'
    | 'Jul' | 'JUL'
    | 'Aug' | 'AUG'
    | 'Sep' | 'Sept' | 'SEP' | 'SEPT'
    | 'Oct' | 'OCT'
    | 'Nov' | 'NOV'
    | 'Dec' | 'DEC'
    ;
  abbreviation_day =
      'Mon' | 'MON'
    | 'Tue' | 'TUE'
    | 'Wed' | 'WED'
    | 'Thu' | 'Thurs' | 'THU' | 'THURS'
    | 'Fri' | 'FRI'
    | 'Sat' | 'SAT'
    | 'Sun' | 'SUN'
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
      'Ala' | 'ALA'
    | 'Alas' | 'ALAS'
    | 'Ariz' | 'Az' | 'ARIZ' | 'AZ'
    | 'Ark' | 'ARK'
    | 'Calif' | 'Ca' | 'Cal' | 'CALIF' | 'CA' | 'CAL'
    | 'Colo' | 'Col' | 'COLO' | 'COL'
    | 'Conn' | 'Ct' | 'CONN' | 'CT'
    | 'Del' | 'De' | 'DEL' | 'DE'
    | 'Fla' | 'Fl' | 'Flor' | 'FLA' | 'FL' | 'FLOR'
    | 'Ga' | 'GA'
    | 'Id' | 'Ida' | 'ID' | 'IDA'
    | 'Ill' | 'Il' | 'Ills' | 'ILL' | 'IL' | 'ILLS'
    | 'Ind' | 'In' | 'IND' | 'IN'
    | 'Ia' | 'Ioa' | 'IA' | 'IOA'
    | 'Kans' | 'Kan' | 'Ka' | 'Ks' | 'KANS' | 'KAN' | 'KA' | 'KS'
    | 'Ky' | 'Ken' | 'Kent' | 'KY' | 'KEN' | 'KENT'
    | 'La' | 'LA'
    | 'Me' | 'ME'
    | 'Md' | 'MD'
    | 'Mass' | 'MASS'
    | 'Mich' | 'MICH'
    | 'Minn' | 'Mn' | 'MINN' | 'MN'
    | 'Miss' | 'MISS'
    | 'Mo' | 'MO'
    | 'Mont' | 'MONT'
    | 'Nebr' | 'Neb' | 'NEBR' | 'NEB'
    | 'Nev' | 'Nv' | 'NEV' | 'NV'
    | 'O' | 'Oh' | 'OH'
    | 'Okla' | 'Ok' | 'OKLA' | 'OK'
    | 'Oreg' | 'Ore' | 'Or' | 'OREG' | 'ORE' | 'OR'
    | 'Pa' | 'Penn' | 'Penna' | 'PA' | 'PENN' | 'PENNA'
    | 'Tenn' | 'TENN'
    | 'Tex' | 'Tx' | 'TEX' | 'TX'
    | 'Ut' | 'UT'
    | 'Vt' | 'VT'
    | 'Va' | 'Virg' | 'VA' | 'VIRG'
    | 'Wash' | 'Wa' | 'Wn' | 'WASH' | 'WA' | 'WN'
    | 'Wis' | 'Wi' | 'Wisc' | 'WIS' | 'WI' | 'WISC'
    | 'Wyo' | 'Wy' | 'WYO' | 'WY'
    ;
  us_state_2 = 'N' | 'S' | 'W' ;
  canada_state =
      'Alta' | 'ALTA'
    | 'Man' | 'MAN'
    | 'Nfld' | 'NFLD'
    | 'Nun' | 'Nvt' | 'NUN' | 'NVT'
    | 'Ont' | 'ONT'
    | 'Que' | 'QUE'
    | 'Sask' | 'SASK'
    | 'Yuk' | 'YUK'
    ;
  administrative_division = 'Prov'i ;
  abbreviation_state = (
      us_state_1
    | us_state_2 '.' us_state_1
    | canada_state
    | administrative_division
  ) '.' ;

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
    | 'Is' | 'IS'
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
      'Bancorp' | 'BANCORP'
    | 'Bhd' | 'BHD'  # Sdn. Bhd. is Malay for Inc.
    | 'Cie' | 'CIE'  # Abbrevation for company in French.
    | 'Co' | 'Cos' | 'CO' | 'COS'
    | 'Corp' | 'CORP'
    | 'Coop' | 'COOP'
    | 'Co-op' | 'CO-OP'
    | 'Group' | 'GROUP'
    | 'Inc' | 'INC'
    | 'Llp' | 'LLP'
    | 'Ltd' | 'LTD'
    | 'Plc' | 'PLC'
    | 'Pte' | 'PTE'  # Pte. Ltd. is Malay.
    | 'Pty' | 'PTY'
    | 'Sdn' | 'SDN'  # Sdn. Bhd. is Malay for Inc.
    ;
  abbreviation_business =
      'Admin' | 'ADMIN'
    | 'Dept' | 'DEPT'
    | 'Ind' | 'IND'
    | 'Mkts' | 'MKTS'
    | 'Est' | 'EST'
    | 'Manufac' | 'MANUFAC'
    | 'Svcs' | 'SVCS'
    ;
  abbreviation_group =
      'Assn' | 'ASSN'
    | 'Assoc' | 'Assocs' | 'ASSOC' | 'ASSOCS'
    | 'Educ' | 'EDUC'
    | 'Dist' | 'DIST'
    | 'Govt' | 'GOVT'
    | 'Int' | 'Intl' | 'INT' | 'INTL'
    | 'Nat' | 'Natl' | 'NAT' | 'NATL'
    | 'Ntl' | 'NTL'
    | 'Soc' | 'SOC'
    | 'Twp' | 'TWP'
    | 'Uni' | 'Univ' | 'UNI' | 'UNIV'
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
      | 'cont'i
      | 'devel'i
      | 'disc'i
      | 'discont'i
      | 'ed'i
      | 'eds'i
      | 'equiv'i
      | 'etc'i
      | 'excl'i
      | 'expell'i
      | 'fin'i
      | 'imp'i
      | 'incl'i
      | 'ins'i
      | 'inv'i
      | 'invest'i
      | 'ord'i
      | 'pell'i
      | 'repr'i
      | 'rev'i
      | 'trans'i
      | 'viz'i
      | 'vol'i
      | 'vols'i
   ) '.' ;
   non_eos_abbreviation = (
        'cf'i
      | 'eg'i | 'e.g'i
      | 'ie'i | 'i.e'i
      | 'vs'i | 'v.s'i
   ) '.' ;


  abbreviation_bigram =
      ('No' | 'NO') '.' %b1 unicode_space+ %b2 numbers
    | ('N' | 'S') '.' %b1 unicode_space+ %b2 ('Car' | 'CAR' | 'Dak' | 'DAK') '.'
    ;

}%%
