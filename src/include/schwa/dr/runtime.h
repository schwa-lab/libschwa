/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_RUNTIME_H_
#define SCHWA_DR_RUNTIME_H_

#include <string>
#include <vector>

#include <schwa/_base.h>

namespace schwa {
  namespace dr {

    class BaseDocSchema;
    class BaseFieldDef;
    class BaseSchema;
    class BaseStoreDef;
    class RTSchema;
    class RTFieldDef;
    class RTStoreDef;


    class RTFieldDef {
    public:
      const BaseFieldDef *def;
      const RTStoreDef *points_into;
      const std::string serial;
      const uint32_t field_id;
      const bool is_slice : 1;
      const bool is_self_pointer : 1;
      const bool is_collection : 1;

      RTFieldDef(uint32_t field_id, const std::string &serial, const RTStoreDef *points_into, bool is_slice, bool is_self_pointer, bool is_collection, const BaseFieldDef *def=nullptr);
      RTFieldDef(const RTFieldDef &&o);
      ~RTFieldDef(void) { }

      inline bool is_lazy(void) const { return def == nullptr; }

      std::ostream &dump(std::ostream &out) const;

    private:
      DISALLOW_COPY_AND_ASSIGN(RTFieldDef);
    };

    std::ostream &operator <<(std::ostream &out, const RTFieldDef &field);


    class RTStoreDef {
    public:
      const BaseStoreDef *def;
      const RTSchema *klass;
      const char *lazy_data;
      uint32_t lazy_nbytes;
      uint32_t lazy_nelem;
      const uint32_t store_id;
      const std::string serial;

      RTStoreDef(uint32_t store_id, const std::string &serial, const RTSchema *klass, const BaseStoreDef *def);
      RTStoreDef(uint32_t store_id, const std::string &serial, const RTSchema *klass, const char *lazy_data, uint32_t lazy_nbytes, uint32_t lazy_nelem);
      RTStoreDef(const RTStoreDef &&o);
      ~RTStoreDef(void) { }

      inline bool is_lazy(void) const { return def == nullptr; }

      std::ostream &dump(std::ostream &out) const;

    private:
      DISALLOW_COPY_AND_ASSIGN(RTStoreDef);
    };

    std::ostream &operator <<(std::ostream &out, const RTStoreDef &store);


    class RTSchema {
    public:
      const BaseSchema *def;
      std::vector<RTFieldDef *> fields;
      std::vector<RTStoreDef *> stores;
      const uint32_t klass_id;
      const std::string serial;

      explicit RTSchema(uint32_t klass_id, const std::string &serial, const BaseSchema *def=nullptr);
      RTSchema(const RTSchema &&o);
      ~RTSchema(void);

      inline bool is_lazy(void) const { return def == nullptr; }

      std::ostream &dump(std::ostream &out) const;

    private:
      DISALLOW_COPY_AND_ASSIGN(RTSchema);
    };

    std::ostream &operator <<(std::ostream &out, const RTSchema &schema);


    class RTManager {
    public:
      const RTSchema *doc;
      std::vector<RTSchema *> klasses;
      std::vector<char *> lazy_buffers;

      RTManager(void);
      ~RTManager(void);

      std::ostream &dump(std::ostream &out) const;

    private:
      DISALLOW_COPY_AND_ASSIGN(RTManager);
    };

    std::ostream &operator <<(std::ostream &out, const RTManager &rt);


    // ============================================================================
    // Functions
    // ============================================================================
    RTManager *build_rt(const BaseDocSchema &dschema);
    RTManager *merge_rt(RTManager *const rt, const BaseDocSchema &dschema);

  }
}

#endif  // SCHWA_DR_RUNTIME_H_
