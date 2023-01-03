#include "./internal_flash/bsp_internal_flash.h"   
#include "./usart/usart.h"

//int InternalFlash_Test(uint8_t Data)
void FLASH_WriteData(uint8_t *Data)
{
	uint32_t EraseCounter = 0x00; 	//��¼Ҫ��������ҳ
	uint32_t Address = 0x00;				//��¼д��ĵ�ַ
	//uint32_t Data = 0x3210ABCD;			//��¼д�������
	uint32_t NbrOfPage = 0x00;			//��¼д�����ҳ
	int writeCounter = 0;
	
	FLASH_Status FLASHStatus = FLASH_COMPLETE; //��¼ÿ�β����Ľ��	
	TestStatus MemoryProgramStatus = PASSED;//��¼�������Խ��
	

  // ����
  FLASH_Unlock();

  // ����Ҫ��������
  NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;

  // ������б�־λ 
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

  // ��ҳ��
  for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  {
    FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
	}
  
  // ���ڲ�FLASHд������ 
  Address = WRITE_START_ADDR;

  while((Address < WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
  {
    FLASH_ProgramOptionByteData(Address, Data[writeCounter]);
		//FLASH_ProgramHalfWord(Address,Data[dataIndex]);
    Address = Address + 1;
		writeCounter++;
  }

  FLASH_Lock();
}


uint8_t FLASH_ReadByte(uint32_t address)
{
  return *(__IO uint16_t*)address; 
}

void FLASH_Read(u8 *pBuffer)
{
	int i;
	uint32_t address = WRITE_START_ADDR;
	//int NumToRead = WRITE_END_ADDR;
	

	for(i=0; i<50;i++)
	{
		pBuffer[i]=FLASH_ReadByte(address);
		address=address+1;
	}
}

//uint16_t FLASH_ReadHalfWord(uint32_t address)
//{
//   return *(__IO uint16_t*)address; 
//}


  // ���д��������Ƿ���ȷ 
 // Address = WRITE_START_ADDR;
	//printf("%x \r\n", Data);
	//printf("%x \r\n",(*(__IO uint32_t*) Address));
 // while((Address < WRITE_END_ADDR) && (MemoryProgramStatus != FAILED))
  //{
    //if((*(__IO uint8_t*) Address) != Data[])
//{
//MemoryProgramStatus = FAILED;
//}
//Address += 1;
  //}
	//return MemoryProgramStatus;
//}




