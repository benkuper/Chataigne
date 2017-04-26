import urllib
import os,sys
import json

pathToAdd = os.path.abspath(os.path.join(__file__,os.path.pardir,os.path.pardir))
sys.path.insert(1,pathToAdd)

from PyUtils import *

import multiprocessing
njobs = multiprocessing.cpu_count()

execName = "Chataigne"
localMakePath = os.path.abspath(os.path.join(__file__,os.path.pardir,os.path.pardir,os.path.pardir,"Builds/LinuxMakefile/"))+'/'
localExportPath = localMakePath+'build/'
localAppFile = localExportPath+execName
localExportFile = localAppFile+".tar.gz"

print localMakePath


def buildApp(configuration):
	sh('cd '+localMakePath+' && make CONFIG='+configuration+' -j'+str(njobs))


def exportApp(baseName):
	sh('tar -zcvf '+localExportFile+' --directory="'+localExportPath+'" '+execName)
	return localExportFile



