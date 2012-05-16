
namespace schwa {
  namespace io {

    class FileSource : public Source {
    protected:
      FILE *_file;

    public:
      FileSource(FILE *file) : _file(file) { }
      virtual ~FileSource(void) { }

      virtual size_t read(char *buffer, size_t nbytes) {
        return ::fread(buffer, sizeof(char), nbytes, _file);
      }
    };

  }
}
