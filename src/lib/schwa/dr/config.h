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


    class DocrepFieldOp : public config::ConfigNode {
    protected:
      BaseDef &_field;

    public:
      DocrepFieldOp(DocrepClassOp &group, BaseDef &field);
      virtual ~DocrepFieldOp(void);

      virtual config::ConfigNode *find(const std::string &key) override;
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const override;

      virtual bool accepts_assignment(void) const override;
      virtual bool accepts_mention(void) const override;

      virtual void assign(const std::string &value) override;
      virtual void mention(void) override;
      virtual bool validate(const config::Main &main) override;
    private:
      DISALLOW_COPY_AND_ASSIGN(DocrepFieldOp);
    };


    class DocrepClassOp : public config::ConfigNode {
    protected:
      BaseSchema &_schema;
      std::vector<DocrepFieldOp *> _children;

    public:
      DocrepClassOp(config::Group &group, BaseDocSchema &schema);
      DocrepClassOp(config::Group &group, BaseSchema &schema);
      virtual ~DocrepClassOp(void);

      void add(DocrepFieldOp *const child);

      virtual config::ConfigNode *find(const std::string &key) override;
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const override;

      virtual bool accepts_assignment(void) const override;
      virtual bool accepts_mention(void) const override;

      virtual void assign(const std::string &value) override;
      virtual void mention(void) override;
      virtual bool validate(const config::Main &main) override;
    private:
      DISALLOW_COPY_AND_ASSIGN(DocrepClassOp);
    };


    class DocrepOpGroup : public config::Group {
    protected:
      BaseDocSchema &_dschema;

    public:
      DocrepOpGroup(Group &group, BaseDocSchema &dschema, const std::string &name="dr", const std::string &desc="Docrep model options");
      virtual ~DocrepOpGroup(void) { }

    private:
      DISALLOW_COPY_AND_ASSIGN(DocrepOpGroup);
    };

  }
}

#endif  // SCHWA_DR_CONFIG_H_
