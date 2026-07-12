#!/bin/bash

if [[ "$#" < 3 ]]; then
	echo "Usage: /directory_containing_images /output_directory /pallete.png [OPTIONAL] fps"
	exit 1
fi 

directories=()
fps=${4:-8}
animationTitle="john_doe_0"
regular_dir="$2regular"
granular_1_dir="$2granular1"
granular_2_dir="$2granular2"
granular_3_dir="$2granular3"
regular_files=()
granular_1_files=()
granular_2_files=()
granular_3_files=()

for item in "$1"*; do
	directories+=("$item")
done

declare -i x y maxWidth maxWidthReached frames foundFrames fileLine
x=0
y=0
maxWidth=8 
maxWidthReached=0
frames=0
foundFrames=0
mkdir -p "$granular_1_dir"
mkdir -p "$granular_2_dir"
mkdir -p "$granular_3_dir"
mkdir -p "$regular_dir"
files=()

if [[ -e temp.data ]]; then rm temp.data; fi
for directory in "${directories[@]}"; do
	nDir="$directory/N/"
	sDir="$directory/S/"
	eDir="$directory/E/"
	wDir="$directory/W/"
	neDir="$directory/NE/"
	nwDir="$directory/NW/"
	seDir="$directory/SE/"
	swDir="$directory/SW/"
	cardinal=( "$nDir" "$sDir"  "$eDir"  "$wDir"  "$neDir"  "$nwDir"  "$seDir"  "$swDir" )
	for dir in "${cardinal[@]}"; do if [[ ! -e "$dir" ]]; then echo "Missing cardinal directory: $dir"; exit 1; fi done
	
	cardinal=( "$nDir"* "$sDir"*  "$eDir"*  "$wDir"*  "$neDir"*  "$nwDir"*  "$seDir"*  "$swDir"* )
	animName=$(basename "$directory")
	
	echo "[$animName]"
	echo "[$animName]" >> temp.data

	frames=0
	foundFrames=0
	current_card_dir="direction"
	for file in "${cardinal[@]}"; do
		if [[ x -ge maxWidth ]]; then x=0; y+=1; maxWidthReached=1; fi
		n=$(basename "$file")
		card_dir_name=$(dirname "$file") 
		card_dir_name=$(basename "$card_dir_name")
		if [[ foundFrames -eq 0 ]]; then frames+=1; fi

		if [[ $card_dir_name != $current_card_dir ]]; then
			if [[ foundFrames -eq 0 && $current_card_dir != "direction" ]]; then 
				frames+=-1; 
				foundFrames=1
				echo "frames = $frames"

				#append frames and fps to the file
				#using grep and cut to get animation line and sed to append it on a given offset
				line=$(grep -n "$animName" temp.data | cut -d: -f1)
				declare -i offsetLine
				offsetLine=$line+1
				sed -i "$offsetLine""i frames = $frames" temp.data
				offsetLine=$line+2
				sed -i "$offsetLine""i fps = $fps" temp.data
			fi

			echo "[$card_dir_name]" 
			echo "[$card_dir_name]" >> temp.data
			current_card_dir="$card_dir_name"
			echo "x = $x"
			echo "y = $y"

			echo "x = $x" >> temp.data
			echo "y = $y" >> temp.data
			
		fi
		x+=1

		cp "$file" "$regular_dir/$current_card_dir$n"
		regular_files+=("$regular_dir/$current_card_dir$n")

		#palleted files creation
		magick "$file" -background "#FE01FD" -alpha remove +dither -remap $3 "$granular_1_dir/$current_card_dir$n"
		granular_1_files+=("$granular_1_dir/$current_card_dir$n")

		magick "$file" -background "#FE01FD" -alpha remove -dither Riemersma -remap $3 "$granular_2_dir/$current_card_dir$n"
		granular_2_files+=("$granular_2_dir/$current_card_dir$n")

		magick "$file" -background "#FE01FD" -alpha remove -dither FloydSteinberg -remap $3 "$granular_3_dir/$current_card_dir$n"
		granular_3_files+=("$granular_3_dir/$current_card_dir$n")
	done
	
done

echo "[END]" >> temp.data

echo -n "animation title: "
read animationTitle
if [[ $animationTitle == "" ]]; then 
	animationTitle="[Default]"; 
else 
	animationTitle="[$animationTitle]"
fi

sed -i "1i $animationTitle" temp.data

declare -i finalWidth
finalWidth=0
if [[ maxWidthReached -eq 1 ]]; then
	finalWidth=$maxWidth
else
	finalWidth=$x
fi
	y+=1
	magick montage -background transparent -geometry 128x128 -tile "$finalWidth""x""$y" "${regular_files[@]}" "$2/montage_no_pallete.png"

	magick montage -background transparent -geometry 128x128 -tile "$finalWidth""x""$y" "${granular_1_files[@]}" "$2/granular_1_montage.png"

	magick montage -background transparent -geometry 128x128 -tile "$finalWidth""x""$y" "${granular_2_files[@]}" "$2/granular_2_montage.png"

	magick montage -background transparent -geometry 128x128 -tile "$finalWidth""x""$y" "${granular_3_files[@]}" "$2/granular_3_montage.png"

sed -i "2i"" [montage_no_pallete.png]" temp.data
cp temp.data "$2/nopallete.data"
sed -i "2d" temp.data
sed -i "2i"" [granular_1_montage.png]" temp.data
cp temp.data "$2/granular1.data"
sed -i "2d" temp.data
sed -i "2i"" [granular_2_montage.png]" temp.data
cp temp.data "$2/granular2.data"
sed -i "2d" temp.data
sed -i "2i"" [granular_3_montage.png]" temp.data
cp temp.data "$2/granular3.data"

removeTemp="y"
echo -n "remove temp files? (y/n)"
read removeTemp

if [[ "$removeTemp" == "n" ]]; then
	exit 0
else
	rm temp.data
	rm -R "$granular_1_dir" "$granular_2_dir" "$granular_3_dir" "$regular_dir" 
fi


