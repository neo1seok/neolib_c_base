g++ -o test test.cpp -lneo_c_lib -ldl -D_DEFNEODEF_  -DNEODEBUG_ -Ineo_c_lib
g++ -o test2 test.cpp /usr/local/lib/libneo_c_lib.a -ldl -D_DEFNEODEF_  -DNEODEBUG_ -Ineo_c_lib

#g++ -o test test.cpp -L../lib/gnu -lneolib_c_base -ldl -D_DEFNEODEF_  -DNEODEBUG_ -Ineo_c_lib
#g++ -o test test.c -Llib/gnu -lIUST-100 -lslab_usb_spi -ldl -D_DEFNEODEF_  -DNEODEBUG
