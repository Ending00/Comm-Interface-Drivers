#include "can.h"

void CAN_Init(CAN_ConfigType *CANConfig)
{
    uint8_t count;
    CAN_FilterType* CANFilterList = 0;

//    CRG->CAN_CTRL &= ~((uint32_t)3<<30);        //CAN工作时钟选择：HSI 
    CRG->CAN_CTRL |= (uint32_t)3<<30;           //CAN工作时钟选择：AHB   
//    CRG->CAN_CTRL |= (uint32_t)2<<30;           //CAN工作时钟选择：APB   
//    CRG->CAN_CTRL |= (uint32_t)1<<30;           //CAN工作时钟选择：HSE 
    
    CRG->CKEN_CTRL1 |= 1<<4;                    //GPIOB时钟使能
    CRG->CAN_CTRL |= 1<<0;                      //CAN时钟使能
    CRG->CAN_CTRL &= ~(1<<1);                   //CAN软复位失能
   

    IOCFG->IOCFG_CTRLB |= (Bit0_En|Bit3_En);    //PB0 -> CAN_TX   PB1 -> CAN_RX

    CAN->CAN_CR0 |= 1<<7;                  // RESET BIT SET 1    
    
    switch(CANConfig->CAN_WorkMode)
    {
    case CAN_20B:
        CRG->CAN_CTRL &= ~(1<<2);           //CAN2.0
        break;

    case CAN_FD_ISO:
        CRG->CAN_CTRL |= 1<<2;              //CANFD
        CAN->CAN_CR0 |= 1<<23;              //CANFD ISO 11898-1:2015
        break;

    case CAN_FD_NONISO:
        CRG->CAN_CTRL |= 1<<2;              //CANFD
        CAN->CAN_CR0 &= ~(1<<23);           //Bosch CANFD (non-ISO)
        break;

    default:
        break;
    }

    /*
    	tSeg1 = (S_SEG_1 + 2); tSeg2 = (S_SEG_2 + 1).
        BandRate 	= (36M / (S_PRESC + 1) / ((S_SEG_1 + 2) + (S_SEG_2 + 1)))
        SamplePoint = (tSeg1 / (tSeg1 + tSeg2)).
    */
    // 1M bps    samlepoint:75%
    CAN->CAN_SBITRATE &= ~(0xFF7F7FFF);

    CAN->CAN_SBITRATE |= 0x05<<24;              //S_PRESC = 5+1   12M
    CAN->CAN_SBITRATE |= 7<<0;                  //S_SEG1 = 0x07
    CAN->CAN_SBITRATE |= 2<<8;                  //S_SEG2 = 0x02
    CAN->CAN_SBITRATE |= 0x01<<16;              //S_SJW = 1

    if(CANConfig->CANFilterList)
    {
        CANFilterList = (CAN_FilterType *)CANConfig->CANFilterList;
        for(count = 0; count < CANConfig->Filter_Number; count++)
        {
            if(CANFilterList[count].Filter_Status)
            {
                CAN->CAN_ACFCTRL |= ((uint32_t)1 << (CANFilterList[count].Filter_ChannelNum + 16));     //Enable Filter

                CAN->CAN_ACFCTRL |= (CANFilterList[count].Filter_ChannelNum & 0x0F);                     //Filter address

                //Set ACODE
                CAN->CAN_ACFCTRL &= ~(1<<5);        //SELMASK = 0;
                CAN->CAN_ACF = (CANFilterList[count].Acode & 0x1FFFFFFF);
                //Set AMASK

                CAN->CAN_ACFCTRL |= 1<<5;                //SELMASK = 1;
                CAN->CAN_ACF = (CANFilterList[count].Amask & 0x1FFFFFFF);
                if(CANFilterList[count].AIDE)
                    CAN->CAN_ACF |= 1<<28;
                if(CANFilterList[count].AIDEE)
                    CAN->CAN_ACF |= 1<<29;
            }
            else
            {
                CAN->CAN_ACFCTRL &= ~(1 << (CANFilterList[count].Filter_ChannelNum + 16));
            }
        }
    }
    CAN->CAN_CR0 &= ~(0x01<<7);             //Reset Bit clear 0
    CAN->CAN_CR0 |= (0x03<<3);              //tpss = 1 tsss = 1
    CAN->CAN_CR0 &= ~(0x03<<5);             // Loop Back Mode(Internal) and Loop Back Mode(External) are Disable
    CAN->CAN_CR0 &= ~(0x01<<14);            //Listen Only Mode Disable
    CAN->CAN_CR1 = (0X06 << 28);            //AFWL = 0X06
    CAN->CAN_CR1 &= ~(0x0F<<24);            //EWL = 0

    CAN->CAN_CR1 |= 0xC2;                   //RIE,ROIE,EIE set 1

    CAN->CAN_CR1 |= ((uint32_t)0x2A<<16);   //BEIE,ALIE,EPIE enable

}

void CAN_Transmit(CAN_TxMsg *TxMessage, uint8_t CAN_WorkMode)
{
    uint32_t tempDATA = 0;
    uint8_t count, data_length;
    while((CAN->CAN_CR0 & Bit8_En) == Bit8_En);
    CAN->CAN_CR0 |= Bit15_En;         //Select Transmit Buffer: STB

    CAN->TBUF.ID_TTSEN = TxMessage->ID & 0x1FFFFFFFUL;
    if(TxMessage->TTSEN)
        CAN->TBUF.ID_TTSEN |= Bit31_En;

    CAN->TBUF.TXCTRL = (uint32)(TxMessage->DLC & 0x0FUL);

    if(CAN_WorkMode == CAN_20B)
    {      
        if(TxMessage->RTR)
            CAN->TBUF.TXCTRL |= Bit6_En;
    }
    if((CAN_WorkMode == CAN_FD_ISO) || (CAN_WorkMode == CAN_FD_NONISO))
    {
        if(TxMessage->BRS)
            CAN->TBUF.TXCTRL |= Bit4_En;          
        if(TxMessage->FDF)
            CAN->TBUF.TXCTRL |= Bit5_En;  
    }
    
    if(TxMessage->IDE)
        CAN->TBUF.TXCTRL |= Bit7_En;    
    
    if((TxMessage->DLC)<=8)
        data_length = TxMessage->DLC;
    if(TxMessage->DLC == CAN_DLC_12BYTES)
        data_length = 12;
    if(TxMessage->DLC == CAN_DLC_16BYTES)
        data_length = 16;
    if(TxMessage->DLC == CAN_DLC_20BYTES)
        data_length = 20;
    if(TxMessage->DLC == CAN_DLC_24BYTES)
        data_length = 24;
    if(TxMessage->DLC == CAN_DLC_32BYTES)
        data_length = 32;
    if(TxMessage->DLC == CAN_DLC_48BYTES)
        data_length = 48;
    if(TxMessage->DLC == CAN_DLC_64BYTES)
        data_length = 64;
    if(data_length % 4)
    {
        for(count = 0; count < data_length; count += 4)
        {
            tempDATA = TxMessage->Data[count] | (TxMessage->Data[count+1] << 8) |
                       (TxMessage->Data[count+2] << 16) | (TxMessage->Data[count+3] << 24);
            CAN->TBUF.DATA[count>>2] = tempDATA;
        }
    }
    else
    {
        for(count = 0; count < data_length; count += 4)
        {
            CAN->TBUF.DATA[count>>2] = *(uint32_t *)(&TxMessage->Data[count]);
        }
    }
    CAN->CAN_CR0 |= Bit22_En;       //STB slot filled, select next FIFO slot.
    CAN->CAN_CR0 |= Bit9_En;        //Send one message only
}


void CAN_Receive(CAN_RxMsg *RxMessage, uint8_t CAN_WorkMode)     //只给标准格式用
{
    if(CAN_WorkMode == CAN_20B)
    {
        RxMessage->RTR = (uint8_t)((CAN->RBUF.RXCTRL >> 6)&0x01);
    }
    
    if((CAN_WorkMode == CAN_FD_ISO) || (CAN_WorkMode == CAN_FD_NONISO))
    {
        RxMessage->ESI = (uint8_t)(CAN->RBUF.ID_ESI >> 31);         //only in CAN FD
        RxMessage->BRS = (uint8_t)((CAN->RBUF.RXCTRL >> 4)&0x01);   //only in CAN FD
        RxMessage->FDF = (uint8_t)((CAN->RBUF.RXCTRL >> 5)&0x01);   //only in CAN FD    
    }
    
    RxMessage->ID = (uint32_t)(CAN->RBUF.ID_ESI & 0x1FFFFFFFUL);
    RxMessage->DLC = (uint8_t)(CAN->RBUF.RXCTRL & 0X0F);
    RxMessage->IDE = (uint8_t)((CAN->RBUF.RXCTRL >> 7)&0x01);
    RxMessage->Data[0] = CAN->RBUF.DATA[0];
    RxMessage->Data[1] = CAN->RBUF.DATA[1];
}




