#include<stdio.h>
#include<iostream>
#include"neoCoLib.h"
#include "neoDebug.h"




int main(){
	NEO_START;

	NEO_TITLE(main);

	string str =  NCL::GetStaticStrForm("test %s","aa");

	cout << str.c_str() << endl;



	string aaa;
	cin >> aaa;

	return 0;
}

