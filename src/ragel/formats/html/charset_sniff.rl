// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine html_charset_sniff;
  alphtype unsigned char;

  action attribute_name_start  { _attribute_name_start(fpc); }
  action attribute_name_end    { _attribute_name_end(fpc); }
  action attribute_value_start { _attribute_value_start(fpc); }
  action attribute_value_end   { _attribute_value_end(fpc); }
  action meta_end              { _meta_end(); }

  # Use this to try and determine the character encoding of a HTML file from a <meta declaration.
  # This implements the "prescan a byte stream to determine its encoding" algorithm from the HTML5
  # specification.
  #
  # http://www.w3.org/html/wg/drafts/html/master/syntax.html#prescan-a-byte-stream-to-determine-its-encoding

  comment = '<!--' any* :>> '-->' ;
  cdata = '<![CDATA[' any* :>> ']]>' ;
  attribute =
    [^\t\v\f\n\r "'>/=]+ >attribute_name_start %attribute_name_end
    (
      space* '=' space*
      (
          (any - [\t\v\f\n\r "'><`])+ >attribute_value_start %attribute_value_end
        | "'" (any - ['])* >attribute_value_start %attribute_value_end "'"
        | '"' (any - ["])* >attribute_value_start %attribute_value_end '"'
      )
    )? ;
  meta_tag = '<meta'i ( space+ attribute )* space* '/'? '>' %meta_end ;
}%%
