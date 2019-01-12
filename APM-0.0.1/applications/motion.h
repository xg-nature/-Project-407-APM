#ifndef __motion_h
#define __motion_h


//void time_pid_init();
int mycontrol_init2();
#define  u16  unsigned short
#define u8 unsigned char
struct rt_data{
	u16 motor_data;
	u16 encode2_data;
	u16 mytick;
};
#endif
