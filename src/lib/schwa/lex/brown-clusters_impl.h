/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEX_BROWN_CLUSTERS_IMPL_H_
#define SCHWA_LEX_BROWN_CLUSTERS_IMPL_H_


namespace schwa {
  namespace lex {

    template <typename IT>
    inline void
    BrownClusters::set_path_lengths(IT begin, IT end) {
      _path_lengths.assign(begin, end);
    }

  }
}

#endif  // SCHWA_LEX_BROWN_CLUSTERS_IMPL_H_
