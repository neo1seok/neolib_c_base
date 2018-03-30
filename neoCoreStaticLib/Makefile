
###############################################
# neo_c_lib
###############################################

CC = gcc
CXX = g++
AR = ar
LD = $(CXX)

NAME=neo_c_lib
TITLE=lib$(NAME)


OBJDIR = obj/gnu

OUTDIR = ../lib/gnu


##############################################################

DST_DIR = /usr/local
DST_BIN_LIB = $(DST_DIR)/lib
DST_BIN_INC = $(DST_DIR)/include/$(NAME)




INC = -Iinclude -I../include

CDEFINE=-D_USE_UTF8_ -DNEOUSEMBCS -DNEODEBUG -DNEO_STATIC -DLSA_EXPORTS_NOUSE
CFLAGS=-Wall -O2 -fPIC -Wl,-Bsymbolic -std=gnu++11
LDFLAGS = -shared -fPIC  -L../lib/gnu -ldl

CFLAG_ALL = $(CDEFINE) $(CFLAGS) $(INC)



OUT_RELEASE = $(OUTDIR)/$(TITLE).a
OUT_SHARED_RELEASE = $(OUTDIR)/$(TITLE).so
OUT_DEBUG = $(OUTDIR)/$(TITLE)d.a
OUT_SHARED_DEBUG = $(OUTDIR)/$(TITLE)d.so




OBJ_RELEASE = $(OBJDIR)/CSerialRS232.o \
	$(OBJDIR)/EtcModule.o \
	$(OBJDIR)/MemoryNode.o \
	$(OBJDIR)/NeoCoLib.o \
	$(OBJDIR)/neoDebug.o \
	$(OBJDIR)/rs232.o \
	$(OBJDIR)/SystemUtil.o \
	$(OBJDIR)/SystemUtil4Linux.o 		

					
OBJ_RELEASE_ALL =  $(OBJ_RELEASE)  

all: static share
install: lib_install share_install

clean: clean_release 

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OUTDIR) || mkdir -p $(OUTDIR)
	test -d $(OBJDIR) || mkdir -p $(OBJDIR)


after_release: 
	echo 'none'
	#mkdir -p ../include
	#cp -pr ../include/*.h $(DST_DIR)/include
	

static: before_release out_release after_release
share: before_release out_shared_release after_release



lib_install:
	cp -p $(OUT_RELEASE)  $(DST_BIN_LIB)
	cp -pr ../include/*.h $(DST_BIN_INC)
	
share_install:
	cp -p $(OUT_SHARED_RELEASE)  $(DST_BIN_LIB)
	mkdir -p $(DST_BIN_INC)
	cp -pr ../include/*.h $(DST_BIN_INC)
	ldconfig
	#ln -s $(OUT_SHARED_RELEASE) $(OUT_SHARED_RELEASE).0.0.1

		

out_release: $(OBJ_RELEASE_ALL)
	$(AR) rcs $(OUT_RELEASE) $(OBJ_RELEASE_ALL) $(LIB)
	
	
out_shared_release: $(OBJ_RELEASE_ALL)
	echo $(OUT_SHARED_RELEASE)
	echo $(LDFLAGS)
	$(CXX) ${LDFLAGS} -o $(OUT_SHARED_RELEASE) $(OBJ_RELEASE_ALL) $(LIB)
	#cp -p $(OUT_SHARED_RELEASE)  $(DST_BIN_DIR)


	
$(OBJDIR)/%.o: ../src/%.cpp
	$(CXX) $(CFLAG_ALL)  -c -o $@ $<	
	
	

clean_release: 
	rm -f $(OBJ_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR)/src
	rm -rf $(OBJDIR)
	rm -rf $(OBJDIR2)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release


