// Wrapper4CPP.h

#pragma once

using namespace System;
using namespace System::Text;
using namespace System::Runtime::InteropServices;
#include "neoCoLib.h"

namespace NeoUtil4CRL {
	
	public ref class CNeoUtil4CRL
	{
		
	private:
		// TODO: 여기에 이 클래스에 대한 메서드를 추가합니다.
	protected:
		

	public:
		
		

		static unsigned char * arraybytetoptr(array<Byte>  ^input){
			if (input->Length == 0) return NULL;
			pin_ptr<System::Byte> p = &input[0];
			unsigned char* pby = p;
			return reinterpret_cast<unsigned char*>(pby);

		}
		static unsigned char * ArrayBytetoBYTE(array<System::Byte>^ byteArray){
			return ArrayBytetoBYTE(byteArray, 0);
		}
		static unsigned char * ArrayBytetoBYTE(array<System::Byte>^ byteArray,int startindex)
		{
			if (byteArray->Length == 0) return NULL;
			pin_ptr<System::Byte> p = &byteArray[startindex];
			unsigned char* pby = p;
			unsigned char* pch = reinterpret_cast<unsigned char*>(pby);
			return pch;


		}
		static array<System::Byte>^ BYTEtoArrayByte(unsigned char * pbuff, int buffSize)
		{
			array<Byte> ^ dest_array = gcnew array<Byte>(buffSize);
			memcpy(ArrayBytetoBYTE(dest_array),pbuff,buffSize);

			return dest_array;


		}
		static short GetCRC(array<Byte>  ^value){


			unsigned char* data = ArrayBytetoBYTE(value);
			return calcCRC(data, value->Length);

		}

		static short GetCRCInverse(array<Byte>  ^value){


			unsigned char* data = ArrayBytetoBYTE(value);
			short crc = calcCRC(data, value->Length);
			NCL::InverBits(&crc, 2);
			return crc;

		}

		

		static void StringToStlString(String^ managedString, string & outstring){

			// Marshal the managed string to unmanaged memory.
			char* stringPointer = (char*)Marshal::StringToHGlobalAnsi(managedString).ToPointer();
			outstring = stringPointer;
			// Always free the unmanaged string.
			Marshal::FreeHGlobal((IntPtr)stringPointer);


		}

		static array<Byte>^ hexStr2BYTE(String ^ hesStr){
			if (hesStr->Length == 0) gcnew array<Byte>(0);

			string stlhexstr = "";
			StringToStlString(hesStr, stlhexstr);

			unsigned char * buff = new unsigned char[stlhexstr.length()];

			int size = hex2bin(stlhexstr.c_str(), buff);
			return BYTEtoArrayByte(buff, size);
		}

		static String ^ GetSHA256(array<Byte>  ^org){


			string sharet = GetSHA256Hexstr(ArrayBytetoBYTE(org), org->Length);
			String ^ ret = gcnew String(sharet.c_str());

			return ret;

		}
		static String ^ GetHMAC(String ^ hexMsg, String ^ hexKey ){
			
			string stlhexMsg = "";
			string stlhexKey = "";
			StringToStlString(hexMsg, stlhexMsg);
			StringToStlString(hexKey, stlhexKey);

			string sharet = ::GetHMAC(stlhexMsg, stlhexKey);
			String ^ ret = gcnew String(sharet.c_str());

			return ret;

		}
		static void CopyByte(String ^ hexStr, unsigned char * buff, int buffsize){
			array<Byte>  ^bytebf = hexStr2BYTE(hexStr);

			if (bytebf->Length != buffsize) return;

			memcpy(buff, arraybytetoptr(bytebf), buffsize);

		}

	};

	
}
