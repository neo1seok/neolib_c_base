#!/usr/bin/python3.6
import os
import  platform

#os = platform.system()
#if os == "Linux:
list_makefile = ["Makefile.x64","Makefile.armhf","Makefile.x86"]

for tmp in list_makefile:
	os.system("make -f %s clean"%tmp)
	os.system("make -f %s"%tmp)
		
