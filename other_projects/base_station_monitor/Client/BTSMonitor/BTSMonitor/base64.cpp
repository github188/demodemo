#include "stdafx.h"
#include "base64.h"

const BYTE Base64IdxTab[128] =
{
	255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
	255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
	255,255,255,255,  255,255,255,255,  255,255,255,62,   255,255,255,63,
	52,53,54,55,      56,57,58,59,      60,61,255,255,    255,255,255,255,
	255,0,1,2,        3,4,5,6,          7,8,9,10,         11,12,13,14,
	15,16,17,18,      19,20,21,22,      23,24,25,255,     255,255,255,255,
	255,26,27,28,     29,30,31,32,      33,34,35,36,      37,38,39,40,
	41,42,43,44,      45,46,47,48,      49,50,51,255,     255,255,255,255
};

int DecodeBase64(char * pInput, char * pOutput)
{
	int i = 0;
	int iCnt = 0;
	int iSrcLen = (int)strlen(pInput);

	char * p = pOutput;

	for (i=0; i < iSrcLen; i++)
	{
		if (pInput[i] > 127) continue;
		if (pInput[i] == '=') return p-pOutput+1;

		BYTE a = BVal(pInput[i]);
		if (a == 255) continue;
		
		switch (iCnt)
		{
		case 0:
			{
				*p = a << 2;
				iCnt++;
			}
			break;

		case 1:
			{
				*p++ |= a >> 4;
				*p = a << 4;
				iCnt++;
			}
			break;

		case 2:
			{
				*p++ |= a >> 2;
				*p = a << 6;
				iCnt++;
			}
			break;
		case 3:
			{
				*p++ |= a;
				iCnt = 0;
			}
			break;
		} 
	}

	*p = 0x00;
	return p-pOutput;
}
