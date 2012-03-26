/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa { namespace token {

// the multiple inheritance option is a dog in C++
class PyTextStream: public PyStream {
protected:
  std::ostringstream out_;
  TextStream delegate_;

public:
  PyTextStream(bool normalise)
    : delegate_(out_, normalise){}
  virtual ~PyTextStream(void){}

  virtual PyObject *get(void){
    const std::string res = out_.str();
    return PyString_FromStringAndSize(res.data(), res.size());
  }

  virtual void add(Type type, const char *raw, offset_type begin,
                   offset_type len, const char *norm = 0){
    delegate_.add(type, raw, begin, len, norm);
  }

  virtual void error(const char *raw, offset_type begin, offset_type len){
    delegate_.error(raw, begin, len);
  }

  virtual void begin_sentence(void){ delegate_.begin_sentence(); }
  virtual void end_sentence(void){ delegate_.end_sentence(); }

  virtual void begin_paragraph(void){ delegate_.begin_paragraph(); }
  virtual void end_paragraph(void){ delegate_.end_paragraph(); }

  virtual void begin_heading(int depth){ delegate_.begin_heading(depth); }
  virtual void end_heading(int depth){ delegate_.end_heading(depth); }

  virtual void begin_list(void){ delegate_.begin_list(); }
  virtual void end_list(void){ delegate_.end_list(); }

  virtual void begin_item(void){ delegate_.begin_item(); }
  virtual void end_item(void){ delegate_.end_item(); }

  virtual void begin_document(void){ delegate_.begin_document(); }
  virtual void end_document(void){ delegate_.end_document(); }
};


class PyBytesStream: public PyTextStream {
public:
  PyBytesStream(bool normalise): PyTextStream(normalise){}
  virtual ~PyBytesStream(void){}
};


class PyUnicodeStream: public PyTextStream {
public:
  PyUnicodeStream(bool normalise): PyTextStream(normalise){}
  virtual ~PyUnicodeStream(void){}

  virtual PyObject *get(void){
    const std::string res = out_.str();
    return PyUnicode_DecodeUTF8(res.data(), res.size(), "strict");
  }
};

} }
