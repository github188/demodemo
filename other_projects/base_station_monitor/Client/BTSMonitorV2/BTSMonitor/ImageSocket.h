#ifndef IMAGE_SOCKET_H
#define IMAGE_SOCKET_H

#include "stdafx.h"
#include "GokuClient.h"

class ImageSocket: public GokuSocket
{
public:
	ImageSocket(CString &ps, CString &ss): GokuSocket(ps, ss){}
	int read_buffer(char *buffer, int size);
};

#endif