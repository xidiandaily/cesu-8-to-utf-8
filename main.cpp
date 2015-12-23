#include <stdio.h>
#include <string>
#include "cesu2utf8.h"

using namespace std;

char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',   'B','C','D','E','F'};
std::string byte_2_str(char* bytes, int size) 
{
	std::string str;
	for (int i = 0; i < size; ++i) 
	{
		const char ch = bytes[i];
		str.append("\\x");
		str.append(&hex[(ch  & 0xF0) >> 4], 1);
		str.append(&hex[ch & 0xF], 1);
	}
	return str;
}

int main(int argc,char* argv[])
{
    if(argc<=1)
    {
        printf("./cesu2utf8 '\\uD83D\\uDE01'\n");
        return 0;
    }

    char  buff[10]={0};
    string strcesu=argv[1];
    string strutf8=Code2UTF8(strcesu);
    string res_emoji_hex=byte_2_str((char*)strutf8.c_str(),strutf8.length());

    printf("input:%s\n",strcesu.c_str());
    printf("result:%s\n",res_emoji_hex.c_str());
    return 0;
}

