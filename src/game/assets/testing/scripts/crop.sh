#!/bin/bash

set -xe

if [[ "$#" < 3 ]]; then
	echo "Usage: ./crop.sh /path/img.png /path_to_output/ [2x2,4x4...]" 
fi

cuts="2@x2@"
name=$(basename "$1")

magick "$1" -crop "$cuts" "$2/crop%02d_$name"


