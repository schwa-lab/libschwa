namespace schwa {
  namespace docrep {

    class Document {
    private:
      const char *const _bytes;
      const uint32_t _nbytes;
      std::vector<Token *> _tokens;
      std::map<std::string, std::vector<Span *>> _spans;

      Document(const Document &) = delete;
      Document &operator =(const Document &) = delete;

    public:
      Document(const char *bytes, const uint32_t nbytes) : _bytes(bytes), _nbytes(nbytes), _tokens(), _spans() { }
      ~Document(void);

      size_t add_token(Token *tok);
      bool remove_token(const Token *tok);
      inline size_t ntokens(void) const { return _tokens.size(); }

      size_t add_span(const std::string &type, Span *span);
      bool remove_span(const std::string &type, const Span *span);
      inline size_t nspans(const std::string &type) const;

      static Document *unserialize(std::istream &in);
      std::ostream &serialize(std::ostream &out) const;
    };

  }
}
