//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸
#include "esp8266.h"

//Э���ļ�
#include "onenets.h"
#include "mqttkit.h"

//Ӳ������
#include "usart.h"
#include "stdelay.h"
#include "GUI.h"

//C��
#include <string.h>
#include <stdio.h>


#define PROID		"516431"

#define AUTH_INFO	"20220514V1"

#define DEVID		"943137151"


extern unsigned char esp8266_buf[128];
uint8_t numBuf[256]={0};



_Bool OneNet_DevLink(void)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���
	
	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendCmd("AT+CIPSEND\r\n",">");			//�ϴ�ƽ̨
		ESP8266_SendData_N(mqttPacket._data, mqttPacket._len);
		
		dataPtr = ESP8266_GetIPD(250);									//�ȴ�ƽ̨��Ӧ
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0://UsartPrintf(USART_DEBUG, "Tips:	���ӳɹ�\r\n");
									status = 0;break;
					case 1://UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ�Э�����\r\n");
									break;
					case 2://UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");
									break;
					case 3://UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ�������ʧ��\r\n");
									break;
					case 4://UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��û������������\r\n");
									break;
					case 5://UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");
									break;
					
					default://UsartPrintf(USART_DEBUG, "ERR:	����ʧ�ܣ�δ֪����\r\n");
									break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	}
	else
	{
		//UsartPrintf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	}
	
	return status;
	
}

void OneNet_Subscribe(const char *topics[], unsigned char topic_cnt)
{
	
	unsigned char i = 0;
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//Э���
	
	for(; i < topic_cnt; i++)
		//UsartPrintf(USART_DEBUG, "Subscribe Topic: %s\r\n", topics[i]);
	
	if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL2, topics, topic_cnt, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);					//��ƽ̨���Ͷ�������
		
		MQTT_DeleteBuffer(&mqttPacket);											//ɾ��
	}

}

void OneNet_Publish(const char *topic, const char *msg)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//Э���
	
	//UsartPrintf(USART_DEBUG, "Publish Topic: %s, Msg: %s\r\n", topic, msg);
	
	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL2, 0, 1, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);					//��ƽ̨���Ͷ�������
		
		MQTT_DeleteBuffer(&mqttPacket);											//ɾ��
	}

}

void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//Э���
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short topic_len = 0;
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	short result = 0;

	char *dataPtr = NULL;
//	char numBuf[10];
	int num = 0;
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//�����·�
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//���topic����Ϣ��
			if(result == 0)
			{
				//UsartPrintf(USART_DEBUG, "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//����ظ����
				{
					//UsartPrintf(USART_DEBUG, "Tips:	Send CmdResp\r\n");
					
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ظ�����
					MQTT_DeleteBuffer(&mqttPacket);									//ɾ��
				}
			}
		
		break;
			
		case MQTT_PKT_PUBLISH:														//���յ�Publish��Ϣ
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
			if(result == 0)
			{
				//UsartPrintf(USART_DEBUG, "topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
				//													cmdid_topic, topic_len, req_payload, req_len);
				
				switch(qos)
				{
					case 1:															//�յ�publish��qosΪ1���豸��Ҫ�ظ�Ack
					
						if(MQTT_PacketPublishAck(pkt_id, &mqttPacket) == 0)
						{
							//UsartPrintf(USART_DEBUG, "Tips:	Send PublishAck\r\n");
							ESP8266_SendData(mqttPacket._data, mqttPacket._len);
							MQTT_DeleteBuffer(&mqttPacket);
						}
					
					break;
					
					case 2:															//�յ�publish��qosΪ2���豸�Ȼظ�Rec
																					//ƽ̨�ظ�Rel���豸�ٻظ�Comp
						if(MQTT_PacketPublishRec(pkt_id, &mqttPacket) == 0)
						{
							//UsartPrintf(USART_DEBUG, "Tips:	Send PublishRec\r\n");
							ESP8266_SendData(mqttPacket._data, mqttPacket._len);
							MQTT_DeleteBuffer(&mqttPacket);
						}
					
					break;
					
					default:
						break;
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//����Publish��Ϣ��ƽ̨�ظ���Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				//UsartPrintf(USART_DEBUG, "Tips:	MQTT Publish Send OK\r\n");
			
		break;
			
		case MQTT_PKT_PUBREC:														//����Publish��Ϣ��ƽ̨�ظ���Rec���豸��ظ�Rel��Ϣ
		
			if(MQTT_UnPacketPublishRec(cmd) == 0)
			{
				//UsartPrintf(USART_DEBUG, "Tips:	Rev PublishRec\r\n");
				if(MQTT_PacketPublishRel(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					//UsartPrintf(USART_DEBUG, "Tips:	Send PublishRel\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
			
		case MQTT_PKT_PUBREL:														//�յ�Publish��Ϣ���豸�ظ�Rec��ƽ̨�ظ���Rel���豸���ٻظ�Comp
			
			if(MQTT_UnPacketPublishRel(cmd, pkt_id) == 0)
			{
				//UsartPrintf(USART_DEBUG, "Tips:	Rev PublishRel\r\n");
				if(MQTT_PacketPublishComp(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					//UsartPrintf(USART_DEBUG, "Tips:	Send PublishComp\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
		
		case MQTT_PKT_PUBCOMP:														//����Publish��Ϣ��ƽ̨����Rec���豸�ظ�Rel��ƽ̨�ٷ��ص�Comp
		
			if(MQTT_UnPacketPublishComp(cmd) == 0)
			{
				//UsartPrintf(USART_DEBUG, "Tips:	Rev PublishComp\r\n");
			}
		
		break;
			
		case MQTT_PKT_SUBACK:														//����Subscribe��Ϣ��Ack
		
			if(MQTT_UnPacketSubscribe(cmd) == 0)
			{
				//UsartPrintf(USART_DEBUG, "Tips:	MQTT Subscribe OK\r\n");
			}
			else
			{
				//UsartPrintf(USART_DEBUG, "Tips:	MQTT Subscribe Err\r\n");
			}
		
		break;
			
		case MQTT_PKT_UNSUBACK:														//����UnSubscribe��Ϣ��Ack
		
			if(MQTT_UnPacketUnSubscribe(cmd) == 0)
			{
				//UsartPrintf(USART_DEBUG, "Tips:	MQTT UnSubscribe OK\r\n");
			}
			else
			{
				//UsartPrintf(USART_DEBUG, "Tips:	MQTT UnSubscribe Err\r\n");
			}
		
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//��ջ���
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, ':');					//����'}'

	if(dataPtr != NULL && result != -1)					//����ҵ���
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		
		
		num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
		GUI_INFOPRINT("������ɣ�");
		
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}

unsigned char OneNet_FillBuf(char *buf,char *text)
{
	uint8_t i = 0;
//	uint8_t i = 0,len = 0,s[3]={0};
//	memset(text, 0, sizeof(text));
//	sprintf(text, "\"Red_Led\":%d,", led_status.Led4Sta);
//	strcat(buf, text);
	
	buf[i++]=0x7B;
	buf[i++]=0x20;
	buf[i++]=0x22;
	buf[i++]=0x64;
	buf[i++]=0x73;
	buf[i++]=0x5F;
	buf[i++]=0x69;
	buf[i++]=0x64;
	buf[i++]=0x22;
	buf[i++]=0x3A;
	buf[i++]=0x20;
	buf[i++]=0x22;
	buf[i++]=0x66;
	buf[i++]=0x69;
	buf[i++]=0x6E;
	buf[i++]=0x67;
	buf[i++]=0x65;
	buf[i++]=0x72;
	buf[i++]=0x70;
	buf[i++]=0x72;
	buf[i++]=0x69;
	buf[i++]=0x6E;
	buf[i++]=0x74;
	buf[i++]=0x5F;
	buf[i++]=0x64;
	buf[i++]=0x61;
	buf[i++]=0x74;
	buf[i++]=0x61;
	buf[i++]=0x22;
	buf[i++]=0x7D;
	
//	strcpy(buf, "{ ");
//	
//	strcat(buf, "\"ds_id\": \"fingerprint_data\"");
//	
//	strcat(buf, "}");
	
//	i = strlen(buf);
	
	buf[i++] = 0x00;
	buf[i++] = 0x00;
	buf[i++] = 0x00;
	buf[i++] = 0x01;
	
	buf[i++] = 0x30;
	
//	len = strlen(buf);
//	
//	buf[++len]=(193&0xFF000000)>>24;
//	buf[++len]=(193&0xFF0000)>>16;
//	buf[++len]=(193&0xFF00)>>8;
//	buf[++len]=(193&0xFF);
//	
//	for(i=0;i<193;i++)
//	{
//		buf[++len] = text[i+8];
//	}
//	
//	sprintf((char*)s,"%d",len);
//	
//	GUI_INFOPRINT(s);
	
	return 35;

}

void OneNet_SendData(char *data)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//Э���
	
	char buf[512];
	
	short body_len = 0, i = 0;
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf,data);																	//��ȡ��ǰ��Ҫ���͵����������ܳ���
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 2, &mqttPacket) == 0)							//���
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
//			UsartPrintf(USART_DEBUG, "Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
		}
		else
		{
			
		}
//			UsartPrintf(USART_DEBUG, "WARN:	EDP_NewBuffer Failed\r\n");
	}
	
}

void cleanNumBuf(void)
{
	uint16_t i=0;
	
	for(i=0;i<256;i++)
	{
		numBuf[i]=0;
	}
}
