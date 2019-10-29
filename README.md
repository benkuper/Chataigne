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

## Using the software
Compiled versions for Windows, Mac OSX and Linux are available at : http://benjamin.kuperberg.fr/chataigne/#download
If beta versions are available, they will show up on this page.
If you wish to test the latest features and commits, the bleeding-edge section is generated at each commit.

## Building the software

Chataigne is built on top of the JUCE frameworks. You can find more infos about it there : http://www.juce.com
Chataigne is using a modified version of JUCE. So basically, you just need to :
  - Clone the develop-local branch from the JUCE git here : http://github.com/benkuper/JUCE 
  - Clone OrganicUI : https://github.com/benkuper/juce_organicui 
  - Clone Timeline : https://github.com/benkuper/juce_timeline
  - Launch the Projucer and open Chataigne.jucer
  - Link the juce_organicui and juce_timeline modules in the Projucer (see the JUCE documentation) 
  - Save the jucer
  - Open the solution for you platform in the Builds folder
  - Build the solution from your IDE or make in linux
  - Check the dependency section below if you have dependency problems when running Chataigne
  - Enjoy !
  
  
## Dependencies

### Windows running instructions ###
**DLL dependencies** :
  - For **Release**, you can just copy the dll files from there : http://benjamin.kuperberg.fr/chataigne/user/data/Chataigne-win-x64-release-dependencies.zip and paste them in the same folder as the generated executable (in the Binaries folder)
  
  - For **Debug**, you can get a dependency pack there http://benjamin.kuperberg.fr/chataigne/user/data/Chataigne-win-x64-debug-dependencies.zip and do the same in the Binaries/Debug folder
 
### Linux running instructions ###
  - Before running Chataigne, you will need to copy the lib folder from Chataigne.AppDir/usr/lib aside the Chataigne executable, i.e. in the build folder.
  
## Troubleshooting

If you have any issue regarding the software, you can post an issue on github (preferred for code-related problems and features),
there is also the official forum here : http://benjamin.kuperberg.fr/chataigne/forum more suited for questions, wishes and user-level issues.

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

## Perhaps the most important

I do this on my own free time, feel free to buy me a beer :)

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=KLQ6QBDF3ZL2W&lc=FR&item_name=Chataigne&item_number=chataigne%2dgithub&currency_code=EUR&bn=PP%2dDonationsBF%3adonate%2epng%3aNonHosted)

Are you using Chataigne on a regular basis ? Then why not support me accordingly, so I can have a more stable way to work on it !

Choose your angel weapon :

[![Click click click !](http://benjamin.kuperberg.fr/chataigne/user/images/github_sponsors.png)](https://github.com/sponsors/benkuper)

<a href="https://www.patreon.com/bePatron?u=23293310"><img src="https://c5.patreon.com/external/logo/become_a_patron_button@2x.png" width="140"></a>
