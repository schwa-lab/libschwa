namespace schwa {
  namespace config {

    template <typename T>
    std::istream &
    operator >>(std::istream &is, std::vector<T> &vec);

    template <typename T>
    std::ostream &
    operator <<(std::ostream &os, const std::vector<T> &vec);


    // ========================================================================
    // Template implementations below
    // ========================================================================

    template <typename T>
    std::istream &
    operator >>(std::istream &is, std::vector<T> &vec) {
      T temp;
      char delimiter;
      bool comma_delimited = false;

      vec.resize(0);

      if (!(is >> temp)) {
        if (is.eof() && is.fail())
          is.clear(std::ios::eofbit);
        return is; //first token must be of type T
      }
      vec.push_back(temp);

      if (!(is >> delimiter)) {
        is.clear(std::ios::eofbit);
        return is;
      }

      if (delimiter == ',')
        comma_delimited = true;
      else
        is.unget();

      if (!(is>>temp))
        return is;

      vec.push_back(temp);

      do {
        if (comma_delimited) {
          if (!(is >> delimiter)) {
            is.clear(std::ios::eofbit);
            return is;
          }
          if (delimiter!=',') {
            return is;
          }
        }

        if (!(is >> temp)) {
          if (!comma_delimited && is.eof())
            is.clear(std::ios::eofbit);
          return is;
        }
        vec.push_back(temp);
      }
      while(!is.eof());

      return is;
    }


    template <typename T>
    std::ostream &
    operator <<(std::ostream &os, const std::vector<T> &vec){
      if (vec.size() != 0) {
        os << vec[0];
        for(size_t i = 1; i != vec.size(); ++i)
          os << ' ' << vec[i];
      }
      return os;
    }

  }
}
