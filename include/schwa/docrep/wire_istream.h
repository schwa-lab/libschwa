namespace schwa {
  namespace docrep {

    class NoNextObjectException : public std::exception { };
    class DeserializeException : public std::exception { };


    class WireIStream {
    private:
      std::istream &_in;
      msgpack::unpacker _unpacker;
      msgpack::unpacked _result;
      bool _has_next;
      bool _eof;

      void _next(void);

      WireIStream(const WireIStream &);
      WireIStream &operator =(const WireIStream &);

    public:
      WireIStream(std::istream &in) : _in(in), _unpacker(), _result(), _has_next(false), _eof(false) {
        _next();
      }

      bool eof(void) const { return _eof; }
      bool has_next(void) const { return _has_next; }

      msgpack::object
      get(void) {
        if (!has_next())
          throw new NoNextObjectException();
        return _result.get();
      }

      bool
      next(void) {
        _next();
        return has_next();
      }

      operator bool(void) const { return !_eof; }
    };

  }
}

