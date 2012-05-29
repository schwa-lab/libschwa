/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/type_traits/is_base_of.hpp>

namespace schwa {
  namespace dr {

    class Annotation;
    class BaseAnnotationSchema;
    class BaseDocumentSchema;
    class Schema;


    class TypeRegistry {
    public:
      typedef std::vector<BaseAnnotationSchema *> container_type;

    protected:
      container_type _schemas;
      BaseDocumentSchema *const _doc_schema;
      bool _finalised;

      TypeRegistry(BaseDocumentSchema *doc_schema) : _doc_schema(doc_schema), _finalised(false) { }

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
        static_assert(boost::is_base_of<BaseDocumentSchema, typename D::Schema>::value, "D::Schema must be a subclass of BaseDocumentSchema");
        typename D::Schema *schema = new typename D::Schema();
        assert(schema != nullptr);
        return TypeRegistry(schema);
      }

      template <typename T>
      inline typename T::Schema &
      add(void) {
        static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");
        static_assert(boost::is_base_of<BaseAnnotationSchema, typename T::Schema>::value, "T::Schema must be a subclass of BaseAnnotationSchema");
        _finalised = false;
        typename T::Schema *schema = new typename T::Schema();
        assert(schema != nullptr);
        _schemas.push_back(schema);
        return *schema;
      }

      void finalise(void);

      inline size_t size(void) const { return _schemas.size(); }

      inline Schema &doc_schema(void) const { return *_doc_schema; }
      inline container_type::const_iterator begin(void) const { return _schemas.begin(); }
      inline container_type::const_iterator end(void) const { return _schemas.end(); }
    };

  }
}
