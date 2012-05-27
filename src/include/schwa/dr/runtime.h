/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace schwa {
  namespace dr {

    class Annotation;
    class Document;
    class Schema;


    class TypeRegistry {
    protected:
      typedef std::vector<Schema *> container_type;
      container_type _schemas;
      bool _finalised;

      void finalise(void);

    public:
      TypeRegistry(void) : _finalised(false) { }
      ~TypeRegistry(void) {
        for (auto &s : _schemas)
          delete s;
      }

      template <typename T>
      inline typename T::Schema &
      add(void) {
        typedef typename T::Schema S;
        static_assert(boost::mpl::or_<boost::is_base_of<Annotation, T>, boost::is_base_of<Document, T>>::value, "T must either be a subclass of Annotation or a subclass of Document");
        S *schema = new typename T::Schema();
        _schemas.push_back(schema);
        return *schema;
      }

      inline size_t size(void) const { return _schemas.size(); }

      inline container_type::const_iterator begin(void) const { return _schemas.begin(); }
      inline container_type::const_iterator end(void) const { return _schemas.end(); }
    };

  }
}
