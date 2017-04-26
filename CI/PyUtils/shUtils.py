import os;
import subprocess

def sh(cmd,printIt=True):
	def inSH(cmd):
		if printIt : print ("exec : "+cmd);
		res =  subprocess.Popen(cmd, shell=True,stdout=subprocess.PIPE, universal_newlines=True)
		stdout_lines = iter(res.stdout.readline, "")
		for stdout_line in stdout_lines:
			yield stdout_line

		res.stdout.close()
		return_code = res.wait()
		if return_code != 0:
			raise subprocess.CalledProcessError(return_code, cmd)
	res = ""
	for l in inSH(cmd):
		if l != '\n':
			res+=l
			print l
	return res