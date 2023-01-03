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
#include "ff.h"
#include "nmea/nmea.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char bufTime[12]={0};
int nmea_decode_test(double *v_latitude, double *v_longitude, float *v_altitude, float  *v_speed, float *v_bearing, unsigned char *v_timestamp)
{
		double deg_lat;//ת����[degree].[degree]��ʽ��γ��
		double deg_lon;//ת����[degree].[degree]��ʽ�ľ���
	  long m_lat=0;
		long m_lon=0;
		int m_alt=0;
		int m_speed=0;
		int m_bearing=0;
	
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
        		
				//info.lat lon�еĸ�ʽΪ[degree][min].[sec/60]��ʹ�����º���ת����[degree].[degree]��ʽ
				deg_lat = nmea_ndeg2degree(info.lat);
				deg_lon = nmea_ndeg2degree(info.lon);

				m_lat=(long)(deg_lat*(1e6));
				m_lon=(long)(deg_lon*1000000);
				m_alt=(int)(info.elv);
				m_speed=(int)(info.speed*10);
				m_bearing=(int)(info.direction);
				
				if(m_lat!=0)
				{
					*v_latitude = deg_lat;
					*v_longitude = deg_lon;
					*v_altitude = info.elv;
					*v_speed = info.speed;
					*v_bearing = info.direction;

					return 1;
				}

        new_parse = 0;
				memset(bufTime,0,12);
				
				break;
      }
		}
//		nmea_parser_destroy(&parser);
		return 0;
}







/**************************************************end of file****************************************/

