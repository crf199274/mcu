#ifndef _ONENET_H_
#define _ONENET_H_

extern uint8_t numBuf[256];


_Bool OneNet_DevLink(void);

void OneNet_Subscribe(const char *topics[], unsigned char topic_cnt);

void OneNet_Publish(const char *topic, const char *msg);

void OneNet_RevPro(unsigned char *cmd);

void cleanNumBuf(void);

unsigned char OneNet_FillBuf(char *buf,char *text);

void OneNet_SendData(char *data);

void OneNet_SendFingerprint_data(char *data);

uint8_t ONTNET_PING(void);

#endif
