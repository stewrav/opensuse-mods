#!/bin/bash
##############################################################
#
#/ lastmod
#/ 
#/ SYNTAX:
#/         lastmod [-d] [-n] [-h] [num] <dir>
#/ 
#/ DESCRIPTION:
#/         Show when a directory was most recently modified, by displaying
#/         the file name and last modification date/time of the specified
#/         number of most recently modified files.
#/ 
#/         Please do not specify high level directories like
#/         /opt/esas/prd/root/clinical_studies or above, since lastmod will
#/         search the specified directory and all directories below. This
#/         will consume a lot of shared resources.
#/ 
#/ OPTIONS:
#/         -d : debug.
#/              Show lastmod script debugging info.
#/ 
#/         -n : no file names.
#/              Do not display file names when outputting the results. This
#/              makes sense if num=1 as it shows the modification time of
#/              the most recently modified file. It makes less sense as num
#/              gets larger.
#/ 
#/         -h : help.
#/              Show this help note, then exit.
#/ 
#/ PARAMETERS:
#/         num : the maximum number of files to display. If omitted, this
#/               parameter is set to "1".
#/ 
#/         dir : the directory to search. Files in the specified directory
#/               or below are inspected.
#/ 
#/ EXAMPLES:
#/         (*) Show the name and date/time of the most recently modified file
#/             in the current directory or below:
#/ 
#/             lastmod .
#/ 
#/         (*) Show the date/time of the most recently modified file in the
#/             current directory or below:
#/ 
#/             lastmod -n .
#/ 
#/         (*) Show the names and date/times of the three most recently modified
#/             files in the
#/             /opt/esas/prd/root/clinical_studies/ro_testing/cdpt9999/or99999
#/             directory or below:
#/ 
#/             lastmod 3 $ESAS_ROOT/clinical_studies/ro_testing/cdpt9999/or99999
#/ 
#/         (*) Show this help message, then exit:
#/ 
#/             lastmod -h
#/ 
##############################################################

set -o nounset
set -o noclobber
basename="${0##*/}"

function helpnote {
	showdebug str 'showing help note'
	grep '^#/ ' "$0" | cut -c4-
	exit 0
}

function die {
	echo "$basename: $1" >&2
	exit $2
}

function showdebug() {
  local type=$1
  local detail=$2

  (($debug)) || return

  if [[ $type == var ]]; then
    eval echo \"Debug\($basename\): $detail == \$$detail\" >&2
  elif [[ $type == str ]]; then
    echo >&2 "Debug($basename): $detail"
  else
    die "unknown debug type ${type} for detail ${detail}" 1
  fi
}

# Options
showfilenames=1
debug=0
while getopts ':ndh' option
do
	case $option in
		(n) showfilenames=0 ;;
		(d) debug=1   ;;
		(h) helpnote  ;;
		(*) echo "${0##/}: unknown option $option" >&2
		    exit 2    ;;
	esac
done
shift $(($OPTIND - 1))
showdebug var debug
showdebug var showfilenames

# Checks
if [[ $# -eq 2 ]]; then
	num="$1"
	dir="$2"
elif [[ $# -eq 1 ]]; then
	num='1'
	dir="$1"
else
	die "Usage: $basename [num] dir" 1
fi
showdebug var num
showdebug var dir

[[ -e $dir ]] || die "directory $dir does not exist." 2
[[ -d $dir ]] || die "$dir is not a directory." 2
expr $num + 1 >/dev/null 2>&1 || die "$num is not numeric." 2

if [[ -z $(ls -A "$dir" 2>/dev/null) ]]
then
	echo "$basename: directory $dir is empty."
elif (( $showfilenames ))
then
	showdebug str 'showing file names'
	exe="find '$dir' -xdev -type f -print0 | xargs -0 filetime -vsN | sort -br -t$'\t' -k2 | head -n $num"
	showdebug var exe
	eval $exe
else
	showdebug str 'not showing file names'
	exe="find '$dir' -xdev -type f -print0 | xargs -0 filetime -sn | sort -br | head -n $num"
	showdebug var exe
	eval $exe
fi

