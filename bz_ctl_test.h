#ifndef  _SERIAL_H_
#define  _SERIAL_H_

#ifdef __cplusplus
extern "C"{
#endif
	int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
	int open_port(int fd,int comport);
#ifdef __cplusplus
}
#endif
#endif
