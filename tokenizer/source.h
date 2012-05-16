
namespace schwa {

class Source {
public:
	virtual size_t read(char *buffer, size_t nbytes) = 0;
};

}
