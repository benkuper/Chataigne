
import os;
import json;
import urllib;
import multiprocessing



import sys
pathToAdd = os.path.abspath(os.path.join(__file__,os.path.pardir,os.path.pardir))
sys.path.insert(1,pathToAdd)

from PyUtils import *


# default configuration

# configuration  = "Release"
configuration  = "Debug"
cleanFirst = False;

architecture = "i386"




# paths relative to this file
rootPath = os.path.abspath(os.path.join(__file__,os.pardir,os.pardir,os.pardir))

localExportPath = os.path.join(rootPath,'Builds/MacOSX/build/')
localExportPath = os.path.abspath(localExportPath)+"/"

xcodeProjPath = os.path.join(rootPath,"Builds/MacOSX/")
executable_name = "Chataigne"
appPath = os.path.join(xcodeProjPath,"build",configuration,executable_name+".app")

isBeta = False

njobs = multiprocessing.cpu_count()



def buildApp(xcodeProjPath=xcodeProjPath,configuration=configuration,appPath=appPath,njobs=njobs,clean = cleanFirst):
	if len(appPath)>10:
		sh("rm -rf "+appPath)
	configuration="Debug" if not configuration else configuration
	if clean:
		sh("cd "+xcodeProjPath+ " && "\
		+" xcodebuild -project Chataigne.xcodeproj" \
		+" -configuration "+configuration
		+" clean")

	sh("cd "+xcodeProjPath+ " && "\
		+" xcodebuild -project Chataigne.xcodeproj" \
		+" -configuration "+configuration
		+" -arch "+architecture
		+" -jobs "+str(njobs))



def createDmg(exportFileBaseName,appPath):
	import dmgbuild
	print 'creating dmg'
	os.chdir(os.path.abspath(os.path.join(__file__,os.path.pardir)))
	dmgbuild.build_dmg(exportFileBaseName,"Chataigne",settings_file = 'dmgbuild_conf.py',defines={'app':appPath})
	print 'dmg done at :'+exportFileBaseName+'.dmg'
	return exportFileBaseName+'.dmg'




def exportApp(baseName):
	global appPath
	localPath = localExportPath+baseName;
	dmgPath = createDmg(localPath,appPath);
	return dmgPath
	# gitCommit()

# if __name__ == "__main__":
# 	global specificVersion
# 	print sys.argv
	
# 	import argparse
# 	parser = argparse.ArgumentParser(description='python util for building and exporting LGML')
# 	parser.add_argument('--build', action='store_true',
# 	                    help='build it')
# 	parser.add_argument('--export', action='store_true',
# 	                    help='export it')
# 	parser.add_argument('--beta', action='store_true',
# 	                    help='switch to beta version (only name affected for now)')
# 	parser.add_argument('--os',help='os to use : osx, linux', default='osx')

# 	args = parser.parse_args()
# 	needBuild = args.build
# 	needExport= args.export
# 	isBeta = args.beta
# 	if len(sys.argv)==1:
# 		needBuild = True;
# 		needExport=True;


# 	if isBeta:
# 		currentV  = ProJucerUtils.getVersionAsList()
# 		specificVersion = '.'.join(map(str,currentV[:-1]))+"beta"


		
# 	if needBuild:
# 		buildAll(args.os);
# 	if needExport:
# 		# send per default if used explicitly with export arg
# 		sendToOwncloud = args.export
# 		exportAll();


