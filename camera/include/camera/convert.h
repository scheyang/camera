#ifndef _CONVERT_H__
#define _CONVER_H__

typedef struct bmp{
	unsigned char *start;
	unsigned int length;
}BMP;

extern int yuyvtorgb16(const unsigned char *yuyv,const unsigned int w,const unsigned int h,unsigned char **rgb16);

#endif /*_CONVERT_H__*/
