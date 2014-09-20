#!/bin/bash

if [[ -n `which apt-get` ]]
then
	sudo apt-get update -qq
	
	echo "Installing libsdl2-dev through apt-get"
	sudo apt-get install -qq -y libsdl2-dev
	
	if [[ $? -ne 0 ]]
	then
		echo "Install from apt-get failed. Compiling from source."
		echo "Installing necessary dependencies."
		
		sudo apt-get install -qq -y build-essential xorg-dev libudev-dev libts-dev libgl1-mesa-dev libglu1-mesa-dev libasound2-dev libpulse-dev libopenal-dev libogg-dev libvorbis-dev libaudiofile-dev libpng12-dev libfreetype6-dev libusb-dev libdbus-1-dev zlib1g-dev libdirectfb-dev
		
		echo "Downloading SDL source."
		wget http://www.libsdl.org/release/SDL2-2.0.3.tar.gz
		
		echo "Extracting source."
		tar -xvzf SDL2-2.0.3.tar.gz 2>&1 > /dev/null
		
		cd SDL2-2.0.3
		
		echo "Configuring..."
		./configure
		
		echo "Making SDL."
		make
			
		echo "Installing SDL."
		sudo make install
	fi
	
	echo "Installing check through apt-get"
	sudo apt-get install -qq -y check
	
	echo "Installing glib through apt-get"
	sudo apt-get install -qq -y libglib2.0-dev
elif [[ -n `which brew` ]]
then
	echo "Installing check via homebrew."
	brew install check
	
	echo "Installing glib via homebrew."
	brew install glib
fi
