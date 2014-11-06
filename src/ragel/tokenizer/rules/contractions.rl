// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  contractions_quote = single_quote | close_single_quote ;

  contractions_misc =
      contractions_quote 'tis'
    | 'add' contractions_quote 'l'
    | 'Add' contractions_quote 'l'
    | 'c' contractions_quote 'mon'
    | 'C' contractions_quote 'mon'
    | 'cont' contractions_quote 'd' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"continued"); }
    | 'Cont' contractions_quote 'd' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"Continued"); }
    | 'dep' contractions_quote 't'
    | 'Dep' contractions_quote 't'
    | 'gov' contractions_quote 't' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"government"); }
    | 'Gov' contractions_quote 't' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"Government"); }
    | 'Hawai' contractions_quote 'i'
    | 'int' contractions_quote 'l' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"international"); }
    | 'Int' contractions_quote 'l' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"International"); }
    | 'ma' contractions_quote 'am' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"madame"); }
    | 'Ma' contractions_quote 'am' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"Madame"); }
    | 'nat' contractions_quote 'l' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"national"); }
    | 'Nat' contractions_quote 'l' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"National"); }
    | 'o' contractions_quote 'clock'
    | 'O' contractions_quote 'clock'
    | 'O' contractions_quote 'Clock'
    | 'o' contractions_quote 'er' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"over"); }
    | 'O' contractions_quote 'er' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"Over"); }
    | 'qur' contractions_quote 'an'
    | 'Qur' contractions_quote 'an'
    | 'readin' contractions_quote
    | 'req' contractions_quote 'd' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"required"); }
    | 'Req' contractions_quote 'd' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"Required"); }
    | 'rock' contractions_quote 'n' contractions_quote 'roll'
    | 'w' contractions_quote 'end' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"weekend"); }
    | 'W' contractions_quote 'end' @{ _state.n1 = reinterpret_cast<const uint8_t *>(u8"Weekend"); }
    | 'y' contractions_quote 'all'
    | 'Y' contractions_quote 'all'
    | 'y' contractions_quote 'know'
    | 'Y' contractions_quote 'know'
    | 'ya' contractions_quote 'll'
    | 'Ya' contractions_quote 'll'
    ;

  # Fixed-length contraction suffixes.
  contractions_suffix_2 =
      contractions_quote 'd' @s2 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'d"); }
    | contractions_quote 'D' @s2 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'D"); }
    | contractions_quote 'm' @s2 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'m"); }
    | contractions_quote 'M' @s2 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'M"); }
    ;
  contractions_suffix_3 =
      contractions_quote 'll' @s3 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'ll"); }
    | contractions_quote 'LL' @s3 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'LL"); }
    | contractions_quote 're' @s3 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'re"); }
    | contractions_quote 'RE' @s3 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'RE"); }
    | contractions_quote 've' @s3 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'ve"); }
    | contractions_quote 'VE' @s3 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'VE"); }
    ;
  contractions_suffix = contractions_suffix_2 | contractions_suffix_3 ;

  # Auxilary verbs.
  vb_aux_lc =
      'ai'
    | 'are'
    | 'ca'
    | 'could'
    | 'did'
    | 'do'
    | 'does'
    | 'had'
    | 'has'
    | 'have'
    | 'is'
    | 'should'
    | 'was'
    | 'were'
    | 'wo'
    | 'would'
    ;
  vb_aux_tc =
      'Ai'
    | 'Are'
    | 'Ca'
    | 'Could'
    | 'Did'
    | 'Do'
    | 'Does'
    | 'Had'
    | 'Has'
    | 'Have'
    | 'Is'
    | 'Should'
    | 'Was'
    | 'Were'
    | 'Wo'
    | 'Would'
    ;
  vb_aux_uc =
      'AI'
    | 'ARE'
    | 'CA'
    | 'COULD'
    | 'DID'
    | 'DO'
    | 'DOES'
    | 'HAD'
    | 'HAS'
    | 'HAVE'
    | 'IS'
    | 'SHOULD'
    | 'WAS'
    | 'WERE'
    | 'WO'
    | 'WOULD'
    ;

  contractions_neg_lc = vb_aux_lc 'n' contractions_quote 't' @s3 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"n't"); } ;
  contractions_neg_tc = vb_aux_tc 'n' contractions_quote 't' @s3 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"n't"); } ;
  contractions_neg_uc = vb_aux_uc 'N' contractions_quote 'T' @s3 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"N'T"); } ;
  contractions_neg = contractions_neg_lc | contractions_neg_tc | contractions_neg_uc ;

  contractions_neg_error_lc = vb_aux_lc 'nt' @s2 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"n't"); } ;
  contractions_neg_error_tc = vb_aux_tc 'nt' @s2 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"n't"); } ;
  contractions_neg_error_uc = vb_aux_uc 'NT' @s2 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"N'T"); } ;
  contractions_neg_error = contractions_neg_error_lc | contractions_neg_error_tc | contractions_neg_error_uc ;

}%%
