#ifndef 	_LCD_H_
#define 	_LCD_H_
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
#include <linux/fb.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

/*lcd 对象结构体*/
struct lcd {
	int fd;							//lcd 文件描述符
	char dev_name[20];				//默认/dev/fb0
	int screensize;					//屏幕尺寸，初始化后自动获得
	char *fbp;						//屏幕映射后地址，初始化后获得
	struct lcd *pdev;
	struct fb_var_screeninfo vinfo; //记录可修改的显示控制其参数
	struct fb_fix_screeninfo finfo; //记录不可修改的显示器控制参数
	
/*********************************************************************************************/
/*功能：退出lcd的一些销毁操作									  		*/
/*输入：struct lcd * dev  的结构体指针，先调用init后才能调用此函数 		*/
/*输出：无  														    */
/*返回值：0 表示成功，-1 代表失败									    */
/**********************************************************************************************/	
	void(* exit)(struct lcd *dev);
/*********************************************************************************************/
/*功能：指定区域的的清屏函数，指定清屏颜色color									         */
/*输入：坐标(x0,y0)到(x_wide，y_hight)：其中必须   										 */
/*						dev->vinfo.xres >= x_wide >= x0 >=0，宽在[dev->vinfo.xres,0]区间 */
/*						dev->vinfo.yres >= y_high >= y0 >=0, 高在[dev->vinfo.yres,0]区间 */ 
/*		color：清屏区域的颜色															 */
/*		dev：struct lcd 对象指针														 */		
/*输出：无  														  					 */
/*返回值：0 表示成功，-1 代表失败									                     */
/**********************************************************************************************/	
	int (* clear)(int x0, int x_wide, int y0, int y_high, unsigned short color, struct lcd *dev);
/*********************************************************************************************/
/*功能：全屏清屏，指定清屏颜色color									        			 */
/*输入：color：清屏区域的颜色															 */
/*		dev：struct lcd 对象指针														 */		
/*输出：无  														  					 */
/*返回值：无							                    							 */
/**********************************************************************************************/	
	void(* clear_screen)(unsigned short color, struct lcd *dev);
/*********************************************************************************************/
/*功能：显示图像								        								 */
/*输入：坐标(x0,y0)到(x_wide，y_hight)显示相应分辨率的rgb16图像 						 */
/*		color：清屏区域的颜色															 */
/*		dev：struct lcd 对象指针														 */	
/*		unsigned short dst[]:图像数据首地址												 */
/*输出：无  														  					 */
/*返回值：无							                    							 */
/**********************************************************************************************/	
	int (*show_picture_16)( int x0, int x_wide, int y0, int y_hight, const void * dst, struct lcd *dev);
	void *private_data;//私有数据
};

/*************************************************************************************/
/*功能：struct camera dev 的默认初始化操作 及 调用 struct lcd 中的 init 函数，完成camera的初始化*/
/*输入：无																				*/
/*输出： struct camera * dev															*/
/*返回值：0 表示成功，-1 代表失败														*/
/*注：必须判断其返回值																	*/
/**********************************************************************************************/			
extern int lcd_info_init(struct lcd * dev);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
