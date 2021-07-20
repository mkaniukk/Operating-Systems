# uni-eopsy
A program which copies one file to another. Syntax:

  copy [-m] <file_name> <new_file_name>
  
  copy [-h]

Without option -m executes with read() and write() functions to copy file contents. If
the option -m is given,executes with map files to memory regions with mmap() and copy the file with memcpy() instead.

If the option -h is given or the program is called without arguments print
out help information.  
