from setuptools import setup, Extension


setup(
    name='threadfun',
    ext_modules=[
        Extension('threadfun', sources=['src/threadfun.c']),
    ],
)
