/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEX_CLARK_CLUSTERS_H_
#define SCHWA_LEX_CLARK_CLUSTERS_H_

#include <array>
#include <string>
#include <tuple>
#include <vector>

#include <schwa/_base.h>
#include <schwa/utils/hash.h>
#include <schwa/utils/string-pool.h>


namespace schwa {
  namespace lex {

    /**
     * The Clark clusters file is expected to be a MessagePack payload of the format
     * { token_str : [ cluster : prob ] }, where the cluster is an unsigned integer and the
     * probablility is a double which represents the conditional probability P(token|cluster).
     **/
    class ClarkClusters {
    private:
      StringPool &_string_pool;
      std::unordered_map<const uint8_t *, std::tuple<unsigned int, double>, uint8str_hash, uint8str_equal_to> _map;

    public:
      explicit ClarkClusters(StringPool &string_pool);
      ~ClarkClusters(void);

      /**
       * Populate this ClarkClusters instance with the clusters contained within the input stream.
       * The clusters should be in the format described in the documentation of this class. The
       * ClarkClusters inststance can only be loaded once, so a ValueException will be thrown if it
       * attempts to be loaded more than once.
       **/
      void load(std::istream &in);

      bool get_cluster(const std::string &word, unsigned int *cluster, double *prob) const;

      inline bool empty(void) const { return _map.empty(); }
      inline size_t size(void) const { return _map.size(); }
      inline StringPool &string_pool(void) { return _string_pool; }
      inline const StringPool &string_pool(void) const { return _string_pool; }
    };

  }
}

#endif  // SCHWA_LEX_CLARK_CLUSTERS_H_
