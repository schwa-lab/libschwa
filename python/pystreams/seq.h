/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa { namespace token {

typedef std::vector<PyObject *> PyVector;

class PySeqStream: public PyStream {
protected:
	PyVector paragraphs_;
	PyVector sentences_;
	PyVector tokens_;

  virtual PyObject *vector2seq_(PyVector &vec) const = 0;
public:
	PySeqStream(){}

  virtual void add(Type type, const char *raw, offset_type begin,
                   offset_type len, const char *norm = 0){
		PyObject *tuple = 0;
    Py_ssize_t pybegin = begin;
    int pylen = len;
    if(norm)
      tuple = Py_BuildValue("ns#s", pybegin, raw, pylen, norm);
    else
			tuple = Py_BuildValue("ns#", pybegin, raw, pylen);
    Py_INCREF(tuple);
		tokens_.push_back(tuple);
	}

  virtual void error(const char *raw, offset_type begin, offset_type len){}

  PyObject *get(void){
    PyObject *seq = vector2seq_(paragraphs_);
    tokens_.resize(0);
    sentences_.resize(0);
    paragraphs_.resize(0);
    return seq;
  }

  virtual void begin_sentence(void){}
	virtual void end_sentence(void){
    sentences_.push_back(vector2seq_(tokens_));
    tokens_.resize(0);
	}

  virtual void begin_paragraph(void){}
	virtual void end_paragraph(void){
    paragraphs_.push_back(vector2seq_(sentences_));
    sentences_.resize(0);
  }

  virtual void begin_heading(int depth){}
  virtual void end_heading(int depth){}

  virtual void begin_list(void){}
  virtual void end_list(void){
    end_paragraph();
  }

  virtual void begin_item(void){}
  virtual void end_item(void){}

  virtual void begin_document(void){}
	virtual void end_document(void){}
};

class PyListStream: public PySeqStream {
protected:
  virtual PyObject *vector2seq_(PyVector &vec) const {
		PyObject *list = PyList_New(vec.size());
    Py_ssize_t index = 0;
    for(PyVector::iterator i = vec.begin(); i != vec.end(); ++i, ++index)
      PyList_SET_ITEM(list, index, *i);
    Py_INCREF(list);
    return list;
  }
};

class PyTupleStream: public PySeqStream {
protected:
  virtual PyObject *vector2seq_(PyVector &vec) const {
		PyObject *tuple = PyTuple_New(vec.size());
    Py_ssize_t index = 0;
    for(PyVector::iterator i = vec.begin(); i != vec.end(); ++i, ++index)
      PyTuple_SET_ITEM(tuple, index, *i);
    Py_INCREF(tuple);
    return tuple;
  }
};

} }
