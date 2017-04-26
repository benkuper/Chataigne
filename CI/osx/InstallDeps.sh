#!/usr/bin/bash
pwd;

# go to home
cd ~;


pwd;

# install VST if needed
if [ ! -d "SDKs/VST3 SDK" ]; then
	mkdir "SDKs";
	curl -L http://www.steinberg.net/sdk_downloads/vstsdk366_27_06_2016_build_61.zip > VST3.zip
	tar -xzf VST3.zip;
	mv "VST3 SDK/" "SDKs/VST3 SDK/";
fi


# install JUCE if needed
if [ ! -d "Dev/JUCE" ]; then
	mkdir "Dev";
	curl -L https://github.com/julianstorer/JUCE/archive/master.zip > JUCE.zip;
	tar -xzf JUCE.zip;
	mv JUCE-master Dev/JUCE;
fi


# git clone "https://github.com/julianstorer/JUCE.git" --depth 1;
# pwd && ls;