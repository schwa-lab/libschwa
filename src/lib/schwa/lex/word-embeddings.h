/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEX_WORD_EMBEDDINGS_H_
#define SCHWA_LEX_WORD_EMBEDDINGS_H_

#include <iosfwd>
#include <string>
#include <unordered_map>

#include <schwa/_base.h>
#include <schwa/utils/hash.h>
#include <schwa/utils/string-pool.h>


namespace schwa {
  namespace lex {

    /**
     * The word embeddings file is expected to be a MessagePack payload of the format
     * { token_str : [ embedding_double ] }, where all of the arrays of floating point values are
     * the same length.
     **/
    class WordEmbeddings {
    public:
      static const double DEFAULT_SIGMA;
      static const std::string UNKNOWN_WORD_KEY;

    private:
      StringPool &_string_pool;
      double *_embeddings;           //<! Pointer to the giant array of all embedding values.
      double *_unknown_embeddings;   //!< Cached copy of the pointer to the "*UNKNOWN*" embeddings.
      double _sigma;                 //!< Scaling hyperparameter.
      uint32_t _ndimensions;         //!< The number of dimensions.
      std::unordered_map<const uint8_t *, const double *, uint8str_hash, uint8str_equal_to> _map;

    public:
      explicit WordEmbeddings(StringPool &string_pool, double sigma=DEFAULT_SIGMA);
      ~WordEmbeddings(void);

      /**
       * Populate this WordEmbeddings instance with the embeddings contained within the input
       * stream. The embeddings should be in the format described in the documentation of this
       * class. The WordEmbeddings inststance can only be loaded once, so a ValueException will be
       * thrown if it attempts to be loaded more than once.
       **/
      void load(std::istream &in);

      /**
       * Get the embeddings for a given word. If embeddings are found for the word, this method
       * returns a pointer to the embeddings. The dimensionality of the embeddings can be obtained
       * via the WordEmbeddings::ndimensions method. If no embeddings are found (should not happen
       * assuming there is an "*UNKNOWN*" embedding), this method returns nullptr.
       **/
      const double *get_embeddings(const std::string &word) const;

      inline bool empty(void) const { return _map.empty(); }
      inline uint32_t ndimensions(void) const { return _ndimensions; }
      inline size_t size(void) const { return _map.size(); }
      inline StringPool &string_pool(void) { return _string_pool; }
      inline const StringPool &string_pool(void) const { return _string_pool; }
    };

  }
}

#endif  // SCHWA_LEX_WORD_EMBEDDINGS_H_
