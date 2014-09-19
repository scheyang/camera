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

/*lcd ����ṹ��*/
struct lcd {
	int fd;							//lcd �ļ�������
	char dev_name[20];				//Ĭ��/dev/fb0
	int screensize;					//��Ļ�ߴ磬��ʼ�����Զ����
	char *fbp;						//��Ļӳ����ַ����ʼ������
	struct lcd *pdev;
	struct fb_var_screeninfo vinfo; //��¼���޸ĵ���ʾ���������
	struct fb_fix_screeninfo finfo; //��¼�����޸ĵ���ʾ�����Ʋ���
	
/*********************************************************************************************/
/*���ܣ��˳�lcd��һЩ���ٲ���									  		*/
/*���룺struct lcd * dev  �Ľṹ��ָ�룬�ȵ���init����ܵ��ô˺��� 		*/
/*�������  														    */
/*����ֵ��0 ��ʾ�ɹ���-1 ����ʧ��									    */
/**********************************************************************************************/	
	void(* exit)(struct lcd *dev);
/*********************************************************************************************/
/*���ܣ�ָ������ĵ�����������ָ��������ɫcolor									         */
/*���룺����(x0,y0)��(x_wide��y_hight)�����б���   										 */
/*						dev->vinfo.xres >= x_wide >= x0 >=0������[dev->vinfo.xres,0]���� */
/*						dev->vinfo.yres >= y_high >= y0 >=0, ����[dev->vinfo.yres,0]���� */ 
/*		color�������������ɫ															 */
/*		dev��struct lcd ����ָ��														 */		
/*�������  														  					 */
/*����ֵ��0 ��ʾ�ɹ���-1 ����ʧ��									                     */
/**********************************************************************************************/	
	int (* clear)(int x0, int x_wide, int y0, int y_high, unsigned short color, struct lcd *dev);
/*********************************************************************************************/
/*���ܣ�ȫ��������ָ��������ɫcolor									        			 */
/*���룺color�������������ɫ															 */
/*		dev��struct lcd ����ָ��														 */		
/*�������  														  					 */
/*����ֵ����							                    							 */
/**********************************************************************************************/	
	void(* clear_screen)(unsigned short color, struct lcd *dev);
/*********************************************************************************************/
/*���ܣ���ʾͼ��								        								 */
/*���룺����(x0,y0)��(x_wide��y_hight)��ʾ��Ӧ�ֱ��ʵ�rgb16ͼ�� 						 */
/*		color�������������ɫ															 */
/*		dev��struct lcd ����ָ��														 */	
/*		unsigned short dst[]:ͼ�������׵�ַ												 */
/*�������  														  					 */
/*����ֵ����							                    							 */
/**********************************************************************************************/	
	int (*show_picture_16)( int x0, int x_wide, int y0, int y_hight, const void * dst, struct lcd *dev);
	void *private_data;//˽������
};

/*************************************************************************************/
/*���ܣ�struct camera dev ��Ĭ�ϳ�ʼ������ �� ���� struct lcd �е� init ���������camera�ĳ�ʼ��*/
/*���룺��																				*/
/*����� struct camera * dev															*/
/*����ֵ��0 ��ʾ�ɹ���-1 ����ʧ��														*/
/*ע�������ж��䷵��ֵ																	*/
/**********************************************************************************************/			
extern int lcd_info_init(struct lcd * dev);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
