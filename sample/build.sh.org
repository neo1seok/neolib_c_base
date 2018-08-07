export LIB_PATH=../lib/gnu-x64
g++ -o test test.cpp -L$LIB_PATH -lneo_c_lib -ldl -D_DEFNEODEF_  -DNEODEBUG_ -I../include
g++ -o test2 test.cpp $LIB_PATH/libneo_c_lib.a -ldl -D_DEFNEODEF_  -DNEODEBUG_ -I../include

#g++ -o test test.cpp -L../lib/gnu -lneolib_c_base -ldl -D_DEFNEODEF_  -DNEODEBUG_ -Ineo_c_lib
#g++ -o test test.c -Llib/gnu -lIUST-100 -lslab_usb_spi -ldl -D_DEFNEODEF_  -DNEODEBUG
