"""
See these URLs for namespace packages information

* http://peak.telecommunity.com/DevCenter/setuptools#namespace-packages
* http://stackoverflow.com/questions/1675734/how-do-i-create-a-namespace-package-in-python
"""
from distutils.core import setup


setup(
  name='schwa-docrep',
  version='0.1.0',
  description='Schwa Lab Document Representation framework',
  author='Tim Dawborn',
  author_email='tim.dawborn@gmail.com',
  url='http://schwa.org/projects/docrep/',
  package_dir={'schwa.dr': 'api/python/schwa/dr'},
  packages=['schwa.dr'],
  namespace_packages=['schwa'],
  install_requires=['msgpack-python'],
)
