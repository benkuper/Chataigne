FROM ubuntu:14.04
MAINTAINER OrganiGeek <duh@example.com>
RUN apt-get update && apt-get -y --force-yes install git

RUN apt-get -y --force-yes install libfreetype6-dev libx11-dev libxinerama-dev libxrandr-dev libxcursor-dev mesa-common-dev libasound2-dev freeglut3-dev libxcomposite-dev tar

RUN git clone --depth 1  https://github.com/julianstorer/JUCE.git
RUN apt-get -y --force-yes install make pkg-config g++ libcurl3-dev
RUN cd JUCE/extras/Projucer/Builds/LinuxMakefile && make -j2
