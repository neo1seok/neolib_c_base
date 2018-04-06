
###############################################
# neo_c_lib
###############################################


#------------------------------------------------------------------------#
# main definition
#------------------------------------------------------------------------#
CC = gcc
CXX = g++
AR = ar
LD = $(CXX)

TITLE=neo_c_lib



#------------------------------------------------------------------------#
# directory definition
#------------------------------------------------------------------------#
OUT_DIR = ../lib/gnu
#example : ../lib/gnu

DST_DIR = /usr/local
#example : /usr/local

DST_DIR_LIB = $(DST_DIR)/lib
DST_DIR_INC = $(DST_DIR)/include/$(TITLE)

#------------------------------------------------------------------------#
# result definition
#------------------------------------------------------------------------#
OUT_STATIC_LIB = $(OUT_DIR)/lib$(TITLE).a
OUT_SHARED_LIB = $(OUT_DIR)/lib$(TITLE).so
OUT_EXE = $(OUT_DIR)/$(TITLE)


#------------------------------------------------------------------------#
# flag definition
#------------------------------------------------------------------------#
INC = -Iinclude -I../include
#example : -IAAA -IBBB

CDEFINE=-D_USE_UTF8_ -DNEOUSEMBCS -DNEODEBUG -DNEO_STATIC -DLSA_EXPORTS_NOUSE
#example -DBBBB -DCCCCCC

CFLAGS=-Wall -O2 -fPIC -Wl,-Bsymbolic -std=gnu++11
#example -Wall -O2 -fPIC -Wl,-Bsymbolic -std=gnu++11

LDFLAGS = -shared -fPIC  -L../lib/gnu -ldl
#example -shared -fPIC  -L../lib/gnu -ldl

CFLAG_ALL = $(INC) $(CDEFINE) $(CFLAGS)

#------------------------------------------------------------------------#
# object directory definition
#------------------------------------------------------------------------#

OBJDIR____SRC_CPP = obj/gnu____src_cpp

#example
#OBJDIR_OBJ0 = obj/gnu_obj0
#OBJDIR_OBJ1 = obj/gnu_obj1

DEF_RELEASE_INC= ../include


#------------------------------------------------------------------------#
# object set definition
#------------------------------------------------------------------------#

OBJ_SET____SRC_CPP =  $(OBJDIR____SRC_CPP)/CSerialRS232.o  $(OBJDIR____SRC_CPP)/EtcModule.o  $(OBJDIR____SRC_CPP)/MemoryNode.o  $(OBJDIR____SRC_CPP)/NeoCoLib.o  $(OBJDIR____SRC_CPP)/neoDebug.o  $(OBJDIR____SRC_CPP)/rs232.o  $(OBJDIR____SRC_CPP)/SystemUtil.o  $(OBJDIR____SRC_CPP)/SystemUtil4Linux.o 

#example OBJ_SET_OBJ0 =  $(OBJDIR)/CSerialRS232.o  $(OBJDIR)/EtcModule.o  $(OBJDIR)/MemoryNode.o  $(OBJDIR)/NeoCoLib.o 



	
#------------------------------------------------------------------------#
# all object definition
#------------------------------------------------------------------------#		
OBJ_SET_ALL =  $(OBJ_SET____SRC_CPP)   
#example OBJ_SET_ALL =  $(OBJ_SET_OBJ0) $(OBJ_SET_OBJ1) $(OBJ_SET_OBJ2)


#------------------------------------------------------------------------#
# command option definition
#------------------------------------------------------------------------#		

all: static share
install:  lib_install share_install

clean: clean_release 

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OUT_DIR) || mkdir -p $(OUT_DIR)
	test -d $(OBJDIR____SRC_CPP) || mkdir -p $(OBJDIR____SRC_CPP)
	

	


after_release: 
	echo 'none'
	#mkdir -p ../include
	#cp -pr ../include/*.h $(DST_DIR)/include
	
before_install: 
	test -d $(DST_DIR_LIB) || mkdir -p $(DST_DIR_LIB)
	test -d $(DST_DIR_INC) || mkdir -p $(DST_DIR_INC)
	cp -p ../include/*.h  $(DST_DIR_INC)
	

	

static: before_release out_release after_release
share: before_release out_shared_release after_release
exe: before_release out_exe_release after_release


lib_install:before_install 
	test -d $(OUT_STATIC_LIB) || cp -p $(OUT_STATIC_LIB)  $(DST_DIR_LIB)

	
share_install:before_install 
	test -d $(OUT_SHARED_LIB) ||cp -p $(OUT_SHARED_LIB)  $(DST_DIR_LIB)
	#ldconfig
	#ln -s $(OUT_SHARED_RELEASE) $(OUT_SHARED_RELEASE).0.0.1

		

out_release: $(OBJ_SET_ALL)
	$(AR) rcs $(OUT_STATIC_LIB) $(OBJ_SET_ALL) $(LIB)
	
	
out_shared_release: $(OBJ_SET_ALL)
	echo $(OUT_SHARED_RELEASE)
	echo $(LDFLAGS)
	$(CXX)  --shared $(LDFLAGS) -o $(OUT_SHARED_LIB) $(OBJ_SET_ALL) $(LIB)
	#cp -p $(OUT_SHARED_RELEASE)  $(DST_DIR_LIB)

out_exe_release: $(OBJ_SET_ALL)
	echo $(OUT_SHARED_RELEASE)
	echo $(LDFLAGS)
	$(CXX)  $(LDFLAGS) -o $(OUT_EXE) $(OBJ_SET_ALL) $(LIB)

	
$(OBJDIR____SRC_CPP)/%.o: ../src/%.cpp
	$(CXX) $(CFLAG_ALL)  -c -o $@ $<	
	
	
	

clean_release: 
	rm -f $(OBJ_SET_ALL)
	rm -f $(OBJDIR)


.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

