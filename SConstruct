import os
from os.path import join, dirname
import shutil

LIBS=['raylib', 'opengl32', 'gdi32', 'winmm']
LIBPATH='./lib'
# CCFLAGS='-static --target=x86_64-w64-windows-gnu'
# CXXFLAGS='-static --target=x86_64-w64-windows-gnu -std=c++17'
# LINKFLAGS = '--target=x86_64-w64-windows-gnu -static-libgcc -static-libstdc++'
CCFLAGS='--target=x86_64-w64-windows-gnu -Wno-unknown-warning-option'
CXXFLAGS='--target=x86_64-w64-windows-gnu -std=c++17 -Wno-unknown-warning-option'
LINKFLAGS = '--target=x86_64-w64-windows-gnu'

VariantDir('build', 'src', duplicate=False)
env = Environment(
	CC='clang',
	CXX='clang++',
	CPPPATH=['./include'],
	tools = ['mingw'],
	ENV = {'PATH' : os.environ['PATH']},
	LIBS=LIBS,
	LIBPATH=LIBPATH,
	CCFLAGS=CCFLAGS,
	CXXFLAGS=CXXFLAGS,
	LINKFLAGS=LINKFLAGS
)
env['ENV']['TERM'] = os.environ['TERM'] # Colored output

source = [ Glob('build/*.cc') ]
env.Program('bin/vamp.exe', source)
