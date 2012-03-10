#!/usr/bin/env python
import docrep

#class Sentence(docrep.Annotation):
  #token_begin = docrep.Pointer('Token', sname='begin')
  #token_end   = docrep.Pointer('Token', sname='end')


#class Token(docrep.Token):
  #pos  = docrep.Field(sname='gold_pos')


#class Document(docrep.Document):
  #tokens    = docrep.Annotations('Token')
  #sentences = docrep.Annotations('Sentence', sname='CoNLL03Sentence')

  #def token_range(self, start, finish):
    #for i in xrange(start._dr_index, finish._dr_index + 1):
      #yield self.tokens[i]


with open('/n/schwafs/home/tim/repos/docrep/api/python/eng.testa.docrep', 'rb') as f:
  reader = docrep.Reader()
  for doc in reader.stream(f):
    print doc.filename, len(doc.tokens), len(doc.sentences)
    #for sent in doc.sentences:
      #print list(doc.token_range(sent.token_begin, sent.token_end))
    #print

