/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_CONFIG_H_
#define SCHWA_DR_CONFIG_H_

#include <string>
#include <vector>

#include <schwa/_base.h>
#include <schwa/config/base.h>
#include <schwa/config/group.h>

namespace schwa {
  namespace dr {

    class BaseDocSchema;
    class BaseDef;
    class BaseSchema;
    class DocrepClassOp;


    class DocrepFieldOp : public config::OptionBase {
    protected:
      BaseDef &_field;

    public:
      DocrepFieldOp(DocrepClassOp &group, BaseDef &field);
      virtual ~DocrepFieldOp(void);

      virtual config::OptionBase *find(const std::string &orig_key, const std::string &key) override;
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const override;
      virtual void set(const std::string &value) override;
      virtual void validate(void) override;

    private:
      DISALLOW_COPY_AND_ASSIGN(DocrepFieldOp);
    };


    class DocrepClassOp : public config::OptionBase {
    protected:
      BaseSchema &_schema;
      std::vector<DocrepFieldOp *> _children;

    public:
      DocrepClassOp(config::OpGroup &group, BaseDocSchema &schema);
      DocrepClassOp(config::OpGroup &group, BaseSchema &schema);
      virtual ~DocrepClassOp(void);

      void add(DocrepFieldOp *const child);

      virtual config::OptionBase *find(const std::string &orig_key, const std::string &key) override;
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const override;
      virtual void set(const std::string &value) override;
      virtual void validate(void) override;

    private:
      DISALLOW_COPY_AND_ASSIGN(DocrepClassOp);
    };


    class DocrepOpGroup : public config::OpGroup {
    protected:
      BaseDocSchema &_dschema;

    public:
      DocrepOpGroup(OpGroup &group, BaseDocSchema &dschema, const std::string &name="dr", const std::string &desc="Docrep model options");
      virtual ~DocrepOpGroup(void) { }

    private:
      DISALLOW_COPY_AND_ASSIGN(DocrepOpGroup);
    };

  }
}

#endif
