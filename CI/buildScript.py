
import os;
from PyUtils import *



#  default values


bumpVersion = False
sendToOwncloud = False
specificVersion = ""
cleanFirst = False;
localExportPath2 = [
# "/Volumes/Thor/OO\ Projets/OwnCloud/Tools/LGML/App-Dev/OSX/"
# ,"/Volumes/Pguillerme/Documents/LGML/"
];

executable_name = "Chataigne"

rootPath = os.path.abspath(os.path.join(__file__,os.pardir,os.pardir))
isBeta = False

def generateProductBaseName():
	name =  executable_name+ "_v"+str(ProJucerUtils.getVersion())
	return name




def buildAll(osType,configuration):
	global specificVersion
	ProJucerUtils.updatePathsIfNeeded(osType)
	ProJucerUtils.proJucerPath = 'dummy'
	ProJucerUtils.getProjucerIfNeeded(tmpFolder=os.path.abspath(os.path.join(os.environ['HOME'],'Dev','Projucer')),credentials=OwncloudUtils.getCredential(),osType=osType)

	if ProJucerUtils.hasValidProjucerPath():
		ProJucerUtils.updateVersion(bumpVersion,specificVersion);
		ProJucerUtils.buildJUCE();
	else:
		print 'not updating projucer'
	
	if osType=='osx':
		import osx
		osx.buildApp(configuration = configuration);#xcodeProjPath,configuration,appPath,njobs,cleanFirst);
	elif osType=='linux':
		import linux
		linux.buildApp(configuration = configuration)


def exportAll(osType,configuration,sendToOwncloud):
	global appPath
	baseName = generateProductBaseName()
	if osType=='osx':
		import osx
		exportedPath = osx.exportApp(baseName);
	elif osType=='linux':
		import linux
		exportedPath = linux.exportApp(baseName);

	# for p in localExportPath2:
	# 	sh("cp "+exportedPath+" "+p+generateProductBaseName()+".dmg")

	#
	#if sendToOwncloud:
	#	if osType=='osx':
	#		ownCloudPath = "Tools/LGML/App-Dev/OSX/"+generateProductBaseName()+".dmg"
	#	elif osType=='linux':
	#		import platform
	#		distName = ''.join(platform.linux_distribution()[:-1])
	#		ownCloudPath = "Tools/LGML/App-Dev/Linux/"+distName+"/"+generateProductBaseName()+".tar.gz"
	#		
	#	OwncloudUtils.sendToOwnCloud(exportedPath,ownCloudPath)
	
	# gitCommit()

if __name__ == "__main__":
	global specificVersion
	print sys.argv
	
	import argparse
	parser = argparse.ArgumentParser(description='python util for building and exporting LGML')
	parser.add_argument('--build', action='store_true',
	                    help='build it',default = True)
	parser.add_argument('--export', action='store_true',
	                    help='export it',default = True)
	parser.add_argument('--beta', action='store_true',
	                    help='switch to beta version (only name affected for now)',default=True)
	parser.add_argument('--os',help='os to use : osx, linux', default=None)

	parser.add_argument('--configuration',help='configuration to use ', default=None)

	args = parser.parse_args()



# try to find os
	if not args.os:
		import platform,os
		curOs = os.name
		curPlatform = platform.system()

		if curPlatform=='Linux':
			args.os = 'linux'
		elif curPlatform=='Darwin':
			args.os = 'osx'

		else:
			print 'platform not supported : ',curOs,curPlatform
			exit(1)

# default configurations for oses
	if args.os=='osx' and not args.configuration:
		args.configuration = 'Debug'
	elif args.os=='linux' and not args.configuration:
		args.configuration = 'Ubuntu'


#  check actions
	needBuild = args.build
	needExport= args.export


	isBeta = args.beta
	# default if no command (triggers build and export)
	if len(sys.argv)==1:
		needBuild = True;
		needExport=True;

	if isBeta:
		currentV  = ProJucerUtils.getVersionAsList()
		specificVersion = '.'.join(map(str,currentV[:-1]))+"beta"


		
	if needBuild:
		buildAll(args.os,args.configuration);
	if needExport:
		# send per default if used explicitly with export arg
		sendToOwncloud |= args.export
		exportAll(args.os,args.configuration,sendToOwncloud=sendToOwncloud);


