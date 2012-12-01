from setuptools import setup, Extension


nfp_module = Extension('_nfp',
                           sources=['../core/Rating.cpp',],
                           )

setup (name = 'nfp',
       # version = '0.1',
       # author      = "SWIG Docs",
       # description = """Simple swig example from docs""",
       ext_modules = [nfp_module],
       py_modules = ["nfp"],
       )
