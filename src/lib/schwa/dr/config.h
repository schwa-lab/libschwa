/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_CONFIG_H_
#define SCHWA_DR_CONFIG_H_

#include <string>

#include <schwa/config.h>

namespace schwa {
  namespace dr {

    class BaseDocSchema;
    class BaseDef;
    class BaseSchema;

    class DocrepClassGroup;
    class DocrepGroup;


    class OpDocrepField : public config::Op<std::string> {
    protected:
      BaseDef &_field;

      virtual bool _validate(const config::Main &main) override;

    public:
      OpDocrepField(DocrepClassGroup &group, BaseDef &field);
      virtual ~OpDocrepField(void) { }

    private:
      DISALLOW_COPY_AND_ASSIGN(OpDocrepField);
    };


    class DocrepClassGroup : public config::Group {
    protected:
      BaseSchema &_schema;

      virtual void _help_self(std::ostream &out, unsigned int depth) const override;

    public:
      DocrepClassGroup(DocrepGroup &group, BaseDocSchema &schema);
      DocrepClassGroup(DocrepGroup &group, BaseSchema &schema);
      virtual ~DocrepClassGroup(void) { }

      virtual bool accepts_assignment(void) const override;
      virtual bool accepts_mention(void) const override;

      virtual void assign(const std::string &value) override;
      virtual void mention(void) override;

    private:
      DISALLOW_COPY_AND_ASSIGN(DocrepClassGroup);
    };


    class DocrepGroup : public config::Group {
    protected:
      BaseDocSchema &_dschema;

    public:
      DocrepGroup(Group &group, BaseDocSchema &dschema, const std::string &name="dr", const std::string &desc="Schema configuration for docrep documents");
      virtual ~DocrepGroup(void) { }

    private:
      DISALLOW_COPY_AND_ASSIGN(DocrepGroup);
    };

  }
}

#endif  // SCHWA_DR_CONFIG_H_
