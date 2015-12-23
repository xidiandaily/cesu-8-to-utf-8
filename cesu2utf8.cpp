#include "cesu2utf8.h"

static unsigned char UCS2toUTF8Code(unsigned short ucs2_code, unsigned char * utf8_code)
{
	unsigned char length = 0;
	unsigned char * out = utf8_code;
	if(!utf8_code)
	{
		return length;
	}
	if(0x0080 > ucs2_code)
	{
		/* 1 byte UTF-8 Character.*/
		*out = (unsigned char)ucs2_code;
		length++;   
	}
	else if(0x0800 > ucs2_code)
	{
		/*2 bytes UTF-8 Character.*/
		*out = ((unsigned char)(ucs2_code >> 6)) | 0xc0;
		*(out+1) = ((unsigned char)(ucs2_code & 0x003F)) | 0x80;
		length += 2;
	}
	else
	{
		/* 3 bytes UTF-8 Character .*/
		*out = ((unsigned char)(ucs2_code >> 12)) | 0xE0;
		*(out+1) = ((unsigned char)((ucs2_code & 0x0FC0)>> 6)) | 0x80;
		*(out+2) = ((unsigned char)(ucs2_code & 0x003F)) | 0x80;

		length += 3;			   
	}
	return length;

}

std::string Code2UTF8(const std::string & msg)
{
    std::string resPacket;
    std::string str = msg;	
    int nBeginIndex = str.find("\\u");
    if(nBeginIndex == -1)
    {
        nBeginIndex = str.find("\\U");	

    }

    resPacket += str.substr(0, nBeginIndex);
    std::string strUtf;
    while(nBeginIndex != -1)
    {
        std::string strTemp = str.substr(nBeginIndex+2, 4);
        unsigned short n;
        char * stop;
        n = strtol(strTemp.c_str(), &stop, 16);
        unsigned char buf[10] = {0};
        UCS2toUTF8Code(n, buf);
        resPacket += (char *)buf;
        //emoji (\xED[\xA0-\xAF][\x80-\xBF]\xED[\xB0-\xBF][\x80-\xBF])
        if(strUtf.size())
        {
            strUtf +=(char*)buf;
            if(buf[0]==0xED&&((buf[1]&0xF0)==0xB0)&&(0x80<=buf[2]&&buf[2]<=0xBF))
            {
                char emoji_utf[10]={0};
                char pin[7]={0};
                memcpy(pin,strUtf.c_str(),6);
                *(pin+1) +=1;
                *emoji_utf=0xF0|(*(pin+1)&0x1C)>>2;
                *(emoji_utf+1)=0x80|(*(pin+1)&0x03)<<4 | (*(pin+2)&0x3C)>>2;
                *(emoji_utf+2)=0x80|(*(pin+2)&0x03)<<4 | (*(pin+4)&0x0F);
                *(emoji_utf+3)=*(pin+5);

                int Pos=resPacket.length()-strUtf.length();
                resPacket.replace(Pos,resPacket.length(),emoji_utf);
            }
            strUtf.clear();
        }
        else if(buf[0]==0xED &&((buf[1]&0xF0)==0xA0)&&(0x80<=buf[2] && buf[2]<=0xBF))
        {
            strUtf +=(char*)buf;
        }

        str = str.substr(nBeginIndex+6, str.size()-nBeginIndex-6);
        nBeginIndex = str.find("\\u");	
        if(nBeginIndex == -1)
        {
            nBeginIndex = str.find("\\U");	

        }
        if(nBeginIndex != -1)
        {
            resPacket += str.substr(0, nBeginIndex);
        }
        else
        {
            resPacket += str;
        }
    }

    return resPacket;
}


