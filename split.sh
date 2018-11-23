#!/bin/bash
rm ll
count=1
cat text.py | while read line
do
	((i++))
	isVal=$(($i%7))
	if [ $isVal -eq 6 ];then
		echo $line
	else
		echo $line >> ll
		echo $isVal
		
	fi
done

