# Chataigne
Artist-friendly Modular Machine for Art and Technology
Official website : http://benjamin.kuperberg.fr/chataigne

### Intro

Chataigne is made with one goal in mind : create a common tool for artists, technicians and developers who wish to use technology and synchronize softwares for shows, interactive installations or prototyping.
It aims to be as simple as possible for basic interactions, but can be easily extended to create complex interactions.

![What a beautiful screenshot !](http://benjamin.kuperberg.fr/chataigne/user/images/screenshot2.png)


### Okay, but what does it do exactly ?

While Chataigne won't do much by itself, its purpose is to be the central hub in a project involving multiple interfaces, softwares, devices and sensors.
You can see it as a Conductor, which will control all the softwares with the big picture in mind.
Chataigne as been designed to fit both linear shows and realtime/interactive project needs.

### Seems cool, tell me more !

You can check the tutorials to better understand what and how you can use Chataigne for your projects.

### Features
  - Supports communication and synchronization of the following protocols :
    - OSC
    - MIDI
    - DMX
    - Serial
    - UDP
    - TCP
    - HTTP
    - WebSockets
    - PJLink 
    
  - Supports communication and synchronization of the following hardware :
    - KinectV2 (Win only)
    - StreamDeck
    - Joystick
    - Gamepad
    - Mouse
    - Keyboard
    - Wiimote
    - Joycon
    - Sound card
    
   - Pre-configured modules for controlling softwares :
    - Resolume
    - Madmapper
    - Millumin
    - QLab
    - HeavyM
    - D::Light
    - Reaper
    - Ableton Live
    - Powerpoint
    - Watchout
    
 - State Machine : Create both simple and complex real-time interactions and automations.
 - Time Machine : Create sequence based animations and triggers
 - Module Router : Easily route multiple values at once from one software to another, independant of the protocol
 - Custom Variables : Store and manipulate values, use presets and interpolate group of values.
 - Dashboard : Create your own UI and controls, customize colors and labels


## Using the software

Compiled versions for Windows, Mac OSX and Linux are available at : http://benjamin.kuperberg.fr/chataigne/#download

If beta versions are available, they will show up on this page.

If you wish to test the latest features and commits, the bleeding-edge section is generated at each commit.

## Getting Started

You can start by checking the Amazing Chataigne Documentation, made with love ! https://chataigne-goldengeek.gitbook.io/chataigne/

Also you can watch tutorials from the website and follow the interactive guide from withing the application.

## Building the software

Chataigne is built on top of the JUCE frameworks. You can find more infos about it there : http://www.juce.com
Chataigne is using a modified version of JUCE. So you first need to compile the Projucer from the modified version of JUCE instead of downloading JUCE :

  - Clone the develop-local branch from the JUCE git here : http://github.com/benkuper/JUCE 
  ```
  git clone --branch=develop-local http://github.com/benkuper/JUCE
  ```
  - In the *JUCE/extras/Projucer/Builds* folder, choose the folder depending on you system, open the solution with your IDE and compile it or make in linux. You can then open the new Projucer app that you just built.
  
  - Clone Chataigne with --recursive options to integrate organicUI and timeline modules
  ```
  git clone --recursive http://github.com/benkuper/Chataigne
  ```
  - Launch the Projucer and open Chataigne.jucer
  - Save the jucer
  - Open the solution for you platform in the Builds folder
  - Build the solution from your IDE and compile it or make in linux
  - Check the dependency section below if you have dependency problems when running Chataigne
  - Enjoy !
  
  
## Dependencies

### Windows running instructions ###
**DLL dependencies** :
  - For **Release**, you can just copy the dll files from there : http://benjamin.kuperberg.fr/chataigne/user/data/Chataigne-win-x64-release-dependencies.zip and paste them in the same folder as the generated executable (in the Binaries folder)
  
  - For **Debug**, you can get a dependency pack there http://benjamin.kuperberg.fr/chataigne/user/data/Chataigne-win-x64-debug-dependencies.zip and do the same in the Binaries/Debug folder
 
### Linux running instructions ###
  - Before running Chataigne, you just need to do a chmod a+x on the .AppImage file, or right-click on it and allow execution, then you're ready to play !  
  
## Troubleshooting

If you have any issue regarding the software, you can post an issue on github (preferred for code-related problems and features),
there is also the official forum here : http://benjamin.kuperberg.fr/chataigne/forum more suited for questions, wishes and user-level issues.

Chataigne is now on Discord, join and meet the community !

![Chataigne on discord](https://discordapp.com/api/guilds/697400442027769998/widget.png?style=banner2)

## The Badge Zone
Linux (x64):  [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/benkuper/Chataigne/branches/master/2)](https://travis-ci.org/benkuper/Chataigne)

MacOS : [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/benkuper/Chataigne/branches/master/1)](https://travis-ci.org/benkuper/Chataigne)

Windows (x64) : [![Build status](https://ci.appveyor.com/api/projects/status/7lskg35ttnsv99cq?svg=true)](https://ci.appveyor.com/project/benkuper/chataigne)

Raspberry Pi (3B):  [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/benkuper/Chataigne/branches/master/3)](https://travis-ci.org/benkuper/Chataigne)


[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/makes-people-smile.svg)](https://forthebadge.com)
[![https://good-labs.github.io/greater-good-affirmation/assets/images/badge.svg](https://good-labs.github.io/greater-good-affirmation/assets/images/badge.svg)](https://good-labs.github.io/greater-good-affirmation)

## Libraries uses, references and other kudos

This list contains all the references to projects and libraries that Chataigne uses, either as an external libraries, or merged with and modified for Chataigne's source code.

- JUCE : https://www.juce.com
- Serial Library : https://github.com/wjwwood/serial
- MIDI Timecode, from MStarPlayer : https://github.com/ServiusHack/MStarPlayer
- Wiimote library Wiiuse : https://github.com/wiiuse/wiiuse
- Joycon library : https://github.com/CTCaer/joytime-core
- Zeroconf library Servus : https://github.com/HBPVIS/Servus
- Libartnet : https://github.com/OpenLightingProject/libartnet
- Joystick Library SDL : https://www.libsdl.org/
- HIDAPI : https://github.com/signal11/hidapi
- Microsoft Kinect SDK 2.0 : https://developer.microsoft.com/en-us/windows/kinect

## Perhaps the most important

I do this on my own free time, feel free to buy me a beer :)

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=KLQ6QBDF3ZL2W&lc=FR&item_name=Chataigne&item_number=chataigne%2dgithub&currency_code=EUR&bn=PP%2dDonationsBF%3adonate%2epng%3aNonHosted)

Are you using Chataigne on a regular basis ? Then why not support me accordingly, so I can have a more stable way to work on it !

Choose your angel weapon :

[![Click click click !](http://benjamin.kuperberg.fr/chataigne/user/images/github_sponsors.png)](https://github.com/sponsors/benkuper)

<a href="https://www.patreon.com/bePatron?u=23293310"><img src="https://c5.patreon.com/external/logo/become_a_patron_button@2x.png" width="140"></a>
