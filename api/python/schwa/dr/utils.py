# vim: set ts=2 et:
__all__ = ['pluralise', 'to_lower']

VOWELS = set('aeiou')
CONSONANTS = set('bcdfghjklmnpqrstvwxyz')


def to_lower(name):
  word = []
  for i, c in enumerate(name):
    if c.isupper() and i != 0:
      word.append('_')
    word.append(c.lower())
  return ''.join(word)


def pluralise(name):
  word = to_lower(name)
  if word.endswith('s') or word.endswith('ch') or word.endswith('sh'):
    return word + 'es'
  elif len(word) >= 2 and (word[-1] == 'o' and word[-2] in CONSONANTS):
    return word + 'es'
  elif len(word) > 3 and (word[-1] == 'y' and word[-2] in CONSONANTS):
    return word[:-1] + 'ies'
  else:
    return word + 's'
