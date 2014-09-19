/*
 *write by Chris He
 * This file is the LCD API	
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include "lcd.h"
#include "logo.h"
#include <unistd.h>
//定义初始化接口函数
static int lcd_init(struct lcd *dev){
	/*1.打开帧缓冲设备文件*/
	dev->fd = open(dev->dev_name,O_RDWR);
	if(-1==dev->fd)
	{
		perror("open framebuffer device");
		return -1;
	}
	printf("The framebuffer device was opened successfully.\n");
	/*2.获取屏幕参数*/
	//获得可修改屏幕参数
	if(ioctl(dev->fd,FBIOGET_VSCREENINFO,&dev->vinfo))
	{
		printf("Error reading variable information.\n");
		return -1;
	}

	puts("=----------------");
	printf("left_margin:%d,right_margin:%d\n",dev->vinfo.left_margin,dev->vinfo.right_margin);
	printf("upper_margin:%d,lower_margin:%d\n",dev->vinfo.upper_margin,dev->vinfo.lower_margin);
	printf("hsync_len:%d,vsync_len:%d\n",dev->vinfo.hsync_len,dev->vinfo.vsync_len);
	printf("xres:%d,yres:%d\n",dev->vinfo.xres,dev->vinfo.yres);
	printf("xres_virtual:%d,yres_virtual:%d\n",dev->vinfo.xres_virtual,dev->vinfo.yres_virtual);
	printf("xoffset:%d,yoffset:%d\n",dev->vinfo.xoffset,dev->vinfo.yoffset);
	printf("%d:bpp\n",dev->vinfo.bits_per_pixel);
	//获得不可修改屏幕参数
	if(ioctl(dev->fd,FBIOGET_FSCREENINFO,&dev->finfo))
	{
		printf("Error reading fixed information.\n");
		return -1;
	}
	printf("line_length:%d\n",dev->finfo.line_length);
	
	 //计算屏幕分辨率
	 dev->screensize = dev->vinfo.xres * dev->vinfo.yres * dev->vinfo.bits_per_pixel / 8;  
	 
	  /*3.映射显示缓冲区到用户空间*/
	 dev->fbp = (char *)mmap(0, dev->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, dev->fd, 0);
	if (dev->fbp == MAP_FAILED) {
      	  perror("Error: dev->fbp failed to map framebuffer device to memory");
      	  return -1;
    	}
	printf("The framebuffer device was mapped to memory successfully.\n");	 
	return 0;
}
/*定义结束帧缓冲显示函数*/
static void lcd_exit(struct lcd *dev)
{
	munmap(dev->fbp, dev->screensize);
	//printf("The framebuffer device was munmapped to memory successfully.\n");
	close(dev->fd);
	printf("The framebuffer device was closed successfully.\n");
}

/*定义显示图片接口函数*/

static int show_picture_16( int x0, int x_wide, int y0, int y_high, const void *src, struct lcd *dev )
{

	int i=0;
	unsigned char *pture = (unsigned char *)src;
	unsigned long  size =x_wide * (dev->vinfo.bits_per_pixel/8);
	unsigned long location = (x0+dev->vinfo.xoffset) * (dev->vinfo.bits_per_pixel/8) + (y0+dev->vinfo.yoffset) * dev->finfo.line_length;	
	if( (x0 > x_wide) ||(y0 > y_high ) ||x0 < 0 || y0 < 0 ||(x_wide > dev->vinfo.xres)\
		||(y_high > dev->vinfo.yres)
		){
		printf("the zone size is not correct,show picture error\n");
		return -1;
	}
	for(i=y0;i <y0+y_high;i++){
		memcpy(dev->fbp+location, pture, size);
		location += dev->finfo.line_length;
		pture +=size;
	}
		
	return 0;
}
static int clear  (int x0, int x_wide, int y0, int y_high, unsigned short color, struct lcd *dev){
	int i=0,j=0;
	if( (x0 > x_wide) ||(y0 > y_high ) ||x0 < 0 || y0 < 0 ||(x_wide > dev->vinfo.xres)\
		||(y_high > dev->vinfo.yres)
		){
		printf("the clear size is not correct\n");
		return -1;
	}
	unsigned short (*p)[dev->vinfo.xres] = (unsigned short(*)[dev->vinfo.xres] )dev->fbp;
	for(j=y0;j<=y_high;j++){
		for(i = x0;i <=x_wide;i++){
			p[j][i] =color;
		}
	}
	return 0;
}
/*定义清除屏为一种颜色（color）的函数*/
static void clear_screen(unsigned short color, struct lcd *dev)
{	
		clear (0, dev->vinfo.xres, 0, dev->vinfo.yres, color, dev);
}

int  lcd_info_init(struct lcd * dev){
	
	if(strlen(dev->dev_name) <= 0 )
		strncpy(dev->dev_name,"/dev/fb0",strlen("/dev/fb0")+2);
	if(!dev->exit)
		dev->exit = &lcd_exit;
	if(!dev->show_picture_16)
		dev->show_picture_16 = &show_picture_16;
	if(!dev->clear)
		dev->clear =&clear;
	if(!dev->clear_screen)
		dev->clear_screen=&clear_screen;
	return lcd_init(dev);
}







