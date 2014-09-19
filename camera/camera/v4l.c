/* 
 *  ./camera.c
 * 
 *  Copyright (C) 2013 Chris he.
 * 
 * This file is placed under the GPL.  Please see the file 
 * COPYING for more details. 
 *
 * Utility routines for v4l
 */

#include "v4l.h"
#include "convert.h"
//#define S5P_CAMERA 1
#include <linux/videodev.h>

static int camera_init(struct camera * dev)
{
	int ret;
#ifdef _CAMERA_DBUG_ 
	puts("int camera_init ");
#endif
	//open dev
	puts(dev->dev_name);
	dev->fd= open(dev->dev_name, O_RDWR, 0);
	if(dev->fd == -1) {
		dev->fd = open("/dev/video100", O_RDWR, 0);
			if(dev->fd== -1){
				printf("open camera failed\n");
				exit(-1);
		}
	}
	printf("open camera  success\n");
//#ifndef  S5P_CAMERA
	//检查当前视频设备支持的标准
	//在亚洲，一般使用PAL（720X576）制式的摄像头，而欧洲一般使用NTSC（720X480），使用VIDIOC_QUERYSTD来检测：
	v4l2_std_id std; 
	do 
	{ 
		ret = ioctl(dev->fd, VIDIOC_QUERYSTD, &std); 
		} 
	while (ret == -1 && errno == EAGAIN); 
	switch (std) 
	{ 
		case V4L2_STD_NTSC: 
			printf("the device is NTSC\n");
		case V4L2_STD_PAL: 
			printf("the device is PAL\n");
		default:
     			printf("camera using unsupported video standard\n");
    	//	    return -1;
	}
//#endif

	//获取摄像头参数
//#ifndef S5P_CAMERA
	struct v4l2_fmtdesc fmtdesc;
	memset(&fmtdesc,0,sizeof(struct v4l2_fmtdesc ));
	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	while ((ret = ioctl(dev->fd, VIDIOC_ENUM_FMT, &fmtdesc)) == 0) 
	{
		fmtdesc.index++;
		printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",
				fmtdesc.pixelformat & 0xFF, (fmtdesc.pixelformat >> 8) & 0xFF,
				(fmtdesc.pixelformat >> 16) & 0xFF,
				(fmtdesc.pixelformat >> 24) & 0xFF, fmtdesc.description);
	}
	struct v4l2_capability cap;
	ret = ioctl(dev->fd, VIDIOC_QUERYCAP, &cap); 
	if (ret < 0) {
		perror("ioctl : VIDIOC_QUERYCAP failed\n");
		return -1;
	}
	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
	{		
		perror("V4L2_CAP_VIDEO_CAPTURE failed!\n");		
		return -1;
	}	
	if (!(cap.capabilities & V4L2_CAP_STREAMING))
	{		
		perror("V4L2_CAP_STREAMING failed!\n");	
		return -1;
	}	
//#endif
//#ifndef S5P_CAMERA
#ifndef CAMERA
//--------------------------------------------------------
	
	//设置视频捕获格式
	//当检测完视频设备支持的标准后，还需要设定视频捕获格式：
	struct v4l2_format fmt; 
	memset ( &fmt, 0, sizeof(fmt) ); 
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	fmt.fmt.pix.width = dev->v4l_info.wide; 
	fmt.fmt.pix.height = dev->v4l_info.high; 
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; 
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED; //???????
	if (ioctl(dev->fd, VIDIOC_S_FMT, &fmt) == -1) 
	{
		printf("ioctl : VIDIOC_S_FMT failed\n");
		return -1; 
	}
	printf("format sucess\n");
#endif

	struct v4l2_requestbuffers reqbuff;
	reqbuff.count =dev->v4l_info.v4l_num;
	reqbuff.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuff.memory = V4L2_MEMORY_MMAP;
	
	//分配内存接下来可以为视频捕获分配内存： 
	if (ioctl(dev->fd, VIDIOC_REQBUFS, &reqbuff) == -1) 
	{ 
		perror("ioctl");
		printf("ioctl : VIDOC_REQBUFS failed\n");
		close(dev->fd);
		return -1;
	}
	dev->v4l_info.v4l_num = reqbuff.count;
	printf("request the v4l reqbuff is %d\n",dev->v4l_info.v4l_num);
	
	//mmap 
	dev->v4l_info.video_buf = calloc(reqbuff.count , sizeof(*dev->v4l_info.video_buf) );
		if(dev->v4l_info.video_buf== NULL) {
				printf("video calloc  buffers failed\n");
				return -1;
		}
	int numBufs;	
	for (numBufs = 0; numBufs < reqbuff.count; numBufs++) 
		{    
			memset( &dev->v4l_info.v4l2_buf, 0, sizeof(struct v4l2_buffer) );   
			dev->v4l_info.v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;    
			dev->v4l_info.v4l2_buf.memory = V4L2_MEMORY_MMAP;    
			dev->v4l_info.v4l2_buf.index = numBufs;        
			if (ioctl(dev->fd, VIDIOC_QUERYBUF, &dev->v4l_info.v4l2_buf) == -1) 
			{ 
				printf("video init VIDIOC_QUERYBUF error\n");
				return -1;    
				}
			dev->v4l_info.video_buf[numBufs].length = dev->v4l_info.v4l2_buf.length;    // 转换成相对地址    
			dev->v4l_info.video_buf[numBufs].start = mmap(NULL, dev->v4l_info.v4l2_buf.length, PROT_READ | PROT_WRITE,MAP_SHARED,dev->fd, dev->v4l_info.v4l2_buf.m.offset); 
			if (dev->v4l_info.video_buf[numBufs].start == MAP_FAILED) 
			{        
				printf("video init mmap failed\n");
				close(dev->fd);	
				return -1;    
				}   
			// 放入缓存队列    
			if (ioctl(dev->fd, VIDIOC_QBUF, &dev->v4l_info.v4l2_buf) == -1) 
			{
				perror("video init ioctl VIDIOC_QBUF failed\n");
	        		return -1;    
			}
	}

	printf("init success\n");
	return 0;
}


//启动视频采集
static int camera_start(struct camera * dev)
{
	//启动视频采集
	enum v4l2_buf_type  type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	if( ioctl(dev->fd, VIDIOC_STREAMON, &type) == -1) {
		perror("ioctl stream start failed\n");
		return -1;
	}
	printf("start camera success\n");
	
	return 0;
}

static int camera_stop(struct camera * dev)
{
	enum v4l2_buf_type  type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	//关闭采集流
	int ret = ioctl(dev->fd, VIDIOC_STREAMOFF, &type); 
	if (ret){ 
		printf("ioctl : VIDIOC_STREAMOFF"); 
		return -1;
	} 
	printf("stop the  camera \n");
	return 0;
}

static int camera_exit(struct camera * dev)
{	

	camera_stop(dev);
	
	//解映射
	int numBufs ;
	for( numBufs = 0;numBufs < dev->v4l_info.v4l_num; numBufs++)
		munmap(dev->v4l_info.video_buf[numBufs].start, dev->v4l_info.video_buf[numBufs].length);
	free(dev->v4l_info.video_buf);
	close(dev->fd);
	printf("camera closed...\n");
	return 0;
}

static  int get_rgb16(struct camera * dev,unsigned char ** rgb_buf){

	if (-1 == ioctl(dev->fd, VIDIOC_DQBUF, &dev->v4l_info.v4l2_buf)) {
		printf("fun  %s: get rgb16 ioctl: VIDIOC_DQBUF\n",__FUNCTION__);	
		return -1;
	}

	yuyvtorgb16(dev->v4l_info.video_buf[dev->v4l_info.v4l2_buf.index].start, dev->v4l_info.wide, dev->v4l_info.high, rgb_buf);

	//重新入队
	if (-1 == ioctl(dev->fd, VIDIOC_QBUF, &dev->v4l_info.v4l2_buf)) {
		printf("fun  %s:get rgb16 ioctl: VIDIOC_QBUF   return queue failed\n",__FUNCTION__);
		return -1;
	}
	return 0;
 }
int  camera_info_init(struct camera * dev){
	
	if(strlen(dev->dev_name) <= 0 )
		strncpy(dev->dev_name,"/dev/video100",strlen("/dev/video100")+2);
	if(dev->v4l_info.wide <= 0 )
		dev->v4l_info.wide = 320;
	if(dev->v4l_info.high <=0 )
		dev->v4l_info.high =240;
	if(dev->v4l_info.v4l_num<=0)
		dev->v4l_info.v4l_num =4;
	
	if(dev->exit==NULL)
		dev->exit   =&camera_exit;
	
	if(dev->get_rgb16==NULL)
		dev->get_rgb16 = &get_rgb16;
	
	if(camera_init(dev) <0)
		return -1;
	return camera_start(dev) ;
}
