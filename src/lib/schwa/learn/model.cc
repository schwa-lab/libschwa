/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/learn/model.h>

#include <iostream>
#include <sstream>

#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/io/paths.h>


namespace schwa {
namespace learn {

// ============================================================================
// ModelParams
// ============================================================================
ModelParams::ModelParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags) :
    config::Group(group, name, desc, flags),
    command_line_path(*this, "command-line-path", "Relative path to the command-line file within the model directory", "./command-line"),
    config_path(*this, "config-path", "Relative path to the config file within the model directory", "./config"),
    log_path(*this, "log-path", "Relative path to the log file within the model directory", "./log"),
    log_level(*this, "log-level", "The level at which the model's log should be logging", "info"),
    model_path(*this, "model-path", "Relative path to the model ile within the model directory", "./model"),
    resources_path(*this, "resources-path", "Relative path to the resources directory within the model directory", "./resources")
  { }

ModelParams::~ModelParams(void) { }


void
ModelParams::deserialise_all(std::istream &in) {
  // For each line in the file...
  for (std::string line; std::getline(in, line); ) {
    // Skip comments and blank lines.
    if (line.empty() || line[0] == '#')
      continue;

    // Split the line into key and value.
    const size_t pos = line.find('=');
    if (pos == std::string::npos)
      continue;
    const std::string key = line.substr(0, pos);
    const std::string value = line.substr(pos + 1);

    // Try and find the corresponding config node.
    config::ConfigNode *const node = find(key);

    // Ensure the node can be mentioned.
    if (node == nullptr || !node->accepts_mention()) {
      std::ostringstream ss;
      ss << "Invalid option \"" << key << "\"";
      throw ConfigException(ss.str());
    }

    // Assign the value to the node.
    LOG(INFO) << "node '" << node->name() << " was_mentioned?=" << node->was_mentioned() << std::endl;
    node->mention();
    node->assign(value);
  }
}


void
ModelParams::serialise_all(std::ostream &out) const {
  for (auto &c : _options) {
    out << c->name() << '=';
    c->serialise(out);
    out << std::endl;
  }
}


// ============================================================================
// InputModel
// ============================================================================
InputModel::InputModel(const std::string &path, ModelParams &params) : _path(path) {
  // Validate the top-level model directory.
  if (!io::path_exists(_path)) {
    std::ostringstream ss;
    ss << "Specified model directory '" << _path << "' does not exist";
    throw IOException(ss.str());
  }
  else if (!io::path_is_dir(_path)) {
    std::ostringstream ss;
    ss << "Specified model directory '" << _path << "' exists but is not a directory";
    throw IOException(ss.str());
  }

  // Read the model params config input file.
  {
    io::InputStream in(io::path_join(_path, params.config_path()));
    params.deserialise_all(in);
  }

  // Update our member variables now that the config file has been loaded.
  _resources_path = io::path_join(_path, params.resources_path());
  _model_path = io::path_join(_path, params.model_path());

  // Validate the resources directory.
  if (!io::path_exists(_resources_path)) {
    std::ostringstream ss;
    ss << "Specified resources directory '" << _resources_path << "' does not exist";
    throw IOException(ss.str());
  }
  else if (!io::path_is_dir(_resources_path)) {
    std::ostringstream ss;
    ss << "Specified resources directory '" << _resources_path << "' exists but is not a directory";
    throw IOException(ss.str());
  }
}

InputModel::~InputModel(void) { }


// ============================================================================
// OutputModel
// ============================================================================
OutputModel::OutputModel(const std::string &path, const ModelParams &params, const config::Main &main_config) :
    _path(path),
    _resources_path(io::path_join(_path, params.resources_path())),
    _model_path(io::path_join(_path, params.model_path())),
    _log(nullptr),
    _logger(nullptr)
  {
  // Validate or create the top-level model directory.
  if (io::path_exists(_path)) {
    if (!io::path_is_dir(_path)) {
      std::ostringstream ss;
      ss << "Specified model directory '" << _path << "' already exists but is not a directory";
      throw IOException(ss.str());
    }
  }
  else
    io::mkdir(_path);

  // Write the command-line output file.
  {
    io::OutputStream out(io::path_join(_path, params.command_line_path()));
    main_config.serialise_cmdline_args(out);
    *out << std::endl;
  }

  // Write the model params config output file.
  {
    io::OutputStream out(io::path_join(_path, params.config_path()));
    params.serialise_all(out);
  }

  // Open the log path and create a logger over it.
  _log = new io::OutputStream(io::path_join(_path, params.log_path()));
  _logger = new io::PrettyLogger(*_log);
  _logger->threshold(params.log_level.level());

  // Validate or create the resources directory.
  if (io::path_exists(_resources_path)) {
    if (!io::path_is_dir(_resources_path)) {
      std::ostringstream ss;
      ss << "Specified model resources directory '" << _resources_path << "' already exists but is not a directory";
      throw IOException(ss.str());
    }
  }
  else {
    LOG2(INFO, logger()) << "Creating resources directory '" << _resources_path << "'" << std::endl;
    io::mkdir(_resources_path);
  }
}

OutputModel::~OutputModel(void) {
  delete _logger;
  delete _log;
}

}  // namespace learn
}  // namespace schwa
