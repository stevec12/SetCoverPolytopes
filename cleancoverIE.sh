#!/bin/bash
# Uses setcover to generate a cover and outputs IEs from polymake
# usage: coverIE filename-stem
if [ ${#} -lt 3 ] ; then
	echo 'usage: $coverIE |Universe| total_sets filename-stem';
	exit 1;
fi
echo "${1} ${2} y ${3}" | ./cleansetcover
#Now have file filename-stem.txt as suitable script file for polymake
polymake --script ${3}.txt > "${3}IE.txt" 2>/dev/null
cat ${3}*
printf "\n"
