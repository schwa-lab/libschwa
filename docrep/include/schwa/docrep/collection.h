namespace schwa {
  namespace docrep {

    class Collection {
    private:
      std::vector<Document *> _docs;

    public:
      size_t add_doc(Document *doc);
      bool remove_doc(const Document *doc);

      inline size_t size(void) const { return _docs.size(); }

      std::istream &unserialize(std::istream &in);
      std::ostream &serialize(std::ostream &out) const;
    };


    inline std::ostream &
    operator <<(std::ostream &out, const Collection &c) {
      return c.serialize(out);
    }


    inline std::istream &
    operator >>(std::istream &in, Collection &c) {
      return c.unserialize(in);
    }

  }
}
