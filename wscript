import os
import sys
import shutil

def remove_files(d):
   try:
      shutil.rmtree(d)
   except Exception as ex:
      print(ex)

def install_symlinks(hdr):

   sym_path =  os.path.abspath('include')

   if not os.geteuid()==0:
      sys.exit('\nERROR: Root Acces is required to execute this script.\n')

   dst_dir = '/usr/local/include/clp'

   remove_files(dst_dir)

   try:
      print('Creating Directory Tree...')
      os.symlink(sym_path,dst_dir)
      print('Header installation complete.')
   except Exception as ex:
      print(ex)



def install_headers(hdr):

   if not os.geteuid()==0:
      sys.exit('\nERROR: Root Acces is required to execute this script.\n')

   src_dir = 'include'
   dst_dir = '/usr/local/include/clp'

   remove_files(dst_dir)

   try:
      print('Creating Directory Tree...')
      shutil.copytree(src_dir,dst_dir,symlinks=True)
      print('Header installation complete.')
   except Exception as ex:
      print(ex)

def options(ctx):
   ctx.load('compiler_cxx')

def configure(ctx):
   ctx.load('compiler_cxx')

def build(bld):
   bld.program( includes='test', source='test/arg.cpp', target='arg-test' )

