#!/usr/bin/env python
from schwa import dr

class Sentence(dr.Annotation):
  span2 = dr.Pointers('Token')
  span = dr.Range('Token')

class Token(dr.Token):
  pos = dr.Field(sname='gold_pos')
  ne  = dr.Field(sname='gold_ne')

class Foo(dr.Annotation):
  bar = dr.Field()

class Document(dr.Document):
  filename  = dr.Field()
  foo       = dr.Singleton('Foo')
  tokens    = dr.Annotations('Token')
  sentences = dr.Annotations('Sentence')


def read_pipefile(file, filename):
  upto = 0
  doc = Document(filename=filename)
  for i, line in enumerate(file):
    if line == '\n':
      if doc.tokens:
        yield doc
        doc = Document(filename=filename)
      upto += 1
    else:
      IN_WORD, IN_POS, IN_NE = range(3)
      state = IN_WORD
      begin = upto
      tokens = []
      buf = []
      word = pos = ne = None
      for c in line:
        upto += 1
        if c == '\n':
          assert state == IN_NE
          ne = ''.join(buf)
          tokens.append(Token(span=slice(begin, upto), raw=word, norm=word, pos=pos, ne=ne))
          doc.tokens += tokens
          b = doc.tokens.index(tokens[0])
          e = doc.tokens.index(tokens[-1])
          doc.sentences.append(Sentence(span=slice(b, e + 1), span2=[tokens[0], tokens[-1]]))
        elif c == ' ':
          assert state == IN_NE
          ne = ''.join(buf)
          buf = []
          state = IN_WORD
          tokens.append(Token(span=slice(begin, upto), raw=word, norm=word, pos=pos, ne=ne))
          begin = upto
        elif c == '|':
          if state == IN_WORD:
            word = ''.join(buf)
            buf = []
            state = IN_POS
          elif state == IN_POS:
            pos = ''.join(buf)
            buf = []
            state = IN_NE
          else:
            assert False
        else:
          buf.append(c)
  if doc.tokens:
    yield doc



if __name__ == '__main__':
  import sys
  if len(sys.argv) != 3:
    print >> sys.stderr, 'Usage: {0} <input> <output>'.format(sys.argv[0])
    sys.exit(1)

  with open(sys.argv[1], 'rU') as fin:
    with open(sys.argv[2], 'wb') as fout:
      writer = dr.Writer(fout)
      for doc in read_pipefile(fin, sys.argv[1]):
        #print doc.filename, len(doc.tokens), len(doc.sentences)
        doc.foo = Foo()
        doc.foo.bar = 42
        writer.write_doc(doc)

