
namespace schwa {

class IStreamSource: public Source {
protected:
	std::istream &stream_;
public:
	IStreamSource(std::istream &stream)
		: stream_(stream){}

	virtual size_t read(char *buffer, size_t nbytes){
		return stream_.read(buffer, nbytes).gcount();
	}
};

}
