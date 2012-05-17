/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace io {

    class IStreamSource : public Source {
    protected:
      std::istream &_istream;

    public:
      IStreamSource(std::istream &istream) : _istream(istream) { }
      virtual ~IStreamSource(void) { }

      virtual size_t read(char *buffer, size_t nbytes) {
        return _istream.read(buffer, nbytes).gcount();
      }
    };

  }
}
