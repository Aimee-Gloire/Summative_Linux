from setuptools import setup, Extension

module = Extension(
    "temp_ext",
    sources=["temp_ext.c"],
)

setup(
    name="temp_ext",
    version="1.0",
    description="Temperature statistics C extension",
    ext_modules=[module],
)
