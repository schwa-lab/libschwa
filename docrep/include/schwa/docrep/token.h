namespace schwa {
  namespace docrep {

    class Token {
    private:
      std::map<std::string, msgpack::object> _map;

    public:
      Token(void) : _map() { }

      void add(const std::string &name, const msgpack::object &value);

      std::ostream &debug(std::ostream &out) const;

      WireIStream &unserialize(WireIStream &in);
      std::ostream &serialize(std::ostream &out) const;

      bool operator ==(const Token &o) const { return _map == o._map; }
    };


    inline std::ostream &
    operator <<(std::ostream &out, const Token &t) {
      return t.debug(out);
    }

  }
}
