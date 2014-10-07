#!/bin/bash

function compile_from_source() {
	FILENAME=`basename $1`
	SRC_DIR=`basename $1 .tar.gz`
	
	echo "Downloading ${FILENAME}."
	wget $1
	
	echo "Extracting ${FILENAME}."
	tar -xvzf $FILENAME 2>&1 > /dev/null
	
	cd $SRC_DIR
	
	echo "Configuring..."
	./configure
	
	echo "Making ${SRC_DIR}."
	make
	
	echo "Installing ${SRC_DIR}."
	sudo make install
}

if [[ -n `which apt-get` ]]
then
	sudo apt-get update -qq
	
	echo "Installing libsdl2-dev through apt-get"
	sudo apt-get install -qq -y libsdl2-dev
	
	if [[ $? -ne 0 ]]
	then
		echo "Install from apt-get failed."
		echo "Installing necessary dependencies."
		
		sudo apt-get install -qq -y build-essential xorg-dev libudev-dev libts-dev libgl1-mesa-dev libglu1-mesa-dev libasound2-dev libpulse-dev libopenal-dev libogg-dev libvorbis-dev libaudiofile-dev libpng12-dev libfreetype6-dev libusb-dev libdbus-1-dev zlib1g-dev libdirectfb-dev

        echo "Compiling from source."

		compile_from_source http://www.libsdl.org/release/SDL2-2.0.3.tar.gz
	fi
	
	echo "Installing libsdl2-image-dev"
	sudo apt-get install -qq -y libsdl2-image-dev
	
	if [[ $? -ne 0 ]]
	then
		echo "Install from apt-get failed. Compiling from source."
		
		compile_from_source https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.0.tar.gz
	fi
	
	echo "Installing check through apt-get"
	sudo apt-get install -qq -y check
	
	echo "Installing glib through apt-get"
	sudo apt-get install -qq -y libglib2.0-dev

    echo "Installing libyaml-dev through apt-get"
    sudo apt-get install -qq -y libyaml-dev
elif [[ -n `which brew` ]]
then
	echo "Installing check via homebrew."
	brew install check
	
	echo "Installing glib via homebrew."
	brew install glib

    echo "installing libyaml via homebrew."
    brew install libyaml
fi
