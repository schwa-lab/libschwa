/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class RTSchema;
    class RTFieldDef;
    class RTStoreDef;

    class RTFieldDef {
    public:
      const BaseFieldDef *def;
      const RTStoreDef *pointer;
      std::string serial;
      uint32_t field_id;
      bool is_slice;

      RTFieldDef(uint32_t field_id, const std::string &serial, const RTStoreDef *pointer, bool is_slice, const BaseFieldDef *def=nullptr);
      RTFieldDef(const RTFieldDef &&o);
      ~RTFieldDef(void) { }

      inline bool is_lazy(void) const { return def == nullptr; }
    };


    class RTStoreDef {
    public:
      const BaseStoreDef *def;
      const RTSchema *klass;
      const char *lazy_data;
      uint32_t lazy_nbytes;
      uint32_t lazy_nelem;
      uint32_t store_id;
      std::string serial;

      RTStoreDef(uint32_t store_id, const std::string &serial, const RTSchema *klass, const BaseStoreDef *def);
      RTStoreDef(uint32_t store_id, const std::string &serial, const RTSchema *klass, const char *lazy_data, uint32_t lazy_nbytes, uint32_t lazy_nelem);
      RTStoreDef(const RTStoreDef &&o);
      ~RTStoreDef(void) { }

      inline bool is_lazy(void) const { return def == nullptr; }
    };


    class RTSchema {
    public:
      const BaseSchema *def;
      std::vector<RTFieldDef *> fields;
      std::vector<RTStoreDef *> stores;
      uint32_t klass_id;

      explicit RTSchema(uint32_t klass_id, const BaseSchema *def=nullptr);
      RTSchema(const RTSchema &&o);
      ~RTSchema(void);

      inline bool is_lazy(void) const { return def == nullptr; }
    };


    class RTManager {
    public:
      const RTSchema *doc;
      std::vector<RTSchema *> klasses;
      std::vector<char *> lazy_buffers;

      ~RTManager(void);
    };


    RTManager *build_rt(const BaseDocSchema &dschema);
    void merge_rt(RTManager *const rt, const BaseDocSchema &dschema);

  }
}
