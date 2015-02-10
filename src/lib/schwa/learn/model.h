/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_MODEL_H_
#define SCHWA_LEARN_MODEL_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>
#include <schwa/config.h>
#include <schwa/io/streams.h>


namespace schwa {
  namespace learn {

    class ModelParams : public config::Group {
    public:
      config::Op<std::string> command_line_path;
      config::Op<std::string> config_path;
      config::Op<std::string> log_path;
      config::OpLogLevel log_level;
      config::Op<std::string> model_path;
      config::Op<std::string> resources_path;

      ModelParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags=config::Flags::NONE);
      virtual ~ModelParams(void);

      void deserialise_all(std::istream &in);
      void serialise_all(std::ostream &out) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(ModelParams);
    };


    class InputModel {
    protected:
      const std::string _path;
      std::string _resources_path;
      std::string _model_path;

    public:
      InputModel(const std::string &path, ModelParams &params);
      virtual ~InputModel(void);

      inline const std::string &model_path(void) { return _model_path; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(InputModel);
    };


    class OutputModel {
    protected:
      const std::string _path;
      const std::string _resources_path;
      const std::string _model_path;
      io::OutputStream *_log;
      io::Logger *_logger;

    public:
      OutputModel(const std::string &path, const ModelParams &params, const config::Main &main_config);
      virtual ~OutputModel(void);

      inline io::Logger &logger(void) const { return *_logger; }
      inline const std::string &model_path(void) { return _model_path; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OutputModel);
    };

  }
}

#endif  // SCHWA_LEARN_MODEL_H_
