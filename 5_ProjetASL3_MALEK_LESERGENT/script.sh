#! /bin/bash

echo '------ Test ------' > rapport.txt
for file in test/*; do

	./as < $file;
	echo "$file : $? " >> rapport.txt;
	echo

done