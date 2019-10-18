#ifndef SERIALIZE_H
#define	SERIALIZE_H

#define NO_ERROR		0
#define	INVALID_FORMAT	1
#define	FILE_ERROR	2

#define FLAG	0x4e45

#include "net.h"

unsigned int serialize_net(NET *net, const char *path);

#endif
