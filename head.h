#ifndef __HEAD_H__
#define	__HEAD_H__
#define prerr(call)\
	do{\
		printf("file:%s line:%d %s %s %s\n",\
		__FILE__, __LINE__, __func__, call, strerror(errno));\
	}while(0)
#endif
