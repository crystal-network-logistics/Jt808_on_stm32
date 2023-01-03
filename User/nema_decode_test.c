/**
  ******************************************************************************
  * @file    nmea_decode_test.c
  * @author  WJSHM
  * @version V1.0
  * @date    2016-07-xx
  * @brief   ����NEMA�����
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 
  
#include "stm32f10x.h"
#include "./usart/usart.h"
#include "./gps/gps_config.h"
//#include "ff.h"
#include "nmea/nmea.h"
#include <stdio.h>
#include <stdlib.h>

char bufTime[12]={0};
int nmea_decode_test(void)
{
		double deg_lat;//ת����[degree].[degree]��ʽ��γ��
		double deg_lon;//ת����[degree].[degree]��ʽ�ľ���
	  long m_lat=0;
		long m_lon;
		int m_alt;
		int m_speed;
		int m_bearing;
	
    nmeaINFO info;          //GPS�����õ�����Ϣ
    nmeaPARSER parser;      //����ʱʹ�õ����ݽṹ  
    uint8_t new_parse=0;    //�Ƿ����µĽ������ݱ�־
  
    nmeaTIME beiJingTime;    //����ʱ�� 

    /* �����������������Ϣ�ĺ��� */
    nmea_property()->trace_func = &trace;
    nmea_property()->error_func = &error;
    nmea_property()->info_func = &gps_info;

    /* ��ʼ��GPS���ݽṹ */
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
	  //char bufTime[64];
	

    while(1)
    {
      if(GPS_HalfTransferEnd)     /* ���յ�GPS_RBUFF_SIZEһ������� */
      {
        /* ����nmea��ʽ���� */
        nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);
        
        GPS_HalfTransferEnd = 0;   //��ձ�־λ
        new_parse = 1;             //���ý�����Ϣ��־ 
      }
      else if(GPS_TransferEnd)    /* ���յ���һ������ */
      {

        nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
       
        GPS_TransferEnd = 0;
        new_parse =1;
      }
      
      if(new_parse )                //���µĽ�����Ϣ   
      {    
        /* �Խ�����ʱ�����ת����ת���ɱ���ʱ�� */
        GMTconvert(&info.utc,&beiJingTime,8,1);
        
        /* �������õ�����Ϣ */
				printf("\r\nʱ��%02d-%02d-%02d  %02d:%02d:%02d\r\n", ((beiJingTime.year+1900)%2000), beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
				//printf("\r\nʱ��%d-%02d-%02d,%d:%d:%d\r\n", beiJingTime.year+1900, beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
				
				//info.lat lon�еĸ�ʽΪ[degree][min].[sec/60]��ʹ�����º���ת����[degree].[degree]��ʽ
				deg_lat = nmea_ndeg2degree(info.lat);
				deg_lon = nmea_ndeg2degree(info.lon);
				
				printf("\r\nγ�ȣ�%f,����%f\r\n",deg_lat,deg_lon);
        printf("\r\n���θ߶ȣ�%f �� ", info.elv);
        printf("\r\n�ٶȣ�%f km/h ", info.speed);
        printf("\r\n����%f ��", info.direction);
				
				printf("\r\n����ʹ�õ�GPS���ǣ�%d,�ɼ�GPS���ǣ�%d",info.satinfo.inuse,info.satinfo.inview);

				printf("\r\n����ʹ�õı������ǣ�%d,�ɼ��������ǣ�%d",info.BDsatinfo.inuse,info.BDsatinfo.inview);
				printf("\r\nPDOP��%f,HDOP��%f��VDOP��%f\n\n",info.PDOP,info.HDOP,info.VDOP);
				
				m_lat=(long)(deg_lat*(1e6));
				m_lon=(long)(deg_lon*1000000);
				m_alt=(int)(info.elv);
				m_speed=(int)(info.speed*10);
				m_bearing=(int)(info.direction);
				
				if(m_lat!=0)
				{
					printf("γ�ȣ�%ld\n",m_lat);
					printf("����: %ld\n",m_lon);
					printf("����: %d �� \n", m_alt);
					printf("�ٶȣ�%d km/h \n", m_speed);
					printf("����%d ��\n", m_bearing);
					printf("ʱ��: %02d-%02d-%02d  %02d:%02d:%02d\n", ((beiJingTime.year+1900)%2000), beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
					
					//char *bufTime=(char *)malloc(64);
					sprintf(bufTime,"%02d%02d%02d%02d%02d%02d",((beiJingTime.year+1900)%2000), beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
					printf("bufTime: %s\n",bufTime);
					//free(bufTime);
				}

        new_parse = 0;
				memset(bufTime,0,12);
				break;
      }
	
	}

    /* �ͷ�GPS���ݽṹ */
     nmea_parser_destroy(&parser);

    
		return 0;
}







/**************************************************end of file****************************************/

