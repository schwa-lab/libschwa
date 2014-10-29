#!/usr/bin/env python3
import json
import os
import sys
import urllib.request

SCRIPT_PATH = None
URL = 'http://www.w3.org/html/wg/drafts/html/master/entities.json'


def main(file, url):
  data = urllib.request.urlopen(url).read().decode('utf-8')
  data = json.loads(data)

  print(r'''
// Generated by: {0}
#include <schwa/formats/html.h>

namespace schwa {{
namespace formats {{

const std::unordered_map<std::string, unicode_t> HTMLLexer::NAMED_CHARACTER_REFERENCES = {{
'''.format(SCRIPT_PATH).strip(), file=file)
  for name, unicode_data in sorted(data.items()):
    code_point = unicode_data['codepoints'][0]
    print('    {{"{0}", {1}}},'.format(name[1:-1], hex(code_point)), file=file)
  print(r'''
};

}  // namespace formats
}  // namespace schwa
'''.strip(), file=file)


if __name__ == '__main__':
  path = os.path.normpath(os.path.dirname(__file__))
  if path != 'src/scripts':
    print('Error: This script must be run from the top-level directory of libschwa.', file=sys.stderr)
    sys.exit(1)

  SCRIPT_PATH = os.path.join(path, os.path.basename(__file__))

  url = URL if len(sys.argv) == 1 else sys.argv[1]
  with open('src/lib/schwa/formats/html_named_character_references_gen.cc', 'w') as f:
    main(f, url)