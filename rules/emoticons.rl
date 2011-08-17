/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  # from list on Wikipedia: http://en.wikipedia.org/wiki/List_of_emoticons
  # with additions from:
  # http://netforbeginners.about.com/cs/netiquette101/a/bl_emoticons101_2.htm
  # http://netforbeginners.about.com/cs/netiquette101/a/bl_emoticons101_3.htm
  # http://netforbeginners.about.com/cs/netiquette101/a/bl_emoticons101_4.htm
  # http://netforbeginners.about.com/cs/netiquette101/a/bl_emoticons101_5.htm

	emo_smile = ">:]"|":-)"|":)"|":o)"|":]"|":3"|":c)"|":>"|"=]"|"8)"|"=)"|":}"|":^)"|":-{)"|":-{)}"|":-}"|":->";
  emo_laugh = ">:D"|":-D"|":D"|"8-D"|"8D"|"x-D"|"xD"|"X-D"|"XD"|"=-D"|"=D"|"=-3"|"=3"|":^D"|"=^D"|"^^^";
  emo_cry = ":'("|";*("|"T.T"|"T_T"|"Y.Y"|"Y_Y"|":~-("|":\'"|":\'("|":\'-("|":.("|":,(";
  emo_frown = ">:["|":-("|":("|":^("|":-c"|":c"|":-<"|":<"|":-["|":["|":{"|">.>"|"<.<"|">.<";
  emo_sad = "D:<"|"D:"|"D8"|"D;"|"D="|"DX"|"v.v"|"D-':"|"=("|":e";
  emo_lick = ":-9";
  # includes raised eyebrow
  emo_wink = ">;]"|";-)"|";)"|"*-)"|"*)"|";-]"|";]"|";D"|">;->"|";-D"|"`:-)";
  emo_tongue = ">:p"i|":-p"i|":^P"i|":p"i|"x-p"i|"xp"i|"=p"i|":-Þ"|":Þ"|":-b"|":b"|":-r"|";p"i|"8-p"i;
  emo_surprise = ">:o"i|":-o"i|":o"i|"°o°"i|"8-o"i|"=O"|":C";
  emo_skeptical = ">:\\"|">:/"|":-/"|":-."|":/"|":\\"|"=/"|"=\\"|":S";
  emo_straight = ":|"|":-|";
  emo_drool = ":)~"|":-)>." "."+|"=F"|":p~~";
  # includes tongue tied
  emo_sealed = ">:x"i|":-x"i|":x"i|":-#"|":#"|":$"|"=X"|":-&";
  emo_angel = "O:-)"|"0:-3"|"0:3"|"O:-)"|"O:)";
  emo_evil = ">:)"|">;)"|">:-)";
  emo_shades = ">B]"|"B)"|"B-)"|"8)"|"8-)"|"B^P";
  emo_high5 = "o/\o"|"^5";
  emo_rose = "@}-;-'---";
  emo_cap = "d:-)"|"qB-)";
  emo_angry = "D:<"|">:("|">:-C"|">:C"|">:O"|"D-:<"|">:-("|":-@[1]"|";("|"`_´"|"D<"|":-||"|":-Q"|"~ :-(";
  emo_kiss = ":-*"|":*"|":-{}"|"=^*"|"|-<>"|":-><";
  # includes broken hearts
  emo_heart = "</3"|"<3"|"<3" "3"+;
  # includes joy etc
  emo_wave = "\o/"|"*\o/*"|"\o"|"o/"|"\^_^/";
  # includes drunk etc
  emo_brain = "%-)"|"%*}"|"%+\\"|"%-6"|"%-/"|"%-|";
  emo_confused = ":-s"|":-S"|":~/"|":>"|":@"|"%-)"|"%-(";
  # includes bored, yawning
  emo_sleep = "|-("|"|-O"|"|-|"|"|^o";
  emo_flame = "~:-("|"~="|"~=="|"~~:-("|"~~:[";
  emo_misc = ":&"|":u"|":3"|">.<"|":-V"|":-Y"|"?("|"?-("|"]:->"|"]:-)"|"~:o"|"*-)";

  emo0 = emo_smile | emo_laugh | emo_cry | emo_frown | emo_sad | emo_lick;
  emo1 = emo_wink | emo_tongue | emo_surprise | emo_skeptical | emo_straight;
  emo2 = emo_drool | emo_sealed | emo_angel | emo_evil | emo_shades | emo_high5;
  emo3 = emo_rose | emo_cap | emo_angry | emo_kiss | emo_heart | emo_wave;
  emo4 = emo_brain | emo_confused | emo_sleep | emo_flame | emo_misc;

  # unicode U+2639 white frowning face
  # unicode U+263a white smiling face
  # unicode U+263b black smiling face
  unicode_2639 = 0xe2 0x98 0xb9 | "&#9785;" | "&#x2639;"i;
  unicode_263a = 0xe2 0x98 0xba | "&#9786;" | "&#x263a;"i;
  unicode_263b = 0xe2 0x98 0xbb | "&#9787;" | "&#x263b;"i;

  # unicode U+1F601 grinning face with smiling eyes
  # unicode U+1F602 face with tears of joy
  # unicode U+1F603 smiling face with open mouth
  # unicode U+1F604 smiling face with open mouth and smiling eyes
  # unicode U+1F605 smiling face with open mouth and cold sweat
  # unicode U+1F606 smiling face with open mouth and tightly-closed eyes
  unicode_1f601 = 0xf0 0x9f 0x98 0x81 | "&#128513;" | "&#x1f601;"i;
  unicode_1f602 = 0xf0 0x9f 0x98 0x82 | "&#128514;" | "&#x1f602;"i;
  unicode_1f603 = 0xf0 0x9f 0x98 0x83 | "&#128515;" | "&#x1f603;"i;
  unicode_1f604 = 0xf0 0x9f 0x98 0x84 | "&#128516;" | "&#x1f604;"i;
  unicode_1f605 = 0xf0 0x9f 0x98 0x85 | "&#128517;" | "&#x1f605;"i;
  unicode_1f606 = 0xf0 0x9f 0x98 0x86 | "&#128518;" | "&#x1f606;"i;
  
  emo_unicode = unicode_2639 | unicode_263a | unicode_263b;

  emoticon = emo0 | emo1 | emo2 | emo3 | emo4 | emo_unicode;

}%%
