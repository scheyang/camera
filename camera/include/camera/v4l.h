#ifndef _V4L_H
#define _V4L_H
/*
 * ./v4l.h
 *
 * Copyright (C) 2013 Chris he.
 *
 * This file is placed under the LGPL.
 *
 * $Id: v4l.h,v 1.0 2013
 *
 * camera library interface definitions.
 */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <linux/types.h>
#include <unistd.h>
#include <sys/mman.h>

#include <string.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <malloc.h>
#include <sys/time.h>

/*�ں����õ�ӳ����ʼ��ַ*/
struct videobuffer{    
	void   			*start; //��ַ    
	unsigned long   length; //����
};

/*****v4l �����Ϣ*********/
struct v4l_info{
	int v4l_num; 					//���ں� request numbers of buf ��Ĭ��4
	struct v4l2_buffer v4l2_buf;  	//v4l2_buffer
	struct videobuffer *video_buf;	//mmap video_buf ��Ϣ
	int wide; 	 					//Ĭ��320
	int high;  						//Ĭ��240
};

/*camera ����ṹ��*/
struct camera{
	int fd;							//������
	char dev_name[20];  			//Ĭ��/dev/vide0 
	struct v4l_info v4l_info;		//v4l�����Ϣ�ṹ��
	struct camera *pdev;			
/*************************************************************************************/
/*���ܣ����camera�豸���˳�����									 */
/*���룺struct camera * dev  �Ľṹ��ָ�룬����ʱע��dev�Ѿ���ʼ��	 */
/*�������  														 */
/*����ֵ��0 ��ʾ�ɹ���-1 ����ʧ��									 */
/**********************************************************************************************/		
	int (*exit)(struct camera * dev);

/*************************************************************************************/
/*���ܣ����һ֡��rgb16ͼ��																*/
/*���룺struct camera * dev  �Ľṹ��ָ��												*/
/*�����rgb16��ָ��unsigned char *��ָ��												*/
/*				ʹ��ʱ�����ѿ��ٿռ��� �򴫵ݸÿռ�										*/
/*						��û�п��ٿռ����Զ����٣�����Աfree							*/
/* ����ֵ	0 ��ʾ�ɹ���-1 ����ʧ��														*/
/*eg:																					*/
/*	1> �ѿ��ٿռ�                               2> û�п��٣��Զ����٣�����Աfree		*/
/*	unsigned char buf[w*h*2];					char *buf = NULL;//���븳ֵNULL����Ȼ�δ���*/
/*	unsigned char *pbuf = buf;					get_rgb16(dev,&buf).............		*/
/*	get_rgb16(dev,&buf)							free(buf);*/
/**********************************************************************************************/	
	int (*get_rgb16)(struct camera * dev,unsigned char ** rgb_buf);		
	void * private_data; //˽������,��չ
};

/*************************************************************************************/
/*���ܣ�struct camera dev ��Ĭ�ϳ�ʼ������ 		*/
/*���룺��																				*/
/*����� struct camera * dev															*/
/*����ֵ��0 ��ʾ�ɹ���-1 ����ʧ��														*/
/*ע�������ж��䷵��ֵ																	*/
/**********************************************************************************************/			
extern int camera_info_init(struct camera * dev);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*_V4L_H_*/