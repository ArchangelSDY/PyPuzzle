from distutils.core import setup, Extension

module = Extension(
            'pypuzzle',
            sources=['pypuzzle.c'],
            libraries=['puzzle', 'gd']
        )

setup (
    name='pypuzzle',
    version='1.0',
    description='A python wrapper for libpuzzle.',
    ext_modules=[module]
)
