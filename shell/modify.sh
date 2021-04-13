#!bin/sh
# Michał Kaniuk 296815
# EOPSY Lab 1

upper=0
lower=0
recursive=0
pattern=0
files=0
name="$(basename "$0")"


error_message()
{
	echo "$name: error: $1"
	help 
}

help(){
	cat << EOT
        $name [-r] [-l|-u] <dir/file names...>
	$name [-r] <sed pattern> <dir/file names...>
	$name [-h]
EOT
exit 1
}

# recursive
recursive_rename(){
		
	if [ $upper == 1 ] ; then
		pattern='s/[a-z]/\U&/g'
	elif [ $lower == 1 ] ; then
		pattern='s/[A-Z]/\L&/g'
	fi
	if [ -d "$files" ] ; then	# check if a directory
		cd  "$files"
	fi
	for el in *; do		# iterate elements
		if [[ -d $el ]] ; then		# check if a directory
			cd $el
			recursive_rename
		else
			newname=`echo "$el" | sed "$pattern"`
			if [ "$newname" == '' ] ; then
				echo "wrong pattern"
				help
			elif [ "$el" != "$newname" ] ; then
				mv "$el" "$newname"
			fi	
		fi
	done
	cd ../
}

# normal
rename(){

	if [ $upper == 1 ] ; then
		pattern='s/[a-z]/\U&/g'
	elif [ $lower == 1 ] ; then
		pattern='s/[A-Z]/\L&/g'
	fi
	if [ -d "$files" ] ; then	# check if a directory
		cd $files
		for el in * ; do
			if [ -f $el ] ; then
				newname=`echo "$el" | sed "$pattern"`
				if [ "$el" != "$newname" ] ; then
					mv "$el" "$newname"
				fi	
			fi
		done
	else
		newname=`echo "$files" | sed "$pattern"`
		if [ "$files" != "$newname" ] ; then
			mv "$files" "$newname"
		fi
	fi

}

check_arguments(){
	if [ $lower == 1 ] && [ $upper == 1 ] ; then	
		echo "wrong arguments, -l and -u used together"
		help 
		exit 1
	fi
}

# if no arguments given
if test -z "$1" ; then
echo "no path was given"
help
exit 1
fi

# recognizing all arguments
for el in "$@"; do
	case "$el" in
		-l | --lowercase) lower=1 ; shift;;
		-u | --uppercase) upper=1 ; shift;;
		-h | --help) help ; shift;;
		-r | --recursion) recursive=1 ; shift;;
		-* ) error_message "wrong argument: $el" ; exit 1;;	
	esac
done 

# main part
check_arguments
while [ "$1" != "" ] ; do
	if test $recursive == 1; 
		then
		check_arguments
		if test -z "$1" ; then
			echo "no path was given"
			help
			exit 1
		fi
		if [ $lower == 1 ] || [ $upper == 1 ] ; then	
			files="$1"
			recursive_rename	
		else		
			if [ $pattern == 0 ] ; then
				pattern="$1"
				if test -z "$2" ; then
					echo "no path was given"
					help
					exit 1
			fi
			fi
			files="$2"
			if [ "$files" == "" ] ; then
				exit 0
			fi
			recursive_rename
		fi	
		
	else	
		files="$1"
		rename
	fi
	shift
done
