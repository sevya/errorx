#! /bin/csh

if ( ! -f $1 ) then
	echo 0
else 
	grep Query $1 | wc -l
endif
