/* -*- Mode: C++; -*- */

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <string>
#include <exception>

class PyRaise {
public:
  PyRaise(void){}
  ~PyRaise(void) throw(){}
};

class PyError: public std::exception {
public:
  PyObject *except;
  std::string msg;

  PyError(PyObject *except, const std::string &msg): except(except), msg(msg){}
  ~PyError(void) throw(){}
};

class ValueError: public PyError {
public:
  ValueError(const std::string &msg): PyError(PyExc_ValueError, msg){}
};

class TypeError: public PyError {
public:
  TypeError(const std::string &msg): PyError(PyExc_TypeError, msg){}
};

inline static PyObject *
add_to_module(PyObject *mod, const char *name, PyObject *obj){
	Py_INCREF(obj);
  PyModule_AddObject(mod, name, obj);
	return obj;
}

inline static PyObject *
add_long_to_module(PyObject *mod, const char *name, long value){
	return add_to_module(mod, name, PyInt_FromLong(value));
}

inline static PyObject *
bool2py(bool value){
  if(value)
    Py_RETURN_TRUE;
  Py_RETURN_FALSE;
}

inline static int
py2char(PyObject *value){
  if(PyString_Check(value)){
    if(PyString_Size(value) != 1)
      throw ValueError("string must be a single character long");
    return *PyString_AsString(value);
  }else if(PyUnicode_Check(value)){
    if(PyUnicode_GetSize(value) != 1)
      throw ValueError("string must be a single character long");
    return *PyUnicode_AsUnicode(value);
  }

  throw TypeError("expected a string or Unicode object");
}

#define M_BOOL(KLASS, FIELD, FN) static PyObject * Py##KLASS##_##FN(Py##KLASS *self){ \
  return bool2py(self->FIELD.FN()); \
}

#define NOARGS(KLASS, FN, DOC) {#FN, (PyCFunction)Py##KLASS##_##FN, METH_NOARGS, DOC}

#define CHAR_GETTER(KLASS, FIELD, FN) static PyObject *Py##KLASS##_get_##FN(Py##KLASS *self, void *closure){ \
  Py_UNICODE c = self->FIELD.FN(); \
  PyObject *res = PyUnicode_FromUnicode(&c, 1); \
  Py_INCREF(res); \
  return res; \
}

#define CHAR_SETTER(KLASS, FIELD, FN) static int Py##KLASS##_set_##FN(Py##KLASS *self, PyObject *value, void *closure){ \
  try{ \
    self->FIELD.FN(py2char(value));	\
    return 0; \
  }catch(PyError &e){ \
    PyErr_SetString(e.except, e.msg.c_str()); \
  }catch(schwa::Error &e){ \
    PyErr_SetString(PyExc_ValueError, e.msg.c_str()); \
  } \
  return -1; \
}

#define STR_GETTER(KLASS, FIELD, FN) static PyObject *Py##KLASS##_get_##FN(Py##KLASS *self, void *closure){ \
  const char *s = self->FIELD.FN(); \
  PyObject *res = PyUnicode_DecodeUTF8(s, strlen(s), 0); \
  Py_INCREF(res); \
  return res; \
}

#define STR_SETTER(KLASS, FIELD, FN) static int Py##KLASS##_set_##FN(Py##KLASS *self, PyObject *value, void *closure){ \
  try{ \
    if(PyString_Check(value)) \
      self->FIELD.FN(PyString_AS_STRING(value), PyString_GET_SIZE(value)); \
    else if(PyUnicode_Check(value)) \
      self->FIELD.FN(PyUnicode_AS_UNICODE(value), PyUnicode_GET_SIZE(value)); \
    else \
      throw TypeError("expected a string or Unicode object"); \
    return 0; \
  }catch(PyError &e){ \
    PyErr_SetString(e.except, e.msg.c_str()); \
  }catch(schwa::Error &e){ \
    PyErr_SetString(PyExc_ValueError, e.msg.c_str()); \
  } \
  return -1; \
}
