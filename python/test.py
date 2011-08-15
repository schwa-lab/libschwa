from schwa.token import *

raw = open('test.txt').read()
tokenizer = Tokenizer()

toks1 = tokenizer.tokenize(raw)
toks2 = tokenizer.tokenize(filename='test.txt')
toks3 = tokenizer.tokenize(open('test.txt'))

class Callback(object):
    def add(self, *args):
        print args

    def error(self, *args):
        print args

    def begin_document(self): print "begin document"
    def end_document(self): print "end document"

    def begin_paragraph(self): print "begin paragraph"
    def end_paragraph(self): print "end paragraph"

    def begin_sentence(self): print "begin sentence"
    def end_sentence(self): print "end sentence"

    def begin_heading(self, level): print "begin heading", level
    def end_heading(self, level): print "end heading", level

    def begin_list(self): print "begin list"
    def end_list(self): print "end list"

    def begin_item(self): print "begin item"
    def end_item(self): print "end item"

callback = Callback()
