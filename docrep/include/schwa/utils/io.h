namespace schwa {

  inline
  void dump_uchar(std::ostream &out, unsigned char value){
    out.put(value);
  }

  inline
  bool load_uchar(std::istream &in, unsigned char &value){
    return in.get((char &)value);
  }

  inline
  void dump_ushort(std::ostream &out, unsigned short value){
    dump_uchar(out, 0xFF & value);
    dump_uchar(out, 0xFF & (value >> 8));
  }

  inline
  bool load_ushort(std::istream &in, unsigned short &value){
    unsigned char tmp;
    if(!load_uchar(in, tmp))
      return false;
    value = (unsigned short)tmp;

    if(!load_uchar(in, tmp))
      return false;
    value |= (unsigned short)tmp << 8;

    return true;
  }

  inline
  void dump_ulong(std::ostream &out, unsigned long value){
    dump_uchar(out, 0xFF & value);
    dump_uchar(out, 0xFF & (value >> 8));
    dump_uchar(out, 0xFF & (value >> 16));
    dump_uchar(out, 0xFF & (value >> 24));
  }

  inline
  bool load_ulong(std::istream &in, unsigned long &value){
    unsigned char tmp;
    if(!load_uchar(in, tmp))
      return false;
    value = (unsigned long)tmp;

    if(!load_uchar(in, tmp))
      return false;
    value |= (unsigned long)tmp << 8;

    if(!load_uchar(in, tmp))
      return false;
    value |= (unsigned long)tmp << 16;

    if(!load_uchar(in, tmp))
      return false;
    value |= (unsigned long)tmp << 24;

    return true;
  }

  inline
  void dump_float(std::ostream &out, float value){
    union {
      unsigned long bits;
      float value;
    } tmp;
    tmp.value = value;

    dump_uchar(out, 0xFF & tmp.bits);
    dump_uchar(out, 0xFF & (tmp.bits >> 8));
    dump_uchar(out, 0xFF & (tmp.bits >> 16));
    dump_uchar(out, 0xFF & (tmp.bits >> 24));
  }

  inline
  bool load_float(std::istream &in, float &value){
    union {
      unsigned long bits;
      float value;
    } res;

    unsigned char tmp;
    if(!load_uchar(in, tmp))
      return false;
    res.bits = (unsigned long)tmp;

    if(!load_uchar(in, tmp))
      return false;
    res.bits |= (unsigned long)tmp << 8;

    if(!load_uchar(in, tmp))
      return false;
    res.bits |= (unsigned long)tmp << 16;

    if(!load_uchar(in, tmp))
      return false;
    res.bits |= (unsigned long)tmp << 24;

    value = res.value;

    return true;
  }

  inline
  void dump_string(std::ostream &out, const char *const str){
    int len = strlen(str);
    assert(len <= USHRT_MAX);
    dump_ushort(out, len);
    out.write(str, len);
  }

  inline
  bool load_string(std::istream &in, char *buffer, unsigned short nbuffer){
    unsigned short len;
    if(!load_ushort(in, len))
      return false;
    if(len >= nbuffer)
      throw NLP::IOException("string too long for buffer in load_string");
    if(!in.read(buffer, len))
      return false;
    buffer[len] = '\0';
    return true;
  }

}
