import sys
import os
import time
from shUtils import *


gitShaPath = os.path.dirname(os.path.abspath(__file__));
gitShaPath = os.path.join(gitShaPath,"../../Source/GitSha.h");
gitShaPath = os.path.abspath(gitShaPath);

def writeSha():
	sha = sh("git rev-parse --verify HEAD")[:-1]
	date =  time.strftime("%d/%m/%y (%H:%M)")

	with open(gitShaPath,'w') as f:
		f.write('#pragma once\n')
		f.write('#define GIT_SHA  "'+sha+'"\n')
		f.write('#define GIT_SHA_SHORT  "'+sha[0:7]+'"\n')
		f.write('#define COMMIT_DATE  "'+date+'"\n')


if __name__=='__main__':
	writeSha()
	sh("git add "+gitShaPath)
	exit(1)

