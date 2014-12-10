// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  # Things that look like acronyms.
  acronym1 = upper ( '.' alpha )+ '.' ;
  acronym2 = upper /[bcdfghj-np-tvxz]+\./ ;
  acronym3 = upper+ '&' upper+ ;
  initial1 = /[A-HJ-Z]\./ ;
  initial2 = upper '.' upper '.' ;

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
  title_prefix = (
        'Amb' | 'AMB'
      | 'Adm' | 'ADM'
      | 'Atty' | 'ATTY'
      | 'Br' | 'BR'
      | 'Brig' | 'BRIG'
      | 'Capt' | 'CAPT'
      | 'Cdr' | 'CDR'
      | 'Cmdr' | 'CMDR'
      | 'Col' | 'COL'
      | 'Cpl' | 'CPL'
      | 'Crpl' | 'CRPL'
      | 'Dr' | 'DR'
      | 'Drs' | 'DRS'
      | 'Ed' | 'ED'
      | 'Fr' | 'FR'
      | 'Gen' | 'GEN'
      | 'Gov' | 'GOV'
      | 'Hon' | 'HON'
      | 'Lt' | 'LT'
      | 'Maj' | 'MAJ'
      | 'Md' | 'MD'
      | 'Messrs' | 'MESSRS'
      | 'Mr' | 'MR'
      | 'Mrs' | 'MRS'
      | 'Ms' | 'MS'
      | 'Msgr' | 'MSGR'
      | 'Mssr' | 'MSSR'
      | 'Mssrs' | 'MSSRS'
      | 'Ofc' | 'OFC'
      | 'Pres' | 'PRES'
      | 'Prof' | 'PROF'
      | 'Pvt' | 'PVT'
      | 'Rep' | 'REP'
      | 'Rev' | 'REV'
      | 'Rt' | 'RT'
      | 'Rtd' | 'RTD'
      | 'Sec' | 'SEC'
      | 'Sen' | 'SEN'
      | 'Sgt' | 'SGT'
      | 'Sr' | 'SR'
      | 'St' | 'ST'
      | 'Supt' | 'SUPT'
      | 'Treas' | 'TREAS'
    ) '.'
    ;
  title_suffix = (
        'Bros' | 'BROS'
      | 'Esq' | 'ESQ'
      | 'Hon' | 'HON'
      | 'Jnr' | 'JNR'
      | 'Jr' | 'JR'
      | 'PhD' | 'Ph.D' | 'PHD' | 'PH.D'
      | 'Ret' | 'RET'
      | 'Snr' | 'SNR'
      | 'Sr' | 'SR'
    ) '.'
    ;

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

  # http://pe.usps.gov/text/pub28/28apc_002.htm
  street_suffix =
      'Aly' | 'ALY'
    | 'Anx' | 'ANX'
    | 'Arc' | 'ARC'
    | 'Av' | 'AV' | 'Ave' | 'AVE' | 'Avn' | 'AVN'
    | 'Bch' | 'BCH'
    | 'Bg' | 'BG' | 'Bgs' | 'BGS'
    | 'Blf' | 'BLF' | 'Blfs' | 'BLFS'
    | 'Blvd' | 'BLVD' | 'Boul' | 'BOUL' | 'Boulv' | 'BOULV'
    | 'Bnd' | 'BND'
    | 'Bot' | 'BOT' | 'Btm' | 'BTM'
    | 'Br' | 'BR'
    | 'Brg' | 'BRG'
    | 'Brk' | 'BRK'
    | 'Byp' | 'BYP'
    | 'Byu' | 'BYU'
    | 'Cct' | 'CCT'
    | 'Cir' | 'CIR'
    | 'Cmn' | 'CMN'
    | 'Cp' | 'CP'
    | 'Cpe' | 'CPE'
    | 'Cr' | 'CR'
    | 'Crcl' | 'CRCL'
    | 'Crk' | 'CRK'
    | 'Cres' | 'CRES'
    | 'Cswy' | 'CSWY'
    | 'Ct' | 'CT'
    | 'Ctr' | 'CTR'
    | 'Cts' | 'CTS'
    | 'Cyn' | 'CYN'
    | 'Dr' | 'DR'
    | 'Drv' | 'DRV'
    | 'Hbr' | 'HBR'
    | 'Hgts' | 'HGTS'
    | 'Hts' | 'HTS'
    | 'Hwy' | 'HWY'
    | 'Is' | 'IS'
    | 'Isl' | 'ISL'
    | 'Jctn' | 'JCTN'
    | 'La' | 'LA'
    | 'Lks' | 'LKS'
    | 'Ln' | 'LN'
    | 'Lndg' | 'LNDG'
    | 'Mnr' | 'MNR'
    | 'Mdw' | 'MDW'
    | 'Mdws' | 'MDWS'
    | 'Mls' | 'MLS'
    | 'Mt' | 'MT'
    | 'Mtn' | 'MTN'
    | 'Mnt' | 'MNT'
    | 'Mntn' | 'MNTN'
    | 'Mntns' | 'MNTNS'
    | 'Pk' | 'PK'
    | 'Pkwy' | 'PKWY'
    | 'Pl' | 'PL'
    | 'Pln' | 'PLN'
    | 'Plns' | 'PLNS'
    | 'Plz' | 'PLZ'
    | 'Rd' | 'RD'
    | 'Rds' | 'RDS'
    | 'Sta' | 'STA'
    | 'Stn' | 'STN'
    | 'Str' | 'STR'
    | 'Te' | 'TE'
    | 'Tr' | 'TR'
    | 'Trk' | 'TRK'
    | 'Trl' | 'TRL'
    | 'Tpk' | 'TPK'
    | 'Tpke' | 'TPKE'
    | 'Trpk' | 'TRPK'
    | 'Vl' | 'VL'
    | 'Vlg' | 'VLG'
    | 'Vly' | 'VLY'
    | 'Via' | 'VIA'
    | 'Vdct' | 'VDCT'
    | 'Wl' | 'WL'
    | 'Wls' | 'WLS'
    ;
  building_suffix =
      'Apt' | 'APT'
    | 'Bldg' | 'BLDG'
    | 'Bsmt' | 'BSMT'
    | 'Dept' | 'DEPT'
    | 'Fl' | 'FL'
    | 'Frnt' | 'FRNT'
    | 'Hngr' | 'HNGR'
    | 'Lbby' | 'LBBY'
    | 'Lowr' | 'LOWR'
    | 'Lvl' | 'LVL'
    | 'Ofc' | 'OFC'
    | 'Ph' | 'PH'
    | 'Rm' | 'RM'
    | 'Spc' | 'SPC'
    | 'Ste' | 'STE'
    | 'Trlr' | 'TRLR'
    | 'Uppr' | 'UPPR'
    ;
  address_suffix = ( street_suffix | building_suffix ) '.' ;

  # Business/organisation names.
  abbreviation_corp =
      'Bancorp' | 'BANCORP'
    | 'Bhd' | 'BHD'  # Sdn. Bhd. is Malay for Inc.
    | 'Co' | 'Cos' | 'CO' | 'COS'
    | 'Corp' | 'CORP'
    | 'Coop' | 'COOP'
    | 'Co-op' | 'CO-OP'
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
    | 'Manufac' | 'MANUFAC' | 'Mfg' | 'MFG'
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
        'accd' | 'ACCD'
      | 'al' | 'AL'  # et al.
      | 'amort' | 'AMORT'
      | 'approx' | 'APPROX'
      | 'avg' | 'AVG'
      | 'cert' | 'CERT'
      | 'cont' | 'CONT'
      | 'devel' | 'DEVEL'
      | 'disc' | 'DISC'
      | 'discont' | 'DISCONT'
      | 'ed' | 'ED'
      | 'eds' | 'EDS'
      | 'equiv' | 'EQUIV'
      | 'etc' | 'ETC'
      | 'excl' | 'EXCL'
      | 'expell' | 'EXPELL'
      | 'fin' | 'FIN'
      | 'imp' | 'IMP'
      | 'incl' | 'INCL'
      | 'ins' | 'INS'
      | 'inv' | 'INV'
      | 'invest' | 'INVEST'
      | 'ord' | 'ORD'
      | 'pell' | 'PELL'
      | 'repr' | 'REPR'
      | 'rev' | 'REV'
      | 'trans' | 'TRANS'
      | 'viz' | 'VIZ'
      | 'vol' | 'VOL'
      | 'vols' | 'VOLS'
   ) '.' ;
   non_eos_abbreviation = (
        'cf' | 'CF'
      | 'Cie' | 'CIE'  # Abbrevation for company in French.
      | 'eg' | 'EG' | 'e.g' | 'E.G'
      | 'ie' | 'IE' | 'i.e' | 'I.E'
      | 'v' | 'V' | 'vs' | 'VS' | 'v.s' | 'V.S'
   ) '.' ;


  abbreviation_bigram =
      ('No' | 'NO') '.' %b1 unicode_space+ %b2 numbers
    | ('N' | 'S') '.' %b1 unicode_space+ %b2 ('Car' | 'CAR' | 'Dak' | 'DAK') '.'
    ;

}%%
