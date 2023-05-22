#include "stm32f10x.h"
#include "service.h"
#include "esp8266.h"
#include "mqttkit.h"
#include "usart.h"
#include "stdelay.h"
#include "GUI.h"
#include "userLed.h"
#include "AllTaskInit.h"
#include <string.h>
#include <stdio.h>

#define PROID		"12345"

#define AUTH_INFO	"159753"

#define DEVID		"mqtt_box3"

extern unsigned char esp8266_buf[128];
uint8_t msgBuf[256]={0};

/*************************************************
函数名: Service_RevPro()
功能: 解包
入口参数: unsigned char *cmd 数据指针
返回值: 无
*************************************************/
void Service_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short topic_len = 0;
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	short result = 0;

	char *dataPtr1 = NULL;
	char *dataPtr2 = NULL;
	int i = 0;
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//命令下发
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//解出topic和消息体
			if(result == 0)
			{
				//UsartPrintf(USART_DEBUG, "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//命令回复组包
				{
					//UsartPrintf(USART_DEBUG, "Tips:	Send CmdResp\r\n");
					
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//回复命令
					MQTT_DeleteBuffer(&mqttPacket);									//删包
				}
			}
		
		break;
			
		case MQTT_PKT_PUBLISH:														//接收的Publish消息
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
			if(result == 0)
			{
				//UsartPrintf(USART_DEBUG, "topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
				//													cmdid_topic, topic_len, req_payload, req_len);
				
				switch(qos)
				{
					case 1:															//收到publish的qos为1，设备需要回复Ack
					
						if(MQTT_PacketPublishAck(pkt_id, &mqttPacket) == 0)
						{
							//UsartPrintf(USART_DEBUG, "Tips:	Send PublishAck\r\n");
							ESP8266_SendData(mqttPacket._data, mqttPacket._len);
							MQTT_DeleteBuffer(&mqttPacket);
						}
					
					break;
					
					case 2:															//收到publish的qos为2，设备先回复Rec
																					//平台回复Rel，设备再回复Comp
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
			
		case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				//UsartPrintf(USART_DEBUG, "Tips:	MQTT Publish Send OK\r\n");
			
		break;
			
		case MQTT_PKT_PUBREC:														//发送Publish消息，平台回复的Rec，设备需回复Rel消息
		
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
			
		case MQTT_PKT_PUBREL:														//收到Publish消息，设备回复Rec后，平台回复的Rel，设备需再回复Comp
			
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
		
		case MQTT_PKT_PUBCOMP:														//发送Publish消息，平台返回Rec，设备回复Rel，平台再返回的Comp
		
			if(MQTT_UnPacketPublishComp(cmd) == 0)
			{
				//UsartPrintf(USART_DEBUG, "Tips:	Rev PublishComp\r\n");
			}
		
		break;
			
		case MQTT_PKT_SUBACK:														//发送Subscribe消息的Ack
		
			if(MQTT_UnPacketSubscribe(cmd) == 0)
			{
				//UsartPrintf(USART_DEBUG, "Tips:	MQTT Subscribe OK\r\n");
			}
			else
			{
				//UsartPrintf(USART_DEBUG, "Tips:	MQTT Subscribe Err\r\n");
			}
		
		break;
			
		case MQTT_PKT_UNSUBACK:														//发送UnSubscribe消息的Ack
		
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
	
	ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;
	
	dataPtr1 = strchr(req_payload, '{');					//搜索'}'
	dataPtr2 = strchr(req_payload, '}');					//搜索'}'

	if(dataPtr1 != NULL && result != -1)					//如果找到了
	{
		dataPtr1++;
		
		while(!(*dataPtr1==*dataPtr2))
		{
				msgBuf[i++] = *dataPtr1++;
		}
		
//		GUI_INFOPRINT("接收完成！");
		LedStatus.MSG = 1;
		
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}

/*************************************************
函数名: Service_DevLink()
功能: 连接服务器
入口参数: 无
返回值: 无
*************************************************/
_Bool Service_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//上传平台
		
		dataPtr = ESP8266_GetIPD(250);									//等待平台响应
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0://GUI_INFOPRINT("连接成功");//UsartPrintf(USART_DEBUG, "Tips:	连接成功\r\n");
									status = 0;break;
					case 1://GUI_INFOPRINT("协议错误");//UsartPrintf(USART_DEBUG, "WARN:	连接失败：协议错误\r\n");
									break;
					case 2://GUI_INFOPRINT("非法的clientid");//UsartPrintf(USART_DEBUG, "WARN:	连接失败：非法的clientid\r\n");
									break;
					case 3://GUI_INFOPRINT("服务器失败");//UsartPrintf(USART_DEBUG, "WARN:	连接失败：服务器失败\r\n");
									break;
					case 4://GUI_INFOPRINT("用户名或密码错误");//UsartPrintf(USART_DEBUG, "WARN:	连接失败：用户名或密码错误\r\n");
									break;
					case 5://GUI_INFOPRINT("非法链接");//UsartPrintf(USART_DEBUG, "WARN:	连接失败：非法链接(比如token非法)\r\n");
									break;
					
					default://GUI_INFOPRINT("未知错误");//UsartPrintf(USART_DEBUG, "ERR:	连接失败：未知错误\r\n");
									break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//删包
	}
	else
	{
		//GUI_INFOPRINT("Failed");
		//UsartPrintf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	}
	
	return status;
	
}

/*************************************************
函数名: Service_Publish()
功能: 发布消息
入口参数: const char *topic 订阅的主题 unsigned char topic_cnt 订阅的个数
返回值: 无
*************************************************/
void Service_Subscribe(const char *topics[], unsigned char topic_cnt)
{
	
//	unsigned char i = 0;
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//协议包
	
//	for(; i < topic_cnt; i++)
//		UsartPrintf(USART_DEBUG, "Subscribe Topic: %s\r\n", topics[i]);
	
	if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL2, topics, topic_cnt, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);					//向平台发送订阅请求
		
		MQTT_DeleteBuffer(&mqttPacket);											//删包
	}

}

/*************************************************
函数名: Service_Publish()
功能: 发布消息
入口参数: const char *topic 发布的主题 const char *msg 发布消息
返回值: 无
*************************************************/
void Service_Publish(const char *topic, const char *msg)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//协议包
	
	//UsartPrintf(USART_DEBUG, "Publish Topic: %s, Msg: %s\r\n", topic, msg);
	
	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL2, 0, 1, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);					//向平台发送订阅请求
		
		MQTT_DeleteBuffer(&mqttPacket);											//删包
	}

}

/*************************************************
函数名: Service_PING()
功能: 心跳包
入口参数: 无
返回值: 无
*************************************************/
uint8_t Service_PING(void)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//协议包
	
	uint8_t *dataPtr = NULL;
	
	if(MQTT_PacketPing(&mqttPacket) == 0)
	{
		 
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
		
		MQTT_DeleteBuffer(&mqttPacket);															//删包
		
		dataPtr = ESP8266_GetIPD(100);
		
		if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_PINGRESP)
		{
			
			ESP8266_Clear();
			
			return 0;
			
		}else{
			
			ESP8266_Clear();
			
			return 1;
			
		}
		
	}
	
	return 2;
	
}

/*************************************************
函数名: cleanMsgBuf()
功能: 删包
入口参数: 无
返回值: 无
*************************************************/
void cleanMsgBuf(void)
{
	memset(msgBuf,0,sizeof(msgBuf));
}