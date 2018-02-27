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

Here are all the external dependencies needed to build Chataigne :
  - Chataigne relies on the OrganicUI JUCE module that you can find here : https://github.com/benkuper/juce_organicui

### Windows only ###
You'll need to download and install the Kinect 2.0 SDK : https://www.microsoft.com/en-us/download/details.aspx?id=44561
The installer will automatically create an environment variable so you don't have to do anything else

You'll need to download and link the ASIO SDK as well : https://www.steinberg.net/en/company/developers.html 
Since this one is just a zip file, you'll need to create an environment variable called "ASIOSDK" : https://www.steinberg.net/en/company/developers.html

**DLL dependencies** :
  - For **Release**, you can just copy the dll files included in the latest releases available on the website, and paste them in the same folder as the generated executable (in the Binaries folder)
  - For **Debug**, you can get a dependency pack there http://benjamin.kuperberg.fr/chataigne/user/data/Chataigne-win-x64-debug-dependencies.zip and do the same in the Binaries/Debug folder
  
  
## Troubleshooting ##

If you have any issue regarding the software, you can post an issue on github (preferred for code-related problems and features),
there is also the official forum here : http://benjamin.kuperberg.fr/chataigne/forum , more suited for questions, wishes and user-level issues.



I do this on my own free time, feel free to buy me a beer :)
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=)
