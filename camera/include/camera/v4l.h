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

/*内核自用的映射起始地址*/
struct videobuffer{    
	void   			*start; //地址    
	unsigned long   length; //长度
};

/*****v4l 相关信息*********/
struct v4l_info{
	int v4l_num; 					//向内核 request numbers of buf ，默认4
	struct v4l2_buffer v4l2_buf;  	//v4l2_buffer
	struct videobuffer *video_buf;	//mmap video_buf 信息
	int wide; 	 					//默认320
	int high;  						//默认240
};

/*camera 对象结构体*/
struct camera{
	int fd;							//描述符
	char dev_name[20];  			//默认/dev/vide0 
	struct v4l_info v4l_info;		//v4l相关信息结构体
	struct camera *pdev;			
/*************************************************************************************/
/*功能：完成camera设备的退出功能									 */
/*输入：struct camera * dev  的结构体指针，传参时注意dev已经初始化	 */
/*输出：无  														 */
/*返回值：0 表示成功，-1 代表失败									 */
/**********************************************************************************************/		
	int (*exit)(struct camera * dev);

/*************************************************************************************/
/*功能：获得一帧的rgb16图像																*/
/*输入：struct camera * dev  的结构体指针												*/
/*输出：rgb16，指向unsigned char *的指针												*/
/*				使用时：若已开辟空间则 则传递该空间										*/
/*						若没有开辟空间则自动开辟，程序员free							*/
/* 返回值	0 表示成功，-1 代表失败														*/
/*eg:																					*/
/*	1> 已开辟空间                               2> 没有开辟，自动开辟，程序员free		*/
/*	unsigned char buf[w*h*2];					char *buf = NULL;//必须赋值NULL，不然段错误*/
/*	unsigned char *pbuf = buf;					get_rgb16(dev,&buf).............		*/
/*	get_rgb16(dev,&buf)							free(buf);*/
/**********************************************************************************************/	
	int (*get_rgb16)(struct camera * dev,unsigned char ** rgb_buf);		
	void * private_data; //私有数据,扩展
};

/*************************************************************************************/
/*功能：struct camera dev 的默认初始化操作 		*/
/*输入：无																				*/
/*输出： struct camera * dev															*/
/*返回值：0 表示成功，-1 代表失败														*/
/*注：必须判断其返回值																	*/
/**********************************************************************************************/			
extern int camera_info_init(struct camera * dev);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*_V4L_H_*/