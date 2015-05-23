#!/usr/bin/sed -f
# Delete HTML comments
# i.e. everything between <!-- and -->
#
# Does not cope with more than one comment per line

/<!--/!b
:a
/-->/!{
	$!{
		N
		ba
	}
}
s/<!--.*-->//
