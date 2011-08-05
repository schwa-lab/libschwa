/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa { namespace tokens {

class TextStream: public Stream {
protected:
	std::ostream &out_;
	bool new_document_;
	bool new_sentence_;
public:
	TextStream(std::ostream &out)
		: out_(out), new_document_(true), new_sentence_(true){}

  virtual void add(Type type, const char *raw, offset_type begin, offset_type len, const char *norm = 0){
		if(!new_sentence_)
			out_ << ' ';
		new_sentence_ = false;

		if(norm)
			out_ << norm;
		else
			out_.write(raw, len);
	}

  virtual void begin_sentence(void){
		new_sentence_ = true;
	}
	virtual void end_sentence(void){
		out_ << '\n';
	}

  virtual void begin_paragraph(void){
		if(!new_document_)
			out_ << '\n';
		new_document_ = false;
	}

	virtual void end_paragraph(void){}

  virtual void begin_heading(int depth){}
  virtual void end_heading(int depth){}

  virtual void begin_list(void){ begin_paragraph(); }
  virtual void end_list(void){}

  virtual void begin_item(void){
    out_ << "* ";
  }
  virtual void end_item(void){}

  virtual void begin_document(void){
		new_document_ = true;
	}

	virtual void end_document(void){}
};

} }
