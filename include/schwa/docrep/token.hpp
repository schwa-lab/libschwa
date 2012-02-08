#include <msgpack.hpp>

namespace schwa {
  namespace docrep {

    class Token {
    private:
      std::map<std::string, msgpack::object> _map;

    public:
      void add(const std::string &name, const msgpack::object &value);

      std::ostream &debug(std::ostream &out) const;

      std::istream &unserialize(std::istream &in);
      std::ostream &serialize(std::ostream &out) const;
    };


    inline std::ostream &
    operator <<(std::ostream &out, const Token &t) {
      return t.debug(out);
    }

  }
}
