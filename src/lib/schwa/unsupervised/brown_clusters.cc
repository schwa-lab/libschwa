/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unsupervised/brown_clusters.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <schwa/io/logging.h>
#include <schwa/port.h>
#include <schwa/utils/counter.h>
#include <schwa/unicode.h>


namespace schwa {
namespace unsupervised {

// ============================================================================
// BrownClusterer::Impl
// ============================================================================
class BrownClusterer::Impl {
public:
  using count_type = unsigned int;
  using id_type = unsigned int;
  using float_type = double;

private:
  unsigned int _nclusters;
  count_type _ntokens;
  std::unordered_map<std::string, id_type> _tokens;
  UnigramCounter<id_type, count_type> _unigram_counts;
  BigramCounter<id_type, id_type, count_type> _bigram_counts;

  unsigned int _membership_nbytes;
  unsigned int _array_dim;  // _nclusters + 1
  uint8_t *_clusters;  // 1D. Size _array_dim * _membership_nbytes
  float_type *_w;      // 1D. Size _array_dim * _array_dim
  float_type *_L;      // 1D. Size _array_dim * _array_dim

  inline uint8_t *_get_cluster(const unsigned int c) { return &_clusters[c*_membership_nbytes]; }
  inline const uint8_t *_get_cluster(const unsigned int c) const { return &_clusters[c*_membership_nbytes]; }

  void _initialise(unsigned int nclusters);
  bool _keep_token(const std::string &token) const;
  void _read_input(std::istream &input);
  void _run_fixed_window(void);
  void _run_hierarchical(void);
  void _write_clusters(std::ostream &out) const;

  void _add_member_to_cluster(unsigned int c, id_type token);
  void _merge_cluster_members(unsigned int c1, unsigned int c2, uint8_t *result) const;

  void _initialise_w(void);
  void _initialise_L(void);
  float_type _compute_w(const uint8_t *cluster1, const uint8_t *cluster2) const;
  float_type _compute_L(unsigned int c1, unsigned int c2, uint8_t *merged_cluster) const;
  float_type _compute_P_c(const uint8_t *cluster) const;
  float_type _compute_P_c1_c2(const uint8_t *cluster1, const uint8_t *cluster2) const;
  inline float_type _compute_w(unsigned int c1, unsigned int c2) const { return _compute_w(_get_cluster(c1), _get_cluster(c2)); }
  inline float_type _compute_P_c(unsigned int c) const { return _compute_P_c(_get_cluster(c)); }
  inline float_type _compute_P_c1_c2(unsigned int c1, unsigned int c2) const { return _compute_P_c1_c2(_get_cluster(c1), _get_cluster(c2)); }

  void _dump_w(void) const;
  void _dump_w(unsigned int hl1, unsigned int hl2) const;

public:
  Impl(void);
  ~Impl(void);

  void run(unsigned int nclusters, std::istream &input, std::ostream &output);

private:
  SCHWA_DISALLOW_COPY_AND_ASSIGN(Impl);
};


BrownClusterer::Impl::Impl(void) : _clusters(nullptr), _w(nullptr), _L(nullptr) { }

BrownClusterer::Impl::~Impl(void) {
  delete [] _clusters;
  delete [] _w;
  delete [] _L;
}


void
BrownClusterer::Impl::_initialise(const unsigned int nclusters) {
  // Don't try and form more clusters than there are unique tokens.
  _nclusters = std::min(nclusters, static_cast<decltype(nclusters)>(_tokens.size()));
  _array_dim = _nclusters + 1;

  // Work out the number of bytes needed in out membership bitmask.
  const auto result = std::div(_tokens.size(), 8);
  _membership_nbytes = result.quot + (result.rem == 0 ? 0 : 1);

  // Allocate memory for the cluster membership information.
  delete [] _clusters;
  _clusters = new uint8_t[_array_dim*_membership_nbytes];
  std::memset(_clusters, 0, _array_dim*_membership_nbytes);

  // Allocate memory for the weights.
  delete [] _w;
  _w = new float_type[_array_dim*_array_dim];
  delete [] _L;
  _L = new float_type[_array_dim*_array_dim];

  LOG(INFO) << "Tokens read: " << _ntokens << std::endl;
  LOG(INFO) << "Distinct unigrams: " << _unigram_counts.size() << std::endl;
  LOG(INFO) << "Distinct bigrams: " << _bigram_counts.size() << std::endl;
  LOG(INFO) << "Clusters to form: " << _nclusters << std::endl;
  LOG(INFO) << "Membership bytes per cluster: " << _membership_nbytes << std::endl;
}


bool
BrownClusterer::Impl::_keep_token(const std::string &token) const {
  UTF8Decoder d(token);
  if (unicode::is_upper(*d.begin()))
    return true;
  else if (token == "and" || token == "of" || token == "'")
    return true;
  else
    return false;
}


void
BrownClusterer::Impl::_read_input(std::istream &input) {
  LOG(DEBUG) << "Reading input ..." << std::endl;
  // Reset state.
  _tokens.clear();
  _unigram_counts.clear();
  _bigram_counts.clear();

  // Read each sentence from the istream.
  id_type tid, prev_tid = 0;
  std::string line, token;
  std::stringstream buffer;
  for (_ntokens = 0; std::getline(input, line); ) {
    // Reset local state.
    buffer.str("");
    buffer.clear();

    // Read each token in the line.
    bool prev_tid_valid = false;
    buffer << line;
    for (unsigned int i = 0; buffer >> token; ++i, ++_ntokens) {
      if (!_keep_token(token)) {
        prev_tid_valid = false;
        continue;
      }

      // Convert the token into a unique token identifier.
      const auto it = _tokens.find(token);
      if (it == _tokens.end()) {
        tid = _tokens.size();
        _tokens.emplace(token, tid);
      }
      else
        tid = it->second;

      // Update the unigram and bigram counts.
      ++_unigram_counts(tid);
      if (prev_tid_valid)
        ++_bigram_counts(prev_tid, tid);
      prev_tid = tid;
      prev_tid_valid = true;
    }
  }
  LOG(DEBUG) << "Done" << std::endl;
}


void
BrownClusterer::Impl::_run_fixed_window(void) {
  // Order the tokens in descending order by frequency.
  using pair_type = std::pair<id_type, count_type>;
  std::vector<pair_type> ordered_tokens(_unigram_counts.begin(), _unigram_counts.end());
  std::sort(ordered_tokens.begin(), ordered_tokens.end(), [](const pair_type &a, const pair_type &b) { return a.second > b.second; });

  // Place the first `_nclusters` most frequent tokens into their own clusters.
  for (unsigned int i = 0; i != _nclusters; ++i)
    _add_member_to_cluster(i, ordered_tokens[i].first);

  // Initialise the w and L tables.
  _initialise_w();
  _initialise_L();

  // Bail early if we don't need to do any clustering.
  if (_nclusters == _tokens.size())
    return;

  // Place the remaining `_ntokens - _nclusters` tokens into a new cluster, and merge it.
  std::unique_ptr<uint8_t> merged(new uint8_t[_membership_nbytes]);
  for (unsigned int i = _nclusters; i != _tokens.size(); ++i) {
    LOG(INFO) << "Inserting token " << i << "/" << _tokens.size() << " into the fixed window" << std::endl;
    // Set the current token as the only member of the last cluster.
    std::memset(_get_cluster(_nclusters), 0, _membership_nbytes);
    _add_member_to_cluster(_nclusters, ordered_tokens[i].first);

    // Update the row and column of w to do with this cluster.
    for (unsigned int j = 0; j != _nclusters + 1; ++j) {
      const uint8_t *cluster1 = _get_cluster(j);
      const uint8_t *cluster2 = _get_cluster(_nclusters);
      _w[j*_array_dim + _nclusters] = _w[_nclusters*_array_dim + j] = _compute_w(cluster1, cluster2);
    }

    // Update the column of L to do with this cluster.
    for (unsigned int j = 0; j != _nclusters + 1; ++j)
      _L[j*_array_dim + _nclusters] = _compute_L(j, _nclusters, merged.get());

    // Find the maximum value in L.
    unsigned int max_c1 = 0, max_c2 = 0;
    float_type max_value = std::numeric_limits<float_type>::lowest();
    for (unsigned int c1 = 0; c1 != _nclusters + 1; ++c1) {
      for (unsigned int c2 = c1 + 1; c2 != _nclusters + 1; ++c2) {
        const float_type value = _L[c1*_array_dim + c2];
        if (value > max_value) {
          max_c1 = c1;
          max_c2 = c2;
          max_value = value;
        }
      }
    }

    LOG(DEBUG) << "Maximum value " << max_value << " occurs by merging " << max_c1 << " and " << max_c2 << std::endl;
    assert(!(max_c1 == 0 && max_c2 == 0));

    // Merge the members of clusters max_c1 and max_c2 into max_c1.
    _merge_cluster_members(max_c1, max_c2, _get_cluster(max_c1));

    // Move across the cluster membership values.
    //for (unsigned int c = max_c2 + 1; c != _nclusters + 1; ++c)
      //std::memcpy(_get_cluster(c - 1), _get_cluster(c), _membership_nbytes);
    //std::memset(_get_cluster(_nclusters), 0, _membership_nbytes);
    assert((_nclusters + 1 - (max_c2 + 1))*_membership_nbytes >= 0);
    std::memmove(_get_cluster(max_c2), _get_cluster(max_c2 + 1), (_nclusters + 1 - (max_c2 + 1))*_membership_nbytes);

    // Move the values in w that do not need to be recomputed but have changed position after the cluster merge.
    for (unsigned int row = max_c2 + 1; row != _nclusters + 1; ++row)
      for (unsigned int col = 0; col != _nclusters + 1; ++col)
        _w[(row - 1)*_array_dim + col] = _w[row*_array_dim + col];
    for (unsigned int col = max_c2 + 1; col != _nclusters + 1; ++col)
      for (unsigned int row = 0; row != _nclusters + 1; ++row)
        _w[row*_array_dim + col - 1] = _w[row*_array_dim + col];

    // Recompute the merged entries in w.
    for (unsigned int c = 0; c != _nclusters; ++c)
      _w[c*_array_dim + max_c1] = _w[max_c1*_array_dim + c] = _compute_w(c, max_c1);

    // Update the column of L to do with the merged cluster.
    for (unsigned int j = 0; j != _nclusters; ++j)
      _L[j*_array_dim + max_c1] = _compute_L(j, max_c1, merged.get());

    //_write_clusters(std::cerr);
  }
}


void
BrownClusterer::Impl::_run_hierarchical(void) {
  // TODO
}


void
BrownClusterer::Impl::_dump_w(void) const {
  for (unsigned int c1 = 0; c1 != _array_dim; ++c1) {
    for (unsigned int c2 = 0; c2 != _array_dim; ++c2) {
      if (c2 != 0)
        std::printf(" ");
      std::printf("%6.3f", _w[c1*_array_dim + c2]);
    }
    std::printf("\n");
  }
}


void
BrownClusterer::Impl::_dump_w(const unsigned int hl1, const unsigned int hl2) const {
  for (unsigned int c1 = 0; c1 != _array_dim; ++c1) {
    for (unsigned int c2 = 0; c2 != _array_dim; ++c2) {
      if (c2 != 0)
        std::printf(" ");
      if (c1 == hl1 || c2 == hl1)
        std::printf("%s", port::RED);
      else if (c1 == hl2 || c2 == hl2)
        std::printf("%s", port::DARK_GREY);
      std::printf("%6.3f", _w[c1*_array_dim + c2]);
      if (c1 == hl1 || c1 == hl2 || c2 == hl1 || c2 == hl2)
        std::printf("%s", port::OFF);
    }
    std::printf("\n");
  }
}


inline void
BrownClusterer::Impl::_add_member_to_cluster(const unsigned int c, const id_type token) {
  const auto d = std::div(token, 8);
  _get_cluster(c)[d.quot] |= 1 << d.rem;
}


inline void
BrownClusterer::Impl::_merge_cluster_members(const unsigned int c1, const unsigned int c2, uint8_t *result) const {
  const uint8_t *cluster1 = _get_cluster(c1);
  const uint8_t *cluster2 = _get_cluster(c2);
  for (unsigned int i = 0; i != _membership_nbytes; ++i)
    *result++ = *cluster1++ | *cluster2++;
}


void
BrownClusterer::Impl::_initialise_w(void) {
  LOG(INFO) << "Initialising w ..." << std::endl;
  for (unsigned int c1 = 0; c1 != _nclusters; ++c1) {
    for (unsigned int c2 = c1; c2 != _nclusters; ++c2)
      _w[c1*_array_dim + c2] = _w[c2*_array_dim + c1] = _compute_w(c1, c2);
    _w[c1*_array_dim + _nclusters] = _w[_nclusters*_array_dim + c1] = 0;
  }
  LOG(INFO) << "Initialising w ... Done" << std::endl;
}


void
BrownClusterer::Impl::_initialise_L(void) {
  LOG(INFO) << "Initialising L ..." << std::endl;
  std::unique_ptr<uint8_t> merged(new uint8_t[_membership_nbytes]);
  for (unsigned int c1 = 0; c1 != _nclusters; ++c1)
    for (unsigned int c2 = c1 + 1; c2 != _nclusters; ++c2)
      _L[c1*_array_dim + c2] = _compute_L(c1, c2, merged.get());
  LOG(INFO) << "Initialising L ... Done" << std::endl;
}


/**
 * Equation (4.4) from Liang, 2005.
 **/
auto
BrownClusterer::Impl::_compute_w(const uint8_t *cluster1, const uint8_t *cluster2) const -> float_type {
  float_type result, p_c1_c2;
  const float_type denominator = _compute_P_c(cluster1) * _compute_P_c(cluster2);

  result = 0;
  p_c1_c2 = _compute_P_c1_c2(cluster1, cluster2);
  if (std::fpclassify(p_c1_c2) != FP_ZERO)
    result = p_c1_c2 * std::log(p_c1_c2 / denominator);
  if (cluster1 != cluster2) {
    p_c1_c2 = _compute_P_c1_c2(cluster2, cluster1);
    if (std::fpclassify(p_c1_c2) != FP_ZERO)
      result += p_c1_c2 * std::log(p_c1_c2 / denominator);
  }
  return result;
}


/**
 * Equation (4.5) from Liang, 2005.
 **/
auto
BrownClusterer::Impl::_compute_L(const unsigned int c1, const unsigned int c2, uint8_t *merged_cluster) const -> float_type {
  // Merge c1 and c2 into merged_cluster.
  std::memset(merged_cluster, 0, _membership_nbytes);
  _merge_cluster_members(c1, c2, merged_cluster);

  float_type first = _compute_w(merged_cluster, merged_cluster);
  for (unsigned int d = 0; d != _nclusters + 1; ++d)
    if (d != c1 && d != c2)
      first += _compute_w(merged_cluster, _get_cluster(d));

  float_type second = 0;
  for (unsigned int d = 0; d != _nclusters + 1; ++d)
    second += _w[c1*_array_dim + d] + _w[c2*_array_dim + d];

  return first - second;
}


auto
BrownClusterer::Impl::_compute_P_c(const uint8_t *cluster) const -> float_type {
  count_type numerator = 0;
  for (unsigned int i = 0; i != _membership_nbytes; ++i, ++cluster)
    for (unsigned int b = 0; b != 8; ++b)
      if ((*cluster & (1 << b)) != 0)
        numerator += _unigram_counts(i*8 + b);
  return static_cast<float_type>(numerator) / _ntokens;
}


auto
BrownClusterer::Impl::_compute_P_c1_c2(const uint8_t *cluster1, const uint8_t *const cluster2) const -> float_type {
  count_type numerator = 0;
  for (unsigned int i1 = 0; i1 != _membership_nbytes; ++i1, ++cluster1) {
    for (unsigned int b1 = 0; b1 != 8; ++b1) {
      if ((*cluster1 & (1 << b1)) == 0)
        continue;
      const uint8_t *cluster2_copy = cluster2;
      for (unsigned int i2 = 0; i2 != _membership_nbytes; ++i2, ++cluster2_copy)
        for (unsigned int b2 = 0; b2 != 8; ++b2)
          if ((*cluster2_copy & (1 << b2)) != 0)
            numerator += _bigram_counts(i1*8 + b1, i2*8 + b2);
    }
  }
  return static_cast<float_type>(numerator) / _ntokens;
}


void
BrownClusterer::Impl::_write_clusters(std::ostream &output) const {
  // Construct the inverse mapping of token id to token string.
  std::vector<std::string> inv_tokens(_tokens.size());
  for (const auto &pair : _tokens)
    inv_tokens[pair.second] = pair.first;

  // For each cluster...
  for (unsigned int c = 0; c != _nclusters; ++c) {
    output << c;
    // For each member in the cluster, output its string representation.
    const uint8_t *cluster = _get_cluster(c);
    for (unsigned int i = 0; i != _membership_nbytes; ++i, ++cluster)
      for (unsigned int b = 0; b != 8; ++b)
        if ((*cluster & (1 << b)) != 0)
          output << " " << inv_tokens[i*8 + b] << "(" << (i*8 + b) << ")";
    output << std::endl;
  }
}


void
BrownClusterer::Impl::run(const unsigned int nclusters, std::istream &input, std::ostream &output) {
  _read_input(input);
  _initialise(nclusters);
  _run_fixed_window();
  _run_hierarchical();
  _write_clusters(output);
}


// ============================================================================
// BrownClusterer
// ============================================================================
BrownClusterer::BrownClusterer(void) : _impl(new Impl()) { }

BrownClusterer::~BrownClusterer(void) {
  delete _impl;
}

void
BrownClusterer::run(unsigned int nclusters, std::istream &input, std::ostream &output) {
  _impl->run(nclusters, input, output);
}

}  // namespace unsupervised
}  // namespace schwa
