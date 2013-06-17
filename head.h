#ifndef __HEAD_H__
#define	__HEAD_H__
#include <string.h>
#include <errno.h>
#define prerr(commname)\
	do{\
		printf("file:%s line:%d %s %s %s\n",\
		__FILE__, __LINE__, __func__, commname, strerror(errno));\
	}while(0)
#endif
