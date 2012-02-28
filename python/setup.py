# -*- coding: utf-8 -*-

from distutils.core import setup, Extension

token = Extension('schwa.token',
                  language = 'c++',
                  define_macros = [('MAJOR_VERSION', '1'),
                                   ('MINOR_VERSION', '0')],
                  include_dirs = ['/usr/local/include'],
                  libraries = ['boost_iostreams'],
                  library_dirs = ['/usr/local/lib'],
                  sources = ['pytoken.cc', '../tokenizer.cc'])

setup(name = 'token',
      version = '1.0',
      description = 'ə-lab tokenizer',
      author = 'James R. Curran',
      author_email = 'james.r.curran@sydney.edu.au',
      url = 'http://schwa.org/tokenizer',
      long_description = '''package for tokenizing raw text in Python''',
      packages = ['schwa'],
      ext_modules = [token],
)
