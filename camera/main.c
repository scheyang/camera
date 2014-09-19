#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "lcd.h"
#include "v4l.h"
#include "lcd.h"
void init_main(){
	int ret;
	struct camera  camera;
	memset(&camera,0,sizeof(camera));
	strcpy(camera.dev_name,"/dev/video100");
//	camera.v4l_info.wide=480;
//	camera.v4l_info.high=320;
	
	ret = camera_info_init(&camera);
	if(ret == -1){
		printf("camera_info_init failed\n");
		exit(-1);
	}
	

	struct lcd lcd;
	memset(&lcd,0,sizeof(lcd));
	if(-1==lcd_info_init(&lcd)){
		printf("lcd nit failed\n");
		exit(-1);
	}
	unsigned char *buf = NULL;
	while(1){
		camera.get_rgb16(&camera,&buf);
		lcd.show_picture_16(240,camera.v4l_info.wide,120,camera.v4l_info.high,buf,&lcd);
	}
}

int main( void ){
	
	init_main();


//	lcd_exit();
//	camera_stop();
//	camera_exit();
	return 0;
}


