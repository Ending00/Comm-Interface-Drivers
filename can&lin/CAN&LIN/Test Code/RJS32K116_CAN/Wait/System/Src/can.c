#include "can.h"

void CAN_Init(CAN_InitTypeDef *CANInitStructure)
{
    uint8_t count;
    CAN_FilterTypeDef *CANFilterInit = 0;

    CAN->CAN_CR0 |= 1<<7;                  //CAN复位使能
    
    /*
    	tSeg1 = (S_SEG_1 + 2); tSeg2 = (S_SEG_2 + 1).
        BandRate 	= (36M / (S_PRESC + 1) / ((S_SEG_1 + 2) + (S_SEG_2 + 1)))
        SamplePoint = (tSeg1 / (tSeg1 + tSeg2)).
    */
    // 1M bps    samlepoint:75%    
    switch(CANInitStructure->CAN_WorkMode)
    {
    case CAN_20B:
        CRG->CAN_CTRL &= ~(1<<2);           //CAN2.0

        CAN->CAN_SBITRATE &= ~(0xFF7F7FFF);
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_Presc)<<24;
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_Seg1)<<0;                 
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_Seg2 & 0x7F)<<8;
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_SJW & 0x7F)<<16;             
        break;

    case CAN_FD_ISO:
        CRG->CAN_CTRL |= 1<<2;              //CANFD
        CAN->CAN_CR0 |= 1<<23;              //CANFD ISO 11898-1:2015
    
        CAN->CAN_SBITRATE &= ~(0xFF7F7FFF);
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_Presc)<<24;
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_Seg1)<<0;                 
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_Seg2 & 0x7F)<<8;
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_SJW & 0x7F)<<16;               

        CAN->CAN_FBITRATE &= ~(0xFF0F0F1F);
        CAN->CAN_FBITRATE |= (CANInitStructure->FBitRateInitStructure.F_Presc)<<24;
        CAN->CAN_FBITRATE |= (CANInitStructure->FBitRateInitStructure.F_Seg1 & 0x1F)<<0;
        CAN->CAN_FBITRATE |= (CANInitStructure->FBitRateInitStructure.F_Seg2 & 0x0F)<<8;
        CAN->CAN_FBITRATE |= (CANInitStructure->FBitRateInitStructure.F_SJW & 0x0F)<<16;                
        break;

    case CAN_FD_NONISO:
        CRG->CAN_CTRL |= 1<<2;              //CANFD
        CAN->CAN_CR0 &= ~(1<<23);           //Bosch CANFD (non-ISO)
    
        CAN->CAN_SBITRATE &= ~(0xFF7F7FFF);
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_Presc)<<24;
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_Seg1)<<0;                 
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_Seg2 & 0x7F)<<8;
        CAN->CAN_SBITRATE |= (CANInitStructure->SBitRateInitStructure.S_SJW & 0x7F)<<16;          

        CAN->CAN_FBITRATE &= ~(0xFF0F0F1F);
        CAN->CAN_FBITRATE |= (CANInitStructure->FBitRateInitStructure.F_Presc)<<24;
        CAN->CAN_FBITRATE |= (CANInitStructure->FBitRateInitStructure.F_Seg1 & 0x1F)<<0;
        CAN->CAN_FBITRATE |= (CANInitStructure->FBitRateInitStructure.F_Seg2 & 0x0F)<<8;
        CAN->CAN_FBITRATE |= (CANInitStructure->FBitRateInitStructure.F_SJW & 0x0F)<<16;    
        break;

    default:
        break;
    }

    if(CANInitStructure->FilterInitStructure)
    {
        CANFilterInit = (CAN_FilterTypeDef *)CANInitStructure->FilterInitStructure;
        for(count = 0; count < CANInitStructure->Num_Filter; count++)
        {
            if(CANFilterInit[count].Filter_Status)
            {
                CAN->CAN_ACFCTRL |= ((uint32_t)1 << (CANFilterInit[count].Filter_ChannelNum + 16));     //Enable Filter

                CAN->CAN_ACFCTRL |= (CANFilterInit[count].Filter_ChannelNum & 0x0F);                     //Filter address

                //Set ACODE
                CAN->CAN_ACFCTRL &= ~(1<<5);        //SELMASK = 0;
                CAN->CAN_ACF = (CANFilterInit[count].Acode & 0x1FFFFFFF);
                //Set AMASK

                CAN->CAN_ACFCTRL |= 1<<5;                //SELMASK = 1;
                CAN->CAN_ACF = (CANFilterInit[count].Amask & 0x1FFFFFFF);
                if(CANFilterInit[count].AIDE)
                    CAN->CAN_ACF |= 1<<29;                
                if(CANFilterInit[count].AIDEE)
                    CAN->CAN_ACF |= 1<<30;                
                CAN->CAN_ACFCTRL &= ~(1<<5);        //SELMASK = 0;
            }
            else
            {
                CAN->CAN_ACFCTRL &= ~((uint32_t)1 << (CANFilterInit[count].Filter_ChannelNum + 16));     //Enable Filter
            }
        }
    }
    CAN->CAN_CR0 &= ~(0x01<<7);             //CAN复位失能
    
    CAN->CAN_CR0 |= (0x03<<3);              //tpss = 1 tsss = 1
    CAN->CAN_CR0 &= ~(0x03<<5);             // Loop Back Mode(Internal) and Loop Back Mode(External) are Disable
    CAN->CAN_CR0 &= ~(0x01<<14);            //Listen Only Mode Disable
    CAN->CAN_CR1 = (0X05 << 28);            //AFWL = 0X03
    CAN->CAN_CR1 &= ~(0x0F<<24);            //EWL = 0

//    CAN->CAN_CR1 |= 1<<1;                   //EIE
//    CAN->CAN_CR1 |= 1<<2;                   //TSIE
//    CAN->CAN_CR1 |= 1<<3;                   //TPIE
//    CAN->CAN_CR1 |= 1<<4;                   //RAFIE
//    CAN->CAN_CR1 |= 1<<5;                   //RFIE
//    CAN->CAN_CR1 |= 1<<6;                   //ROIE
    CAN->CAN_CR1 |= 1<<7;                   //RIE
   
//    CAN->CAN_CR1 |= 1<<17;                   //BEIE
//    CAN->CAN_CR1 |= 1<<19;                   //ALIE
//    CAN->CAN_CR1 |= 1<<21;                   //EPIE
    

}

void CAN_Transmit(CAN_TxMsg *TxMessage, uint8_t CAN_WorkMode, uint8_t tbsel)
{
    uint32_t tempDATA = 0;
    uint8_t count, data_length;
    if(tbsel)
    {
        while((CAN->CAN_CR0 & Bit9_En) == Bit9_En);     //check TSALL status
        CAN->CAN_CR0 |= Bit15_En;                        //Select Transmit Buffer: STB
    }
    else
    {
        while((CAN->CAN_CR0 & Bit11_En) == Bit11_En);
        CAN->CAN_CR0 &= Bit15_Dis;                      //Select Transmit Buffer: PTB
    }

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
    
    if(tbsel)     //STB
    {
        CAN->CAN_CR0 |= Bit22_En;       //STB slot filled, select next FIFO slot.
        CAN->CAN_CR0 |= Bit10_En;       //Transmit Secondary one frame     
//      CAN->CAN_CR0 |= Bit8_En;          //Transmit Secondary ALL frames        
    }
    else        //PTB
    {
        CAN->CAN_CR0 |= Bit12_En;      
    }
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




