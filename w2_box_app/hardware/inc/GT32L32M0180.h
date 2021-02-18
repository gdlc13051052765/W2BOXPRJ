
#ifndef _GT32L32M0180_H_
#define _GT32L32M0180_H_
 
extern unsigned char r_dat_bat(unsigned long address,unsigned long byte_long,unsigned char *p_arr);

#define ASCII_5X7              1      //ASCII����5X7����
#define ASCII_7X8              2      //ASCII����7X8����
#define ASCII_7X8_F            3      //ASCII����7X8����Fat����ַ����������ο�����飩
#define ASCII_6X12             4      //ASCII����6X12����
#define ASCII_8X16             5      //ASCII����8X16����
#define ASCII_8X16_F           6      //ASCII����8X16����Fat����ַ����������ο�����飩
#define ASCII_12X24            7      //ASCII����12X24����
//#define ASCII_12X24_P          8      //ASCII����12X24�����ӡ��
#define ASCII_16X32            9      //ASCII����16X32����
#define ASCII_16X32_F         10      //ASCII����16X32����Fat����ַ����������ο�����飩
#define ASCII_12_A            11      //ASCII����12x12���󲻵ȿ�Arial����ַ����������ο�����飩		
#define ASCII_12_T            12		  //ASCII����12X12���󲻵ȿ�Time news Roman����ַ����������ο�����飩
#define ASCII_16_A            13      //ASCII����16X16���󲻵ȿ�Arial����ַ����������ο�����飩		
#define ASCII_16_T            14		  //ASCII����16X16���󲻵ȿ�Time news Roman����ַ����������ο�����飩
#define ASCII_24_A            15      //ASCII����24X24���󲻵ȿ�Arial����ַ����������ο�����飩		
#define ASCII_24_T            16		  //ASCII����24X24���󲻵ȿ�Time news Roman����ַ����������ο�����飩
#define ASCII_32_A            17      //ASCII����32X32���󲻵ȿ�Arial����ַ����������ο�����飩		
#define ASCII_32_T            18		  //ASCII����32X32���󲻵ȿ�Time news Roman����ַ����������ο�����飩

#define NUB_14X28							 0
#define NUB_20X40							 1
#define NUB_28X28							 2
#define NUB_40X40							 3


/*************************************************************
�����÷���
    unsigned char DZ_Data[���鳤�ȿͻ��Զ���];
    ASCII_GetData(0x41,ASCII_5X7,DZ_Data);      //��ȡ5X7���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ8 BYTE
	  ASCII_GetData(0x41,ASCII_7X8,DZ_Data);      //��ȡ7X8���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ8 BYTE
		ASCII_GetData(0x41,ASCII_7X8_F,DZ_Data);		//��ȡ7X8���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ8 BYTE
	  ASCII_GetData(0x41,ASCII_6X12,DZ_Data);     //��ȡ6X12���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ12 BYTE
	  ASCII_GetData(0x41,ASCII_8X16,DZ_Data);   	//��ȡ8X16���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ16 BYTE
	  ASCII_GetData(0x41,ASCII_8X16_F,DZ_Data);   //��ȡ8X16���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ16 BYTE
	  ASCII_GetData(0x41,ASCII_12X24,DZ_Data);		//��ȡ12X24���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ48 BYTE
		ASCII_GetData(0x41,ASCII_16X32,DZ_Data);		//��ȡ16X32���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ64 BYTE
		ASCII_GetData(0x41,ASCII_16X32_F,DZ_Data);	//��ȡ16X32���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ64 BYTE
		ASCII_GetData(0x41,ASCII_12_A,DZ_Data);     //��ȡ12X12���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ26 BYTE
		ASCII_GetData(0x41,ASCII_12_T,DZ_Data);     //��ȡ12X12���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ26 BYTE
	  ASCII_GetData(0x41,ASCII_16_A,DZ_Data);     //��ȡ16X16���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ34 BYTE
		ASCII_GetData(0x41,ASCII_16_T,DZ_Data);     //��ȡ16X16���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ34 BYTE
		ASCII_GetData(0x41,ASCII_24_A,DZ_Data);     //��ȡ24X24���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ74 BYTE
		ASCII_GetData(0x41,ASCII_24_T,DZ_Data);     //��ȡ24X24���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ74 BYTE
		ASCII_GetData(0x41,ASCII_32_A,DZ_Data);     //��ȡ32X32���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ128 BYTE
		ASCII_GetData(0x41,ASCII_32_T,DZ_Data);     //��ȡ32X32���� ASCII ����A�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ128 BYTE
				
*************************************************************/ 
unsigned char  ASCII_GetData(unsigned char  ASCIICode,unsigned long  ascii_kind,unsigned char *DZ_Data);

/*************************************************************
�����÷���
    unsigned char DZ_Data[���鳤�ȿͻ��Զ���];
    Dig_Ch_GetData(1,NUB_14X28,DZ_Data);	//��ȡ14X28�������ַ����ַ��������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ56 BYTE
	  Dig_Ch_GetData(1,NUB_20X40,DZ_Data);	//��ȡ20X40�������ַ����ַ��������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ120 BYTE
		Dig_Ch_GetData(1,NUB_28X28,DZ_Data);	//��ȡ28X28�������ַ����ַ��������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ112 BYTE
	  Dig_Ch_GetData(1,NUB_40X40,DZ_Data);	//��ȡ40X40�������ַ����ַ��������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ200 BYTE
*************************************************************/ 
unsigned char	 Dig_Ch_GetData(unsigned char  Sequence,unsigned long  NUB_kind,unsigned char *DZ_Data);
 
/*************************************************************
�����÷���
    unsigned char DZ_Data[���鳤�ȿͻ��Զ���];
    gt_12_GetData(0xb0,0xa1,0x00,0x00,DZ_Data); //��ȡ12X12���� ����"��"�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ24 BYTE
*************************************************************/
unsigned long  gt_12_GetData(unsigned char c1,unsigned char c2,unsigned char c3,unsigned char c4,unsigned char *DZ_Data);

/*************************************************************
�����÷���
    unsigned char DZ_Data[���鳤�ȿͻ��Զ���];
    gt_16_GetData(0xb0,0xa1,0x00,0x00,DZ_Data); //��ȡ16X16���� ����"��"�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ24 BYTE
*************************************************************/ 
unsigned long  gt_16_GetData(unsigned char  c1, unsigned char  c2, unsigned char  c3, unsigned char  c4,unsigned char *DZ_Data);

/*************************************************************
�����÷���
    unsigned char DZ_Data[���鳤�ȿͻ��Զ���];
    gt_24_GetData(0xb0,0xa1,0x00,0x00,DZ_Data); //��ȡ24X24���� ����"��"�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ72 BYTE
*************************************************************/ 
unsigned long  gt_24_GetData(unsigned char  c1, unsigned char  c2, unsigned char  c3, unsigned char  c4,unsigned char *DZ_Data);

/*************************************************************
�����÷���
    unsigned char DZ_Data[���鳤�ȿͻ��Զ���];
    gt_32_GetData(0xb0,0xa1,0x00,0x00,DZ_Data); //��ȡ32X32���� ����"��"�ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ128 BYTE
*************************************************************/ 
unsigned long  gt_32_GetData(unsigned char  c1,unsigned char  c2,unsigned char  c3,unsigned char  c4,unsigned char *DZ_Data);

/*************************************************************
�����÷���
    unsigned long BAR_NUM[13]={0,1,2,3,4,5,6,7,8,9,1,2,3};
    unsigned long BAR_ADDR[13];
		BAR_CODE13(BAR_NUM,BAR_ADDR); //��ȡ������ĵ�ַ��������ַ��������BAR_ADDR��
*************************************************************/
unsigned long  BAR_CODE13(unsigned char  * BAR_NUM,unsigned char *BAR_PIC_ADDR);
/*************************************************************
�����÷���
    unsigned long BAR_NUM[13]={0,1,2,3,4,5,6,7,8,9,1,2,3};
    unsigned long BAR_ADDR[7];
		BAR_CODE128(BAR_NUM,1,BAR_ADDR); //��ȡ������ĵ�ַ��������ַ��������BAR_ADDR��
��ʼ����3��ģʽ
��flag=1ʱΪCode-128-A;
��flag=2ʱΪCode-128-B;
��flag=3ʱΪCode-128-C;		
*************************************************************/
unsigned long  BAR_CODE128(unsigned char  *BAR_NUM,unsigned char  flag,unsigned char *BAR_PIC_ADDR);

/*************************************************************
�����÷���
    unsigned char DZ_Data[���鳤�ȿͻ��Զ���];
    Antenna_CODE_12X12_GetData(0,DZ_Data); //��ȡ12X12���������ַ��ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ24 BYTE
*************************************************************/ 
unsigned long  Antenna_CODE_12X12_GetData(unsigned char  NUM,unsigned char *DZ_Data);

/*************************************************************
�����÷���
    unsigned char DZ_Data[���鳤�ȿͻ��Զ���];
    Battery_CODE_12X12_GetData(0,DZ_Data); //��ȡ12X12�������ַ��ĵ������ݣ������������ݴ���DZ_Data�����У����ݳ���Ϊ24 BYTE
*************************************************************/ 
unsigned long  Battery_CODE_12X12_GetData(unsigned char   NUM,unsigned char *DZ_Data);

/*************************************************************
�����÷���
    unsigned char GBCode;
    GBCode = U2G(0x554A); //��unicode��������ת��GBK����
*************************************************************/ 
unsigned long	U2G(unsigned int  unicode);

/*************************************************************
�����÷���
    unsigned char GBCode;
    GBCode = U2G_13(0x554A); //��unicode��������ת��GBK���루�ַ�����
*************************************************************/ 
unsigned int  U2G_13(unsigned int  Unicode);

/*************************************************************
�����÷���
    unsigned char GBCode;
    GBCode = BIG5_G(0xB0DA); //��Big5��������ת��GBK���� 
*************************************************************/ 
unsigned long  BIG5_G(unsigned int  B5code);
 


#endif




