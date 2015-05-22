#!/usr/bin/sh

typeset arg file opts
typeset debug=n
typeset verbose=n

#####################################
# Example usage
#
# arg_list.sh -dv -f option one two
#
#####################################

function showUsage {
	print -u2 "Usage: ${0##*/} [-d] [-v] [-f file] arg1 [arg2] [...]"
	exit 1
}

print "\$#=$#"

while getopts 'dvf:ng:' opts
do
	case $opts in
		(d)  debug=y ;;
		(v)  verbose=y ;;
		(f)  file=$OPTARG ;;
		(n)  n=n ;;
		(g)  g=$OPTARG ;;
		(\?) showUsage ;; 
	esac
done
shift $(($OPTIND - 1))

print "\$#=$#"

cat <<-EOF
	debug=$debug
	verbose=$verbose
	file=$file
	n=$n
	g=$g
EOF

((i=0))
for arg; do
	((i+=1))
	print "arg$i=$arg"
done
