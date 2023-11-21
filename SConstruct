import os
from os.path import join, dirname
import shutil

# platform = 'x64-mingw-static'
platform = 'wasm32-emscripten'

# LIBS=['raylib', 'opengl32', 'gdi32', 'winmm']
LIBS = ['raylib']
# LIBPATH=['lib', f'vcpkg/installed/{platform}/lib']
# CPPPATH=['include', f'vcpkg/installed/{platform}/include', 'submodule/raylib-cpp/include', 'submodule/tileson']
LIBPATH=['lib']
CPPPATH=['include', 'submodule/raylib-cpp/include', 'submodule/tileson']

target = 'x86_64-w64-windows-gnu'
target = 'wasm32-unknown-emscripten'

# CCFLAGS = f'--target={target} -static -Wno-unknown-warning-option -Wunused-variable -Os'
# CXXFLAGS = f'--target={target} -static -std=c++17 -Wno-unknown-warning-option -Wunused-variable -Os'
CCFLAGS = f'--target={target} -static -Wno-unknown-warning-option -Wunused-variable -O3'
CXXFLAGS = f'--target={target} -static -std=c++17 -Wno-unknown-warning-option -Wunused-variable -O3 -Wno-unqualified-std-cast-call'
LINKFLAGS = f'--target={target} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --preload-file assets --preload-file config.cfg -s ALLOW_MEMORY_GROWTH --shell-file shell.html'
# TOOLS=['clang', 'clang++', 'gnulink']
TOOLS=['gcc','gnulink']

VariantDir('build', 'src', duplicate=False)
env = Environment(
	CC="emcc",
	CXX="em++",
	# TOOLS=TOOLS,
	CPPPATH=CPPPATH,
	ENV = {'PATH' : os.environ['PATH']},
	LIBS=LIBS,
	LIBPATH=LIBPATH,
	CXXFLAGS=CXXFLAGS,
	LINKFLAGS=LINKFLAGS,
	PROGSUFFIX='.html'
)
env['ENV']['TERM'] = os.environ['TERM'] # Colored output

source = [ Glob('build/*.cc') ]
# env.Program('bin/hemoglobin.exe', source)
env.Program('biogoth', source)
