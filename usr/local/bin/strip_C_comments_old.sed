# Delete C comments
# i.e. everything between /* and */
#
# Does not cope with more than one comment per line
#
# S G Ravenhall

/\/\*/!b
:a
/\*\//!{
	$!{
		N
		ba
	}
}
s:/\*.*\*/::
