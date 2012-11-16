/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace tokenizer {

    static const unsigned int MAX_SENTENCE = 256;

    struct Tokenizer::State {
    public:
      int cs; // current state of the FSA
      const char *ts; // beginning of the current match (first character)
      const char *te; // end of the current match (one past last char)
      int act; // last pattern matched (for back tracking)

      const char *offset; // beginning of the current buffer + bytes consumed

      const char *n1;
      const char *n2;
      int suffix;

      bool in_document;
      bool in_heading;
      bool in_paragraph;
      bool in_list;
      bool in_item;
      bool in_sentence;

      bool seen_terminator;
      bool in_double_quotes;
      bool in_single_quotes;

      unsigned int sentence_length = 0; // Need to track word count so we can split on long sentences.

      State(void) : cs(0), ts(0), te(0), act(0), offset(0), n1(0), n2(0), suffix(0), in_document(false), in_heading(false), in_paragraph(false), in_list(false), in_item(false), in_sentence(false), seen_terminator(false), in_double_quotes(false), in_single_quotes(false) { }

      void
      error(Stream &dest) {
        dest.error(ts, ts - offset, te - ts);
      }

      void
      check_sentence_length(Stream &dest) {
        sentence_length++;
        if (sentence_length >= MAX_SENTENCE) {
          // Split on long sentences
          end_sentence(dest);
	  ensure_sentence(dest);
        }
      }

      void
      add(Type type, Stream &dest, const char *norm=0) {
        dest.add(type, ts, ts - offset, te - ts, norm ? norm : n1);
        check_sentence_length(dest);
        n1 = n2 = 0;
        suffix = 0;
      }

      void
      split(Type type1, Type type2, Stream &dest, const char *norm1=0, const char *norm2=0) {
        const char *split = te - suffix;
        dest.add(type1, ts, ts - offset, split - ts, norm1 ? norm1 : n1);
        check_sentence_length(dest);
        dest.add(type2, split, split - offset, te - split, norm2 ? norm2 : n2);
        check_sentence_length(dest);
        n1 = n2 = 0;
        suffix = 0;
      }

      void
      flush_sentence(Stream &dest) {
        if (seen_terminator)
          end_sentence(dest);
        seen_terminator = false;
      }

      void
      begin_document(Stream &dest) {
        in_document = true;
        dest.begin_document();

        in_double_quotes = false;
        in_single_quotes = false;

        seen_terminator = false;

        in_item = false;
        in_list = false;
        in_heading = false;
        in_paragraph = false;
        in_sentence = false;
      }

      void
      begin_heading(Stream &dest, int depth) {
        end_paragraph(dest);

        in_heading = true;
        in_single_quotes = false;
        in_double_quotes = false;

        dest.begin_heading(depth);
      }

      void
      begin_paragraph(Stream &dest) {
        end_paragraph(dest);

        in_paragraph = true;
        in_single_quotes = false;
        in_double_quotes = false;

        dest.begin_paragraph();
      }

      void
      ensure_paragraph(Stream &dest) {
        if (in_paragraph)
          return;
        begin_paragraph(dest);
      }

      void
      begin_sentence(Stream &dest) {
        if (!in_heading && !in_list)
          ensure_paragraph(dest);

        in_sentence = true;
        seen_terminator = false;
        dest.begin_sentence();
      }

      void
      ensure_sentence(Stream &dest) {
        if (in_sentence)
          return;
        begin_sentence(dest);
      }

      void
      end_sentence(Stream &dest) {
        if (!in_sentence) 
          return;

        dest.end_sentence();
        seen_terminator = false;
        in_sentence = false;
        sentence_length = 0;
      }

      void
      end_paragraph(Stream &dest) {
        if (!in_paragraph)
          return;

        end_sentence(dest);
        dest.end_paragraph();
        in_single_quotes = false;
        in_double_quotes = false;
        in_paragraph = false;
      }

      void
      end_heading(Stream &dest, int depth) {
        if (!in_heading)
          return;

        end_sentence(dest);

        // FIXME: ensure consistent depth
        dest.end_heading(depth);

        in_single_quotes = false;
        in_double_quotes = false;
        in_heading = false;
      }

      void
      end_document(Stream &dest) {
        end_paragraph(dest);
        end_list(dest);
        dest.end_document();

        in_document = false;
      }

      void
      begin_list(Stream &dest) {
        end_list(dest);

        in_list = true;
        in_single_quotes = false;
        in_double_quotes = false;

        dest.begin_list();
      }

      void
      ensure_list(Stream &dest) {
        if (in_list)
          return;
        begin_list(dest);
      }

      void
      end_list(Stream &dest) {
        if (!in_list)
          return;

        end_item(dest);
        dest.end_list();

        in_single_quotes = false;
        in_double_quotes = false;
        in_list = false;
      }

      void
      begin_item(Stream &dest) {
        end_item(dest);
        ensure_list(dest);

        in_item = true;
        dest.begin_item();
      }

      void
      ensure_item(Stream &dest) {
        if (in_item)
          return;
        begin_list(dest);
      }

      void
      end_item(Stream &dest) {
        if (!in_item)
          return;

        end_sentence(dest);
        dest.end_item();
        in_item = false;
      }
    };

  }
}
