/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/type_traits/is_base_of.hpp>

namespace schwa {
  namespace dr {

    class Annotation;
    class DocSchema;
    class TypeSchema;


    class TypeRegistry {
    public:
      typedef std::vector<TypeSchema *> container_type;

    protected:
      container_type _schemas;
      DocSchema *const _doc_schema;
      bool _finalised;

      TypeRegistry(DocSchema *doc_schema) : _doc_schema(doc_schema), _finalised(false) { }

    public:
      TypeRegistry(const TypeRegistry &&o) : _schemas(o._schemas), _doc_schema(o._doc_schema), _finalised(o._finalised) { }

      ~TypeRegistry(void) {
        delete _doc_schema;
        for (auto &s : _schemas)
          delete s;
      }

      template <typename D>
      static TypeRegistry create(void) {
        static_assert(boost::is_base_of<Document, D>::value, "D must be a subclass of Document");
        static_assert(boost::is_base_of<DocSchema, typename D::Schema>::value, "D::Schema must be a subclass of DocSchema");
        typename D::Schema *schema = new typename D::Schema();
        assert(schema != nullptr);
        return TypeRegistry(schema);
      }

      template <typename T>
      inline typename T::Schema &
      add(void) {
        static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");
        static_assert(boost::is_base_of<TypeSchema, typename T::Schema>::value, "T::Schema must be a subclass of TypeSchema");
        typename T::Schema *schema = new typename T::Schema();
        assert(schema != nullptr);
        _schemas.push_back(schema);
        return *schema;
      }

      void finalise(void);

      inline size_t size(void) const { return _schemas.size(); }

      inline DocSchema &doc_schema(void) const { return *_doc_schema; }
      inline container_type::const_iterator begin(void) const { return _schemas.begin(); }
      inline container_type::const_iterator end(void) const { return _schemas.end(); }
    };

  }
}
