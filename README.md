# Chataigne
Artist-friendly Modular Machine for Art and Technology

This software is under creation, any production usage is at your own risk !.
This page will be updated once it comes to a stable version.

You can check it up here : http://benjamin.kuperberg.fr/chataigne

Linux : [![Build Status](https://travis-ci.org/benkuper/Chataigne.svg?branch=master)](https://travis-ci.org/benkuper/Chataigne)

[![forthebadge](http://forthebadge.com/images/badges/gluten-free.svg)](http://forthebadge.com)


## Using the software

Compiled versions for Windows (8.1 and up), Mac OSX and Linux are available at : http://benjamin.kuperberg.fr/chataigne/download


## Building the software

Chataigne is built on top of the JUCE frameworks. You can find more infos about it there : http://www.juce.com
So basically, you just need to :
  - Download JUCE
  - Download the dependencies (see below)
  - Launch the Projucer and open Chataigne.jucer
  - Link the dependencies
  - Save the jucer
  - Open the solution for you platform in the Builds folder
  - Build the solution from your IDE or make in linux without Code::Blocks

### Dependencies
  
  Chataigne relies on the OrganicUI JUCE module that you can find here : https://github.com/benkuper/juce_organicui
  You will need to download, extract and link the Projucer juce_organicui module to its parent folder

### Windows only ###
**DLL dependencies** :
  - For **Release**, you can just copy the dll files from there : http://benjamin.kuperberg.fr/chataigne/user/data/Chataigne-win-x64-release-dependencies.zip and paste them in the same folder as the generated executable (in the Binaries folder)
  
  - For **Debug**, you can get a dependency pack there http://benjamin.kuperberg.fr/chataigne/user/data/Chataigne-win-x64-debug-dependencies.zip and do the same in the Binaries/Debug folder
  
  
## Troubleshooting ##

If you have any issue regarding the software, you can post an issue on github (preferred for code-related problems and features),
there is also the official forum here : http://benjamin.kuperberg.fr/chataigne/forum , more suited for questions, wishes and user-level issues.



I do this on my own free time, feel free to buy me a beer :)
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=KLQ6QBDF3ZL2W&lc=FR&item_name=Chataigne&item_number=chataigne%2dgithub&currency_code=EUR&bn=PP%2dDonationsBF%3adonate%2epng%3aNonHosted)
