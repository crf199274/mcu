#ifndef __Service_H
#define __Service_H

extern uint8_t msgBuf[256];

_Bool Service_DevLink(void);

void Service_RevPro(unsigned char *cmd);

uint8_t Service_PING(void);

void Service_Publish(const char *topic, const char *msg);

void Service_Subscribe(const char *topics[], unsigned char topic_cnt);

void cleanMsgBuf(void);

#endif
