/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MACROS_H_
#define SCHWA_MACROS_H_

/**
 * A macro to disallow the copy constructor and operator= functions.
 * This should be used in the private: declarations for a class.
 **/
#define SCHWA_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &) = delete;   \
    void operator =(const TypeName &) = delete


/**
 *
 **/
#define SCHWA_MAIN(cfg, fn) \
  try { \
    fn(); \
  } \
  catch (schwa::ConfigException &e) { \
    std::cerr << schwa::print_exception("ConfigException", e) << std::endl; \
    cfg.help(std::cerr); \
    return 1; \
  } \
  catch (schwa::Exception &e) { \
    std::cerr << schwa::print_exception(e) << std::endl; \
    return 1; \
  } \
  catch (schwa::SystemExit &e) { \
    return e.exit_code(); \
  }

#endif  // SCHWA_MACROS_H_
