#!/bin/ksh
# Strip C comments
# by Stewart Ravenhall
# 4 October 2000

# Strip everything between /* and */ inclusive

# Copes with multi-line comments,
# disassociated end comment symbols,
# disassociated start comment symbols,
# multiple comments per line

# Need one command-line parameter
[[ $# = 1 ]] || {
	print "Usage: ${0##*/} file"
	exit 1
}

# Check given file exists
[[ -a $1 ]] || {
	print "${0##*/}: $1 does not exist"
	exit 2
}

# Create shell variables for ASCII 1 (control-a)
# and ASCII 2 (control-b)
a="$(echo | tr '\012' '\001')"
b="$(echo | tr '\012' '\002')"

# Swap start comments for control-a
# and end comments for control-b
sed "
	s:/\*:$a:g
	s:\*/:$b:g
" $1 | sed "
	# If no start comment then go to end of script
	/$a"'/!b
	:a
	# If no end comment
	/'"$b"'/!{
		# If not last line then read in next one
		$!{
			N
			ba
		}
		# If last line then remove from start
		# comment to end of line
		# then go to end of script
		${
			'"s:$a[^$b]*"'$::
			b
		}
	}
	# Remove comments
	'"s:$a[^$b]*$b::g
" | sed "
	# Swap control characters back to comment symbols
	s:$a:/*:g
	s:$b:*/:g
"
