/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/typeof/typeof.hpp>

namespace schwa {
  namespace dr {

    typedef uint8_t loadmode_t;
    static const loadmode_t LOAD_RW = 1;
    static const loadmode_t LOAD_RO = 2;


    class BaseDef {
    protected:
      const std::string _name;
      const std::string _help;
      const loadmode_t _mode;
      std::string _serial;

      BaseDef(Schema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : _name(name), _help(help), _mode(mode), _serial(serial) {
        schema.add(this);
      }

    public:
      virtual ~BaseDef(void) { }
    };


    template <typename T, T fn>
    class FieldDef;

    template <typename R, typename T, R T::*member_obj_ptr>
    class FieldDef<R T::*, member_obj_ptr> : public BaseDef {
    public:
      typedef T doc_type;
      typedef R value_type;

      FieldDef(Schema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseDef(schema, name, help, mode, serial) { }
      virtual ~FieldDef(void) { }
    };


    template <typename T, T fn>
    class StoreDef;

    template <typename R, typename T, R T::*member_obj_ptr>
    class StoreDef<R T::*, member_obj_ptr> : public BaseDef {
    public:
      typedef T doc_type;
      typedef typename R::value_type value_type;

      StoreDef(Schema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseDef(schema, name, help, mode, serial) { }
      virtual ~StoreDef(void) { }
    };


    #define FIELD(member_obj_ptr) schwa::dr::FieldDef<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr>
    #define STORE(member_obj_ptr) schwa::dr::StoreDef<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr>


    class Annotation {
    public:
      typedef TypeSchema Schema;

    protected:
      size_t _dr_index;

      Annotation(void) : _dr_index(0) { }
      Annotation(const Annotation &) = delete;

    public:
      inline void set_dr_index(size_t dr_index) { _dr_index = dr_index; }
    };


    class Document {
    public:
      typedef DocSchema Schema;

    protected:
      Document(void) = default;
      Document(const Document &) = delete;
    };

  }
}
