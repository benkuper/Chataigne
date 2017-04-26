# this script analyses code and remove useless dependencies (can not be fully trusted, but still useful )
# if one want to force dependency to stay the line has to end by "//keep"


import os,copy

# globalOptions
globalIgnore = [""]
showIgnored = True
dryRun = True;


rootPath = "../../Source/"

def sh(cmd):
	print ("exec : "+cmd);
	res =  os.popen(cmd).read()
	print res
	return res

outWarnings = sh("cd "+rootPath+" && cppclean -i ../JuceLibraryCode -i ../External/serial/include *;")
lines = outWarnings.split('\n')
linesForward = [x for x in lines if "does not need to be #included; use a forward declaration instead" in x]
linesNoNeedInclude = [x for x in lines if (x[-29:]=="does not need to be #included") or x[-8:]=="not used"]


processedFiles  = {}
def addProcessedFile(l,typ):
	splitted = l.split(":")
	file = splitted[0]
	line = int(splitted[1])
	include = splitted[2].split("'")[1]
	className = include.split('.h')[0]
	if not file in processedFiles.keys():
		processedFiles[file] = {"forward":[],"noNeed":[]}
	processedFiles[file][typ] += [[line,className] ]

for l in linesForward:
	addProcessedFile(l,"forward")
for l in linesNoNeedInclude:
	addProcessedFile(l,"noNeed")

print processedFiles




for file,linesToChange in processedFiles.iteritems():
	with open(os.path.abspath(rootPath+file),'r') as f:
		originLines  = f.readlines()
	def checkInnerMethod(className,originLines):
		if(className in globalIgnore):
			if showIgnored : print ' \t ignoring '+className
			return True;
		for l in originLines:
			if className+"::" in l:
				if showIgnored :print '\t  using internal class Call for '+className
				return True;
		return False

	for l in linesToChange["noNeed"]:
		line =l[0]-1
		className = l[1]
		if not '//keep' in originLines[line]:
			if not (file in [className+"UI.h",className+"Editor.h"]) and not checkInnerMethod(className,originLines):
				print "deleting "+className + " from : "+file
				originLines[line] = ""
		else:
			if showIgnored : print " \t prevent delete "+className + " from : "+file

	for l in linesToChange["forward"]:
		line =l[0]-1
		className = l[1]
		if not '//keep' in originLines[line]:
			if not (file in [className+"UI.h",className+"Editor.h"]) and not checkInnerMethod(className,originLines):
				print "forwarding "+className + " from : "+file
				originLines[line] = "class "+className+";\n"
		else:
			if showIgnored : print " \t prevent forward "+className + " from : "+file

	if not dryRun:
		with open(os.path.abspath(rootPath+file),'w') as f:
			f.writelines(originLines)

