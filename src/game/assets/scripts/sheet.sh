#!/bin/bash

if [[ "$#" < 3 ]]; then
	echo "Usage: ./sheet /directory_containing_images /output_directory /pallete.png"
	exit 1
fi

files=()

for file in "$1"*.png ;do
	files+=("$file")
done

magick montage -background transparent -geometry 128x128 "${files[@]}" "$2/montage_no_pallete.png"

granular_1_pallete="$2/granular1"
granular_2_pallete="$2/granular2"
granular_3_pallete="$2/granular3"
granular_1_files=()
granular_2_files=()
granular_3_files=()

mkdir -p "$granular_1_pallete"
mkdir -p "$granular_2_pallete"
mkdir -p "$granular_3_pallete"

for file in "${files[@]}" ;do
	n=$(basename "$file")

	echo "$granular_1_pallete/$n"
	magick "$file" -background "#FE01FD" -alpha remove +dither -remap $3 "$granular_1_pallete/$n"
	granular_1_files+=("$granular_1_pallete/$n")

	echo "$granular_2_pallete/$n"
	magick "$file" -background "#FE01FD" -alpha remove -dither Riemersma -remap $3 "$granular_2_pallete/$n"
	granular_2_files+=("$granular_2_pallete/$n")

	echo "$granular_3_pallete/$n"
	magick "$file" -background "#FE01FD" -alpha remove -dither FloydSteinberg -remap $3 "$granular_3_pallete/$n"
	granular_3_files+=("$granular_3_pallete/$n")
done

magick montage -geometry 128x128 "${granular_1_files[@]}" "$2/montage_pallete1.png"
magick montage -geometry 128x128 "${granular_2_files[@]}" "$2/montage_pallete2.png"
magick montage -geometry 128x128 "${granular_3_files[@]}" "$2/montage_pallete3.png"

echo -n "delete temp files?[y/n]: "
read -r ans
if [[ "$ans" == "y" || "$ans" == "N" ]]; then
	echo "deleting files..."
	rm -R "$granular_1_pallete" "$granular_2_pallete" "$granular_3_pallete" 
fi

#magick ./output/raw_renders/ThinWall0.png -background "#FE01FD" -alpha remove -remap pallete.png ThinWall0.png

