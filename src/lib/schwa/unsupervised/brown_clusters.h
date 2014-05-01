/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UNSUPERVISED_BROWN_CLUSTERS_H_
#define SCHWA_UNSUPERVISED_BROWN_CLUSTERS_H_

#include <iosfwd>

#include <schwa/_base.h>


namespace schwa {
  namespace unsupervised {

    class BrownClusterer {
    private:
      class Impl;
      Impl *_impl;

    public:
      BrownClusterer(void);
      ~BrownClusterer(void);

      void run(unsigned int nclusters, unsigned int nthreads, std::istream &input, std::ostream &output);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(BrownClusterer);
    };

  }  // namespace unsupervised
}  // namespace schwa

#endif  // SCHWA_UNSUPERVISED_BROWN_CLUSTERS_H_
