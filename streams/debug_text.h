/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa { namespace token {

class DebugTextStream: public Stream {
protected:
	std::ostream &out_;
	bool new_sentence_;

	unsigned int i;
public:
	DebugTextStream(std::ostream &out)
		: out_(out), new_sentence_(true){}

  virtual void add(Type type, const char *raw, offset_type begin,
									 offset_type len, const char *norm = 0){
    const static int NCOLOURS = 7;
		const static char *const COLOURS[] = {
			"\033[0;31m", "\033[0;32m", "\033[1;33m", "\033[1;34m",
      "\033[0;35m", "\033[0;36m", "\033[1;37m"
    };
		const static char *const OFF = "\033[0m";

		if(!new_sentence_)
			out_ << ' ';
		new_sentence_ = false;

		if(norm)
			out_ << COLOURS[i++ % NCOLOURS] << norm << OFF;
		else
			(out_ << COLOURS[i++ % NCOLOURS]).write(raw, len) << OFF;
	}

  virtual void error(const char *raw, offset_type begin, offset_type len){
    if(!new_sentence_)
      out_ << ' ';
    (out_ << "<error>").write(raw, len) << "</error>";
  }

  virtual void begin_sentence(void){
    out_ << "<s>";
		new_sentence_ = true;
    i = 0;
	}
	virtual void end_sentence(void){
    out_ << "</s>\n";
	}

  virtual void begin_list(void){
    out_ << "<ul>\n";
	}

	virtual void end_list(void){
    out_ << "</ul>\n";
  }

  virtual void begin_item(void){
    out_ << "<li>\n";
	}

	virtual void end_item(void){
    out_ << "</li>\n";
  }

  virtual void begin_paragraph(void){
    out_ << "<p>\n";
	}

	virtual void end_paragraph(void){
    out_ << "</p>\n";
  }

  virtual void begin_heading(int depth){
    out_ << "<h" << depth << ">\n";
  }
  virtual void end_heading(int depth){
    out_ << "</h" << depth << ">\n";
  }

  virtual void begin_document(void){
    out_ << "<doc>\n";
	}

	virtual void end_document(void){
    out_ << "</doc>\n";
  }
};

} }
