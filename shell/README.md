# uni-eopsy

A script with the following syntax:

  modify [-r] [-l|-u] <dir/file names...>
  
  modify [-r] <sed pattern> <dir/file names...>
  
  modify [-h]
  
The script is dedicated to lowerizing (-l) file names,
uppercasing (-u) file names or internally calling sed command with the given sed pattern which will operate on file names. 
Changes may be done either with recursion (-r) or without it.
