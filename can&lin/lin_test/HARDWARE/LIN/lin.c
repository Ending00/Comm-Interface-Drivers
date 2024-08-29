#include "lin.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"

void LIN_Mode_Init(u32 bound)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_LINCmd(USART2,ENABLE);
	USART_LINBreakDetectLengthConfig(USART2,USART_LINBreakDetectLength_10b);
	USART_Cmd(USART2, ENABLE);	
  //Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART2, USART_IT_LBD,ENABLE);        //��LIN�Ͽ�������ж�
}	

/*����֡ͷ����*/
void Lin_SendBreak(void)
{
    USART_SendBreak(USART2);
}

/*ͬ�������*/
void Lin_SendSyncSegment(void)
{
    USART_SendData(USART2,0x55);
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET );
}

/*ͬ����*/
u8 Lin_CheckPID(u8 id)
{
    u8 returnpid ;
    u8 P0 ;
    u8 P1 ;   
    P0 = (((id)^(id>>1)^(id>>2)^(id>>4))&0x01)<<6 ;
    P1 = ((~((id>>1)^(id>>3)^(id>>4)^(id>>5)))&0x01)<<7 ;   
    returnpid = id|P0|P1 ;  
    return returnpid ;  
}

/*����PID*/
void Lin_SendHead(u8 id)
{
    Lin_SendBreak();
    Lin_SendSyncSegment();
    USART_SendData(USART2,Lin_CheckPID(id));
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET );
}


/*******************************************************
* �Ǿ���У�黹����ǿУ�飬�����ֻ֡�ܾ���У��
* �ú�������ǵ�Ƭ����Ϊ�������͵�֡ͷ������ָ��ID����֡ͷ
  ���մӻ����ص����ݣ�Ҳ���Է���֡ͷ+���ݣ��ôӻ����ա�
*******************************************************/
u8 Lin_Checksum(u8 id , u8 data[])
{
    u8 t ;
    u16 sum ;

    sum = data[0];
    if(id == 0x3c)          // ��������֡���þ���У��
    { 
        for(t=1;t<sizeof(&data)-1;t++)
        {
            sum += data[t];
            if(sum&0xff00)
            {
                sum&=0x00ff;
                sum+=1;
            }
        }
        sum = ~sum; 
        return (u8)sum ;
    }  
    for(t=1;t<sizeof(&data)-1;t++)
    {
        sum += data[t];
        if(sum&0xff00)
        {
            sum&=0x00ff;
            sum+=1;
        }
    }
    sum+=Lin_CheckPID(id);
    if(sum&0xff00)
    {
        sum&=0x00ff;
        sum+=1;
    }
    sum = ~sum;
    return (u8)sum ;
}

/*******************************************************
* ����ID+���ݣ�����У��Ͷ�
*******************************************************/
void Lin_SentData(u8 data[])
{
    u8 t ;
    for(t=0;t<sizeof(&data)-1;t++)
    {
        USART_SendData(USART2,data[t]);
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET );
    }
}

/*******************************************************
* ���ݷ��ͺ���
*******************************************************/
void Lin_SendAnswer(u8 id ,u8 data[])
{
	Lin_SendHead(id);
    Lin_SentData(data);
    USART_SendData(USART2,Lin_Checksum(id,data));
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET );
}

/*******************************************************
* ���ݽ��պ���
*******************************************************/
u8 DataReceiveflag = 0 ,FrameReceiveOverFlag = 0;
u8 ReceivePID ,ReceiveCheckSum;
u8 LinReceiveData[8] = {0X00};
void Lin_DataProcess(void)
{
	u8 ReceiveID ;
	u8 PIDChecksum ;
	u8 SumCheck ;
	
	if(DataReceiveflag == 1)
	{
		
		ReceiveID = ReceivePID&0x3f ;
		PIDChecksum = Lin_CheckPID(ReceiveID);
		if (PIDChecksum != ReceivePID)
		{
			
			return ;
		}
		else
		{}
		if(FrameReceiveOverFlag == 1)							// �ӻ���Ҫִ���ź�
		{
			SumCheck = Lin_Checksum(ReceiveID,LinReceiveData);
			if(ReceiveCheckSum != SumCheck)
			{
				return ;
			}
			else
			{}
					
			if(ReceiveID == 0x23)
			{
				if(LinReceiveData[3] == 0x01)
				{
					LED0 = 0 ;
				}
				else if(LinReceiveData[3] == 0x02)
				{
					LED0 = 1 ;
				}
				else
				{}

			}		
		}		
		FrameReceiveOverFlag = 0 ;
		DataReceiveflag = 0 ;
	}	
}

u8 DataProcess = 0 ,DtRProcess = 0;
u8 testdata[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 ReceiveData;
	u8 ReceiveID;

	 if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
	 {
		ReceiveData =USART_ReceiveData(USART2);	//��ȡ���յ�������
		if(DataProcess == 0)
		{
			if(ReceiveData != 0x55)
			{
				return ;
			}
			if(ReceiveData == 0x55)
			{
				DataProcess = 1 ;
				return ;
			}
		}
		if(DataProcess == 1)
		{
			ReceivePID = ReceiveData ;
			ReceiveID = ReceivePID&0x3f ;
			if(ReceiveID == 0x33)							// �ӻ���Ҫ�����ź�
			{
				Lin_SentData(testdata);
				USART_SendData(USART2,Lin_Checksum(ReceiveID,testdata));
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET );
				
				DataProcess = 0 ;
				return ;
			}
			DataReceiveflag = 1 ;
			DataProcess = 2 ;
			return ;
		}
		if(DataProcess == 2)
		{
			if(DtRProcess<8)
			{
				LinReceiveData[DtRProcess] = ReceiveData ;
				DtRProcess += 1 ;
				if(DtRProcess == 8)
				{
					DtRProcess = 0 ;
					DataProcess = 3 ;
					return ;
				}
			}	
		}
		if(DataProcess == 3)
		{
			ReceiveCheckSum = ReceiveData ;
			FrameReceiveOverFlag = 1 ;
			DataProcess = 0 ;
		}
     } 
} 









