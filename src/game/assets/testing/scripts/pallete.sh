#!/bin/bash

#get pallete from txt
#I extracted this pallete as inspiration from fallout 2

set -x

INFILE=./data/colors.txt

colors=()
num=0

while read -r COLOR;
do
	colors+=("xc:$COLOR")
	(( num++ ))
done < "$INFILE"

magick -size 10x10 "${colors[@]}" +append pallete.png
echo "converted: $num"
