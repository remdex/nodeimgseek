import Options
from os import unlink, symlink, popen
from os.path import exists

srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "imgseek"
  obj.source = "imgseek.cpp imgdb.cpp bloom_filter.cpp haar.cpp" 
  obj.lib = ["Magick++"]
  obj.cxxflags = ["-g","-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE","-Wall"]
  obj.includes = ["/usr/include/ImageMagick"]

def shutdown():
  if Options.commands['clean']:
    if exists('imgseek.node'): unlink('imgseek.node')
  else:
    if exists('build/Release/imgseek.node') and not exists('imgseek.node'):
      symlink('build/Release/imgseek.node', 'imgseek.node')
