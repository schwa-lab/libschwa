
namespace schwa {

class FileSource: public Source {
protected:
	FILE *file_;
public:
	FileSource(FILE *file)
		: file_(file){}

	virtual size_t read(char *buffer, size_t nbytes){
		return fread(buffer, sizeof(char), nbytes, file_);
	}
};

}
