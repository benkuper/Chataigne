#!/usr/bin/bash

## these are devloper libs,   not sure wich are needed in released version...


sudo apt-get -y --force-yes install libfreetype6-dev
sudo apt-get -y --force-yes install libx11-dev
sudo apt-get -y --force-yes install libxinerama-dev
sudo apt-get -y --force-yes install libxrandr-dev
sudo apt-get -y --force-yes install libxcursor-dev
sudo apt-get -y --force-yes install mesa-common-dev
sudo apt-get -y --force-yes install libasound2-dev
sudo apt-get -y --force-yes install freeglut3-dev
sudo apt-get -y --force-yes install libxcomposite-dev
sudo apt-get -y --force-yes install libjack-dev




#https
#sudo apt-get -y install libcurl4-gnutls-dev

# install source needed for LGML build
cd ;
if [ ! -d "SDKs/VST3 SDK" ]; then
	mkdir "SDKs"
	curl -L http://www.steinberg.net/sdk_downloads/vstsdk366_27_06_2016_build_61.zip > VST3.zip
	unzip -q VST3.zip
	mv "VST3 SDK/" "SDKs/VST3 SDK/"
fi

if [ ! -d "Dev/JUCE" ]; then
	mkdir "Dev"
	curl -L https://github.com/julianstorer/JUCE/archive/master.zip > JUCE.zip
	unzip -q JUCE.zip
	mv JUCE-master/ Dev/JUCE
fi

if [ ! -d "Dev/Projucer/linux" ]; then
	cd 
	ls

	cd Dev/JUCE/extras/Projucer/Builds/LinuxMakefile/
	make -j2
	cd 
	pwd
	mkdir Dev/Projucer
	mkdir Dev/Projucer/linux
	mv Dev/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer Dev/Projucer/linux/
fi
