import os
from distutils.core import setup, Extension

ext = Extension(
            'pypuzzle',
            sources=[
                os.path.join('src', 'pypuzzle.c')
            ],
            libraries=['puzzle', 'gd']
        )

setup (
    name='pypuzzle',
    version='1.0',
    description='PyPuzzle -- A Python binding for libpuzzle',
    author='Archangel_SDY',
    author_email='Archangel.SDY@gmail.com',
    url='https://github.com/ArchangelSDY/PyPuzzle',
    classifiers=[
        'Programming Language :: C',
        'Operating System :: POSIX :: Linux',
        'License :: OSI Approved :: MIT License',
        'Topic :: Multimedia :: Graphics',
        'Development Status :: 4 - Beta'
    ],
    ext_modules=[ext],
    long_description="""This module provides Python bindings for libpuzzle."""
)
