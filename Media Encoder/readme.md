# PNG to VideoStream encoder

Creates a single flat PNG Stream file for FAST read into a video playback system

# Compilation

`````
g++ -o PNGEncoder PNGEncoder.cpp upng.cpp
`````
or
`````
make
`````

# PNG Media requirements

This tool is set up for the 192x64px LCD Display and requires each PNG frame to be the same dimensions. Please all your PNG files into a single directory eg "/home/user/AnimationNameBlah" . Files need to be in the following format
`````
AnimationNameBlah_00001.png
`````
The directory name must be the same as the 1st part of the PNG file name, followed by "_" followed by a 5 digit counter indexing the files, starting at 1.

To run the tool on the above example directory:
`````
./PNGEncoder/PNGEncoder /home/user/AnimationNameBlah/ AnimationNameBlah 1000
`````
where:
`````
./PNGEncoder/PNGEncoder PATH_TO_PNG_FILES ANIMATION_NAME NUMBER_OF_FRAMES
`````
