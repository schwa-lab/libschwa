/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEX_BROWN_CLUSTERS_H_
#define SCHWA_LEX_BROWN_CLUSTERS_H_

#include <array>
#include <string>
#include <vector>

#include <schwa/_base.h>
#include <schwa/utils/hash.h>
#include <schwa/utils/string-pool.h>


namespace schwa {
  namespace lex {

    /**
     * The Brown clusters file is expected to be a MessagePack payload of the format
     * { path_str : { token_str : freq } }, where the frequency is an unsigned integer value,
     * indicating how many times this token appeared in the data used to create the Brown clusters.
     **/
    class BrownClusters {
    public:
      static constexpr const unsigned int DEFAULT_MIN_FREQ = 5;
      static const std::array<unsigned int, 4> DEFAULT_PATH_LENGTHS;

    private:
      StringPool &_string_pool;
      unsigned int _min_freq;
      std::vector<unsigned int> _path_lengths;
      std::unordered_map<const uint8_t *, const uint8_t *, uint8str_hash, uint8str_equal_to> _map;

    public:
      explicit BrownClusters(StringPool &string_pool, unsigned int min_freq=DEFAULT_MIN_FREQ);
      ~BrownClusters(void);

      /**
       * Populate this BrownClusters instance with the clusters contained within the input stream.
       * The clusters should be in the format described in the documentation of this class. The
       * BrownClusters inststance can only be loaded once, so a ValueException will be thrown if it
       * attempts to be loaded more than once.
       **/
      void load(std::istream &in);

      void set_min_freq(const unsigned int min_freq);

      template <typename IT>
      void set_path_lengths(IT begin, IT end);

      /**
       * Get the Brown cluster paths of each length for the given word. The \p path pointer will be
       * populated with the path strnig, and the \p lengths pointer will be populated with the
       * lengths of the path to be used. The number of elements written to the \p lengths array is
       * returned by this method. THe \p lengths pointer must be long enough to store up to
       * BrownClusters::npaths items.
       **/
      size_t get_paths(const std::string &word, const uint8_t **path, unsigned int *lengths) const;

      inline bool empty(void) const { return _map.empty(); }
      inline unsigned int min_freq(void) const { return _min_freq; }
      inline size_t npaths(void) const { return _path_lengths.size(); }
      inline const std::vector<unsigned int> &path_lengths(void) const { return _path_lengths; }
      inline size_t size(void) const { return _map.size(); }
      inline StringPool &string_pool(void) { return _string_pool; }
      inline const StringPool &string_pool(void) const { return _string_pool; }
    };

  }
}

#include <schwa/lex/brown-clusters_impl.h>

#endif  // SCHWA_LEX_BROWN_CLUSTERS_H_
