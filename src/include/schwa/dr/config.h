/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class DocrepClassOp;


    class DocrepFieldOp : public config::OptionBase {
    protected:
      BaseDef &_field;

    public:
      DocrepFieldOp(DocrepClassOp &group, BaseDef &field);
      virtual ~DocrepFieldOp(void);

      virtual config::OptionBase *find(const std::string &orig_key, const std::string key);
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const;
      virtual void set(const std::string &value);
      virtual void validate(void);
    };


    class DocrepClassOp : public config::OptionBase {
    protected:
      BaseSchema &_schema;
      std::vector<DocrepFieldOp *> _children;

    public:
      DocrepClassOp(config::OpGroup &group, BaseDocSchema &schema);
      DocrepClassOp(config::OpGroup &group, BaseSchema &schema);
      virtual ~DocrepClassOp(void);

      inline void add(DocrepFieldOp *const child) { _children.push_back(child); }

      virtual config::OptionBase *find(const std::string &orig_key, const std::string key);
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const;
      virtual void set(const std::string &value);
      virtual void validate(void);
    };


    class DocrepOpGroup : public config::OpGroup {
    protected:
      BaseDocSchema &_dschema;

    public:
      DocrepOpGroup(OpGroup &group, BaseDocSchema &dschema, const std::string &name="dr", const std::string &desc="Docrep model options");
      virtual ~DocrepOpGroup(void) { }
    };

  }
}
