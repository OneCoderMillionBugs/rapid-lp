from distutils.core import setup, Extension

module1 = Extension('nlptools',
                    sources = ['nlptools.c', 'utf8_string.c', 'uint_list.c'])

setup (name = 'nlptools',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])