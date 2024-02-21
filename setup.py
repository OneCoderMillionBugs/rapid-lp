from setuptools import Extension, setup

setup(
    name = 'rapidlp',
    version = '0.1',
    description = 'This is a demo package',
    ext_modules=[
        Extension('rapidlp',
                  sources=['rapidlp.c', 'utf8_string.c', 'uint_list.c']),
    ]
)