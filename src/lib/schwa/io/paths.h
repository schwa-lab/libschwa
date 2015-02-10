/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_PATHS_H_
#define SCHWA_IO_PATHS_H_

#include <string>
#include <vector>

#include <schwa/_base.h>


namespace schwa {
  namespace io {

    /**
     * Returns the absolute path to the currently running executable. This path will include the
     * executable itself. Use path_dirname on the result to obtain just the directory to argv0.
     *
     * See http://stackoverflow.com/a/1024937 for the system-dependant ways to obtain this.
     **/
    std::string abspath_to_argv0(void);

    /**
     * Populates \p paths with the set of paths found in the environment variable
     * named \p env_var. Paths are assumed to be separated by ":".
     **/
    void get_env_paths(std::vector<std::string> &paths, const char *env_var="PATH");

    /**
     * Creates a directory at \p path. An IOException with an appropriate error message is thrown
     * if this action is not able to be completed.
     **/
    void mkdir(const std::string &path);

    /**
     * Returns the leaf target of the provided \p path.
     **/
    std::string path_basename(const std::string &path);

    /**
     * Returns the directory of the provided \p path.
     **/
    std::string path_dirname(const std::string &path);

    /**
     * Returns whether or not the path exists. An IOException with an appropriate error message is
     * thrown when this cannot be determined.
     **/
    bool path_exists(const std::string &path);

    /**
     * Returns whether or not the path exists and is a directory. An IOException with an
     * appropriate error message is thrown when this cannot be determined.
     **/
    bool path_is_dir(const std::string &path);

    /**
     * Returns whether or not the path exists and is a file. This function will follow symlinks
     * to their target. That is, if the target of the symlink is a regular file, this function will
     * return true. An IOException with an appropriate error message is thrown when this cannot be
     * determined.
     **/
    bool path_is_file(const std::string &path);

    /**
     * Returns paths \p a and \p b joined together.
     **/
    std::string path_join(const std::string &a, const std::string &b);

  }
}

#endif  // SCHWA_IO_PATHS_H_
