from setuptools import Extension, setup
import os
from pathlib import Path

cwd = os.getcwd()

def get_all_sources():
    src_dir = os.path.join(cwd, 'wind')
    all_cpp = list(Path(src_dir).rglob("*.cpp")) 
    all_headers = list(Path(src_dir).rglob("*.hpp")) + list(Path(src_dir).rglob("*.h")) 
    return [str(i) for i in all_cpp], [str(i) for i in all_headers]

all_cpp, all_headers = get_all_sources()
wind_ext = Extension(       
                        "wind._wind", 
                        sources = all_cpp, 
                        include_dirs = all_headers,
                        extra_compile_args = ["-O3", "-std=c++14", "-w", "-fpermissive"], 
                        language="c++", 
                        # extra_link_args=['callLib.a'],
                    )
#

setup(  
        name='Wind',
        version = '0.0.1',
        author = 'Touqir Sajed',
        author_email = 'shuhash6@gmail.com',
        description = 'Wind - a high performance Python http server library',
        license = 'MIT',
        # url = 'https://github.com/touqir14/MaxFlow',
        packages = ['wind'],
        ext_modules = [wind_ext],
        # install_requires=['scipy>=0.17.0', 'numpy>=1.13.0'],      
    )