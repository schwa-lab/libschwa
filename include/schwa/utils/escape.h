namespace schwa {
  // convert a control character into its escaped version \t -> t
  char char2esc(char c);
  // convert a control character into its escaped version \t -> \\t
  std::string char2esc_str(char c);
  // convert an escaped character into its control version t -> \t
  char esc2char(char c);


  //to convert escape sequences into 0x??
  std::string asc2bin(const std::string &s);

  //to convert escape sequences into 0x??
  std::string bin2asc(const std::string &s);

  //remove quotes if they exist
  std::string strip_quotes(const std::string &s);

  //removes whitespace at the beginning and end of string
  std::string strip_whitespace(const std::string &s);
}
