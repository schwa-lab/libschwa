
namespace schwa {

class PyFileSource: public FileSource {
protected:
  PyFileObject *obj_;
public:
	PyFileSource(PyObject *obj)
		: FileSource(PyFile_AsFile(obj)), obj_((PyFileObject *)obj){
		Py_INCREF(obj_);
		PyFile_IncUseCount(obj_);
	}

	~PyFileSource(void){
		PyFile_DecUseCount(obj_);
		Py_DECREF(obj_);
	}
};


}
