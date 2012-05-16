
namespace schwa {
  namespace io {

    class Source {
    public:
      virtual ~Source(void) { }

      virtual size_t read(char *buffer, size_t nbytes) = 0;
    };

  }
}
