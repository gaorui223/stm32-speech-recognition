/*	������STM32F103VE	
	ȫ��falsh 512KB 256ҳ ÿҳ2KB
	
	ÿ����������ģ��ռ��4KB ��������ģ�� ÿ������ָ��4������ģ��
	��������趨20������ָ�� ��ռ��320KB
	
	flash���320KB���ڴ洢��������ģ��
	������������ ����洢��������ռ��
	��д����ʱҲ���ܲ����洢�� ѡ������Ҫ��ҳ
*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "Flash.H"
#include "USART.h"
#include "MFCC.H"

u8 save_ftr_mdl(v_ftr_tag* ftr, u32 addr)
{
	u32 i;
	u32 ftr_size;
	
	if(((addr%FLASH_PAGE_SIZE)!=0)||(addr<ftr_start_addr)||(addr>(ftr_end_addr-size_per_ftr)))
	{
		USART1_printf("flash addr error");
		return Flash_Fail;
	}
	ftr_size=2*mfcc_num*ftr->frm_num;
	
	FLASH_UnlockBank1();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
	for(i=0;i<page_per_ftr;i++)
	{
		if(FLASH_ErasePage(addr+FLASH_PAGE_SIZE*i)!=FLASH_COMPLETE)
		{
			USART1_printf("flash Erase Error! ");
			return Flash_Fail;
		}
	}
	
	//����������Ч���
	if(FLASH_ProgramHalfWord(addr, save_mask)!=FLASH_COMPLETE)
	{
		USART1_printf("flash Program Error! ");
		return Flash_Fail;
	}
	addr+=2;
	//��������ģ��֡����
	if(FLASH_ProgramHalfWord(addr,*(u16 *)(&(ftr->frm_num)))!=FLASH_COMPLETE)
	{
		USART1_printf("flash Program Error! ");
		return Flash_Fail;
	}
	addr+=2;
	//��������ģ��MFCC����
	for(i=0;i<ftr_size;i+=2)
	{
		if(FLASH_ProgramHalfWord(addr+i,*(u16 *)((u32)(ftr->mfcc_dat)+i))!=FLASH_COMPLETE)
		{
			USART1_printf("flash Program Error! ");
			return Flash_Fail;
		}
	}
	
	FLASH_LockBank1();
	return Flash_Success;
}