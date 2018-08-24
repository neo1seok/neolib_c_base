#! /usr/bin/python3
import sys
import os
import shutil
print(sys.path)
print(sys.argv)
is_no_force = sys.argv.__len__() >1 and sys.argv[1] == "-f"
print("is_no_force",is_no_force)	
str_list_incldue ="""comdef.h
CSerialBASE.h
CSerialRS232.h
neoCoLib.h
neoDebug.h
NeoSystemLib.h"""

list_include = str_list_incldue.split("\n")
print(list_include)
if not is_no_force:
	input("ready to copy?")
for inc_file in list_include:
	org_file = "include/"+inc_file
	dst_file = "../include/"+inc_file
	print("copy",org_file,"to",dst_file)
	if not os.path.exists('../include'):
		os.makedirs("../include")
	
	shutil.copy(org_file,dst_file)
if not is_no_force:
	input("ok")	

