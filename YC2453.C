
#include	"extern.h"

//#define READ_CHECK  //写入数据读出校验宏定义

//#define SSV_SET_RX
#ifndef SSV_SET_RX 
#define SSV_SET_TX
#endif
#define NEW_INIT
/* SSV7241 Commands */
#define R_REG					0x00
#define W_REG 					0x20
#define R_RX_PLOAD 				0x61
#define W_TX_PLOAD				0xA0
#define FLUSH_TX				0xE1
#define FLUSH_RX				0xE2
#define REUSE_TX_PL				0xE3
#define R_RX_PL_WID				0x60
#define W_ACK_PLOAD				0xA8
#define W_TX_PLOAD_NOACK 	    0xB0
#define D_NOP					0xFF

/* SSV7241 Registers */
#define CFG_TOP 				0x00
#define	EN_AA					0x01
#define	EN_RXADDR				0x02
#define MASK_RX_DR	   		 	0x40
#define MASK_TX_DS	    		0x20
#define MASK_MAX_RT	    		0x10
#define EN_CRC					0x08
#define CRC_2B					0x04
#define PWR_ON					0x02
#define RX_ON					0x01

/* Enable/Disable Dynamic Payload , ACK Payload, Dynamic ACK function */
#define FEATURE					0x1D
#define EN_DPL					0x04
#define	EN_ACK_PAY	    		0x02
#define EN_DYN_ACK				0x01
#define BANK_SEL				0x1F

/* RSSI indicator and Enable/Disable Setting */
#define RSSI					0x09
#define EN_RSSI					0x10
#define RSSI_2					0x02 // - 59dBm
#define RSSI_1					0x01 // - 69dBm
/* Channel number is from 0 (2400) to 127 (2527) */
#define RF_CH					0x05

/* FIFO Status */
#define STATUS_FIFO	    		0x17
#define TX_REUSE				0x40
#define TX_FULL					0x20
#define TX_EMPTY				0x10
#define RX_FULL					0x02
#define RX_EMPTY				0x01

/* Interrupt status */
#define STATUS					0x07
#define RX_DR					0x40
#define TX_DS					0x20
#define MAX_RT					0x10
#define RX_P_NO_5				0x0C
#define RX_P_NO_4				0x0A
#define RX_P_NO_3				0x08
#define RX_P_NO_2				0x06
#define RX_P_NO_1				0x04
#define RX_P_NO_0				0x02



#define KEY_F                   0X01
#define KEY_B                   0X02
#define KEY_L                   0X04
#define KEY_R                   0X08
#define KEY_F_L                 0X05
#define KEY_F_R                 0X09
#define KEY_B_L                 0X06
#define KEY_B_R                 0X0A

#ifdef SSV_SET_TX
//#define KEY_MULTI_FUNC  //多功能按键  <----------------------------------
	/* 键值定义 */


	#define NO_KEY          		0x00

#ifdef KEY_MULTI_FUNC
	#define KEY_DOWN       			0x10
	#define KEY_LONG        		0x20
	#define KEY_LIAN       	 		0x40
	#define KEY_UP          		0x80

	#define KEY_F_DOWN              KEY_F|KEY_DOWN
	#define KEY_F_LONG              KEY_F|KEY_LONG
	#define KEY_F_LIAN              KEY_F|KEY_LIAN
	#define KEY_F_UP                KEY_F|KEY_UP

	#define KEY_B_DOWN              KEY_B|KEY_DOWN
	#define KEY_B_LONG              KEY_B|KEY_LONG
	#define KEY_B_LIAN              KEY_B|KEY_LIAN
	#define KEY_B_UP                KEY_B|KEY_UP

	#define KEY_L_DOWN              KEY_L|KEY_DOWN
	#define KEY_L_LONG              KEY_L|KEY_LONG
	#define KEY_L_LIAN              KEY_L|KEY_LIAN
	#define KEY_L_UP                KEY_L|KEY_UP

	#define KEY_R_DOWN              KEY_R|KEY_DOWN
	#define KEY_R_LONG              KEY_R|KEY_LONG
	#define KEY_R_LIAN              KEY_R|KEY_LIAN
	#define KEY_R_UP                KEY_R|KEY_UP

	#define KEY_SERIES_FLAG         100      //按键连发开始所需时间长度
	#define KEY_SERIES_DELAY        5       //按键连发的时间间隔长度
#endif

	LED0            BIT PA.7;
	KEY0            BIT PA.0;
	KEY1            BIT PA.4;
	KEY2            BIT PA.3;
	KEY3            BIT PA.5;      //按键定义
#else
	MOTO_R            BIT PA.7;
	MOTO_L            BIT PA.0;
	MOTO_B            BIT PA.4;
	MOTO_F            BIT PA.3;
                                  //输出控制定义
#endif
ADD_SET         BIT PA.6;

SSV_CS          BIT PB.6;
SSV_CE          BIT PB.5;
/*************** SPI DEFIN E***************************/
SPI_Out			BIT	PB.0;     //SDO   
SPI_OutMode		BIT	PBC.0;
SPI_In			BIT	PB.1;     //SDI
SPI_InMode		BIT	PBC.1;
SPI_Clk			BIT	PB.7;     //CLK
SPI_ClkMode		BIT	PBC.7;
/*****************************************************/
byte SSV_FLAG;
BIT  RX_READY_FLAG  : SSV_FLAG.0;
BIT  RX_EMPTY_FLAG  : SSV_FLAG.1;
BIT  RX_PACKET_FLAG : SSV_FLAG.2;
BIT  TX_FULL_FLAG   : SSV_FLAG.3;

#define RX_LEN  5
#define TX_LEN  5
byte RX_BUFF[RX_LEN];
byte TX_BUFF[TX_LEN];


byte DYN_ACKOn; 
byte ACK_PAYOn;

//byte Kv_cnt;

byte SSV_RDKey;

byte Key_Return;
/************************************************************
#ifdef simplified 
const word Initial_Reg_Array[] = {
0x1F, 0x01, 0x00,
0x1B, 0x04, 0x10, 0xE1, 0xD3, 0x3D,
0x19, 0x04, 0x06, 0xAA, 0xA2, 0xDB,
0x1F, 0x01, 0x01,
0x19, 0x04, 0x77, 0x48, 0x9A, 0xE8,
0x1B, 0x04, 0x76, 0x87, 0xCA, 0x01,
0x1F, 0x01, 0x02,
0x1B, 0x04, 0xA0, 0x00, 0x18, 0xA0,
0x1F, 0x01, 0x04,
0x18, 0x04, 0x01, 0x00, 0xF0, 0x00,
0x1F, 0x01, 0x05,
0x18, 0x04, 0x84, 0x03, 0x2A, 0x03,
0x19, 0x04, 0x90, 0xBF, 0x00, 0x00,
0x1A, 0x04, 0xA0, 0x0F, 0x00, 0x00,
0x1F, 0x01, 0x00
};
#else
const word Initial_Reg_Array[] = {
//add  len  data   data  data  data     //第一列：寄存器地址；第二列：数据长度；第三列...：数据
0x1F, 0x01, 0x00,
0x1B, 0x04, 0x10, 0xE1, 0xD3, 0x3D,
0x19, 0x04, 0x06, 0xAA, 0xA2, 0xDB,
0x1F, 0x01, 0x01,
0x19, 0x04, 0x77, 0x48, 0x9A, 0xE8,
0x1B, 0x04, 0x76, 0x87, 0xCA, 0x01,
0x1F, 0x01, 0x02,
0x1B, 0x04, 0xA0, 0x00, 0x18, 0xA0,
0x1F, 0x01, 0x04,
0x18, 0x04, 0x01, 0x00, 0xF0, 0x00,
0x1F, 0x01, 0x05,
0x18, 0x04, 0x84, 0x03, 0x2A, 0x03,
0x19, 0x04, 0x90, 0xBF, 0x00, 0x00,
0x1A, 0x04, 0xA0, 0x0F, 0x00, 0x00,
0x1F ,0x01 ,0x08,
0x19 ,0x01 ,0x10,
0x18 ,0x04 ,0x07 ,0x02 ,0x04 ,0x00,
0x19 ,0x01 ,0xD8,
0x19 ,0x01 ,0xF8,
0x18 ,0x04 ,0xD1 ,0x01 ,0xF8 ,0x03,
0x19 ,0x01 ,0x9E,
0x19 ,0x01 ,0xBE,
0x18 ,0x04 ,0x00 ,0x0A ,0x08 ,0x00,
0x19 ,0x01 ,0xDE,
0x19 ,0x01 ,0xFE,
0x18 ,0x04 ,0xCE ,0xC1 ,0xFF ,0x03,
0x19 ,0x01 ,0x9F,
0x19 ,0x01 ,0xbF,
0x18 ,0x04 ,0x00 ,0x08 ,0x08 ,0x00,
0x19 ,0x01 ,0xDF,
0x19 ,0x01 ,0xFF,
0x18 ,0x04 ,0x1D ,0xEE ,0xEF ,0x00,
0x19 ,0x01 ,0x1E,
0x19 ,0x01 ,0x3E,
0x18 ,0x04 ,0x07 ,0x5B ,0x30 ,0x0A,
0x19 ,0x01 ,0x5E,
0x19 ,0x01 ,0x7E,
0x18 ,0x04 ,0x66 ,0xCC ,0xFC ,0x00,
0x19 ,0x01 ,0x1F,
0x19 ,0x01 ,0x3F,
0x18 ,0x04 ,0x01 ,0x0B ,0x50 ,0x0C,
0x19 ,0x01 ,0x5F,
0x19 ,0x01 ,0x7F,
0x19 ,0x01 ,0x25,
0x1F ,0x01 ,0x09,
0x18 ,0x04 ,0x00 ,0x00 ,0xFE ,0xFE,
0x1F ,0x01 ,0x00
};
#endif
*******************************************************/ //mini c 不支持某些c操作，不用数组方式
void SSV_SetChannel(void);
void SSV_SetPW(void);
void SPI_Init(void)
{
	set0	SPI_Clk;         //拉低时钟线
	set1	SPI_ClkMode;     //设置CLK线输出
//	set0	SPI_InMode;      //设置SDI线输入
//	set1	SPI_OutMode;     //设置SDO线输出

	$ SPI_In   IN,LOW;     
	$ SPI_Out  OUT,HIGH,PULL;      
	$ SSV_CS   OUT,HIGH,PULL;      //SSV片选拉高
}

byte SPI_Data_In;
/*****************************************************
//SPI_RW;SPI读写数据
//以累加器ACC传入
******************************************************/
static	void	SPI_RW (void)
{
	BYTE	cnt;                //比特位计数
	BYTE	SPI_Data_Out;       //需要输出的数据

	SPI_Data_Out	=	A;      //从ACC获取需要传送的数据
	cnt	=	8;                  //初始化一个字节

	do                          //从这里开始发送1个字节，8位
	{
		sl		SPI_Data_Out;   //数据右移一位，最高位移入进位位
		nop;                    //延时1T
		set0	SPI_Clk;        //拉低时钟线备用
		.swapc_o SPI_Out;       //进位CF与SPI_OUT互换，即输出进位位的值
		.delay	3;              //延时3
		set1	SPI_Clk;        //发完1字节，拉高时钟线
		#if	_SYS(USE.SWAPC)
			swapc	SPI_In;
			slc		SPI_Data_In;
		#else
			sl		SPI_Data_In;
			if (SPI_In)	SPI_Data_In.0	=	1;
		#endif                      //这里是接收数据的两种实现方式，根据条件不同选不同方式
	} while (--cnt);            //循环判断
	nop;
    nop;
	set0	SPI_Clk;
	A = SPI_Data_In;
}

/*****************************************************
//ssvInit 初始化2.4g芯片
//TEST OK!
******************************************************/
#ifdef NEW_INIT
void SSV_Init(void)  //初始化ssv7241 2.4g  ic
{
	byte value;
//----------------------------------------------------------------
	SSV_CS = 0;
	A = 0X1F|W_REG;                 //写地址
	SPI_RW();
	A = 0x00;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1B|W_REG;                 //写地址;
	SPI_RW();
	A = 0X10;
	SPI_RW();
	A = 0XE1;
	SPI_RW();
	A = 0XD3;
	SPI_RW();
	A = 0X3D;
	SPI_RW(); //0XIB <==0X10;0XE1;0XD3;0X3D
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X06;
	SPI_RW();
	A = 0XAA;
	SPI_RW();
	A = 0XA2;
	SPI_RW();
	A = 0XDB;
	SPI_RW(); //0XI9 <==0X06;0XAA;0XA2;0XDB
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F|W_REG;                 //写地址;
	SPI_RW();
	A = 1;
	SPI_RW(); //0XIF <==0X01
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X77;
	SPI_RW();
	A = 0X48;
	SPI_RW();
	A = 0X9A;
	SPI_RW();
	A = 0XE8;
	SPI_RW(); //0XI9 <==0X77;0X48;0X9A;0XE8
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1B|W_REG;                 //写地址;
	SPI_RW();
	A = 0X76;
	SPI_RW();
	A = 0X87;
	SPI_RW();
	A = 0XCA;
	SPI_RW();
	A = 0X01;
	SPI_RW(); //0XIB <==0X76;0X87;0XCA;0X01
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F|W_REG;                 //写地址;
	SPI_RW();
	A = 2;
	SPI_RW(); //0XIF <==0X02
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1B|W_REG;                 //写地址;
	SPI_RW();
	A = 0XA0;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X18;
	SPI_RW();
	A = 0XA0;
	SPI_RW(); //0XIB <==0XA0;0X00;0X18;0XA0
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F|W_REG;                 //写地址;
    SPI_RW();
	A = 4;
	SPI_RW(); //0XIF <==0X04
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X01;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0XF0;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XIB <==0X01;0X00;0XF0;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F|W_REG;                 //写地址;
	SPI_RW();
	A = 5;
	SPI_RW(); //0XIF <==0X05
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X84;
	SPI_RW();
	A = 0X03;
	SPI_RW();
	A = 0X2A;
	SPI_RW();
	A = 0X03;
	SPI_RW(); //0XI8 <==0X84;0X03;0X2A;0X03
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X90;
	SPI_RW();
	A = 0XBF;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI9 <==0X90;0XBF;0X00;0X00
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1A|W_REG;                 //写地址;
	SPI_RW();
	A = 0XA0;
	SPI_RW();
	A = 0X0F;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XIB <==0XA0;0X0F;0X00;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F|W_REG;                 //写地址;
	SPI_RW();
	A = 8;
	SPI_RW(); //0XIF <==0X08
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X10;
	SPI_RW(); //0XI9 <==0X10
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X07;
	SPI_RW();
	A = 0X02;
	SPI_RW();
	A = 0X04;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0X07;0X02;0X04;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0XD8;
	SPI_RW(); //0XI9 <==0XD8
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0XF8;
	SPI_RW(); //0XI9 <==0XF8
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0XD1;
	SPI_RW();
	A = 0X01;
	SPI_RW();
	A = 0XF8;
	SPI_RW();
	A = 0X03;
	SPI_RW(); //0XI8 <==0XD1;0X01;0XF8;0X03
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X9E;
	SPI_RW(); //0XI9 <==0X9E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0XBE;
	SPI_RW(); //0XI9 <==0XBE
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X0A;
	SPI_RW();
	A = 0X08;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0X00;0X0A;0X08;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0XDE;
	SPI_RW(); //0XI9 <==0XDE
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0XFE;
	SPI_RW(); //0XI9 <==0XFE
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0XCE;
	SPI_RW();
	A = 0XC1;
	SPI_RW();
	A = 0XFF;
	SPI_RW();
	A = 0X03;
	SPI_RW(); //0XI8 <==0XCE;0XC1;0XFF;0X03
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X9F;
	SPI_RW(); //0XI9 <==0X9F
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0XBF;
	SPI_RW(); //0XI9 <==0XBF
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X08;
	SPI_RW();
	A = 0X08;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0X00;0X08;0X08;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0XDF;
	SPI_RW(); //0XI9 <==0XDF
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0XFF;
	SPI_RW(); //0XI9 <==0XFF
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X1D;
	SPI_RW();
	A = 0XEE;
	SPI_RW();
	A = 0XEF;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0X1D;0XEE;0XEF;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X1E;
	SPI_RW(); //0XI9 <==0X1E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X3E;
	SPI_RW(); //0XI9 <==0X3E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X07;
	SPI_RW();
	A = 0X5B;
	SPI_RW();
	A = 0X30;
	SPI_RW();
	A = 0X0A;
	SPI_RW(); //0XI8 <==0X07;0X5B;0X30;0X0A
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X5E;
	SPI_RW(); //0XI9 <==0X5E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X7E;
	SPI_RW(); //0XI9 <==0X7E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X66;
	SPI_RW();
	A = 0XCC;
	SPI_RW();
	A = 0XFC;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0X66;0XCC;0XFC;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X1F;
	SPI_RW(); //0XI9 <==0X1F
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X3F;
	SPI_RW(); //0XI9 <==0X3F
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X01;
	SPI_RW();
	A = 0X0B;
	SPI_RW();
	A = 0X50;
	SPI_RW();
	A = 0X0C;
	SPI_RW(); //0XI8 <==0X01;0X08;0X50;0X0C
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X5F;
	SPI_RW(); //0XI9 <==0X5F
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X7F;
	SPI_RW(); //0XI9 <==0X7F
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X25;
	SPI_RW(); //0XI9 <==0X25
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F|W_REG;                 //写地址;
	SPI_RW();
	A = 0X09;
	SPI_RW(); //0XIF <==0X09
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0XFE;
	SPI_RW();
	A = 0XFE;
	SPI_RW(); //0XI8 <==0X00;0X00;0XFE;0XFE
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F|W_REG;                 //写地址;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;
//----------------------------------------------------------------

	SSV_CS = 0;
	A = CFG_TOP|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A; 
	value |= (EN_CRC | CRC_2B);
	SSV_CS = 1;

	SSV_CS = 0;
	A = CFG_TOP|W_REG;                 //写地址;
	SPI_RW();
	A = value;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;

	SSV_CS = 0;
	A = EN_AA|W_REG;                 //写地址;
	SPI_RW();
	A = 0x01;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;

	SSV_CS = 0;
	A = EN_RXADDR|W_REG;                 //写地址;
	SPI_RW();
	A = 0x01;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;
    A = 75;
    SSV_SetChannel();
//#ifdef SSV_SET_RX
	SSV_SetPW();
//#endif
}
#else
void SSV_Init(void)  //初始化ssv7241 2.4g  ic
{
	byte value;
//----------------------------------------------------------------
	SSV_CS = 0;
	A = 0X1F;                 //写地址
	SPI_RW();
	A = 0x00;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1B;                 //写地址;
	SPI_RW();
	A = 0XD0;
	SPI_RW();
	A = 0XE1;
	SPI_RW();
	A = 0XD3;
	SPI_RW();
	A = 0X3D;
	SPI_RW(); //0XIB <==0X10;0XE1;0XD3;0X3D
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X06;
	SPI_RW();
	A = 0XAA;
	SPI_RW();
	A = 0X62;
	SPI_RW();
	A = 0XC3;
	SPI_RW(); //0XI9 <==0X06;0XAA;0XA2;0XDB
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F;                 //写地址;
	SPI_RW();
	A = 1;
	SPI_RW(); //0XIF <==0X01
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X5C;
	SPI_RW();
	A = 0X14;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X5B;
	SPI_RW(); //0XI9 <==0X77;0X48;0X9A;0XE8
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X77;
	SPI_RW();
	A = 0X48;
	SPI_RW();
	A = 0X9A;
	SPI_RW();
	A = 0XC8;
	SPI_RW(); //0XI9 <==0X77;0X48;0X9A;0XE8
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1B;                 //写地址;
	SPI_RW();
	A = 0X76;
	SPI_RW();
	A = 0X87;
	SPI_RW();
	A = 0XCA;
	SPI_RW();
	A = 0X01;
	SPI_RW(); //0XIB <==0X76;0X87;0XCA;0X01
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F;                 //写地址;
	SPI_RW();
	A = 2;
	SPI_RW(); //0XIF <==0X02
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1B;                 //写地址;
	SPI_RW();
	A = 0XA0;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X18;
	SPI_RW();
	A = 0XA0;
	SPI_RW(); //0XIB <==0XA0;0X00;0X18;0XA0
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F;                 //写地址;
    SPI_RW();
	A = 4;
	SPI_RW(); //0XIF <==0X04
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X01;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0XF0;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XIB <==0X01;0X00;0XF0;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F;                 //写地址;
	SPI_RW();
	A = 5;
	SPI_RW(); //0XIF <==0X05
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X84;
	SPI_RW();
	A = 0X03;
	SPI_RW();
	A = 0X2A;
	SPI_RW();
	A = 0X03;
	SPI_RW(); //0XI8 <==0X84;0X03;0X2A;0X03
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X90;
	SPI_RW();
	A = 0XBF;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI9 <==0X90;0XBF;0X00;0X00
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1A;                 //写地址;
	SPI_RW();
	A = 0XA0;
	SPI_RW();
	A = 0X0F;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XIB <==0XA0;0X0F;0X00;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F;                 //写地址;
	SPI_RW();
	A = 8;
	SPI_RW(); //0XIF <==0X08
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X10;
	SPI_RW(); //0XI9 <==0X10
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0XFB;
	SPI_RW();
	A = 0XFF;
	SPI_RW();
	A = 0X03;
	SPI_RW(); //0XI8 <==0X07;0X02;0X04;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X9E;
	SPI_RW(); //0XI9 <==0XD8
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0XBE;
	SPI_RW(); //0XI9 <==0XF8
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X07;
	SPI_RW();
	A = 0X02;
	SPI_RW();
	A = 0X04;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0XD1;0X01;0XF8;0X03
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0XDE;
	SPI_RW(); //0XI9 <==0X9E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0XFE;
	SPI_RW(); //0XI9 <==0XBE
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X7C;
	SPI_RW();
	A = 0X44;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0X00;0X0A;0X08;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X9F;
	SPI_RW(); //0XI9 <==0XDE
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0XBF;
	SPI_RW(); //0XI9 <==0XFE
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X07;
	SPI_RW();
	A = 0X02;
	SPI_RW();
	A = 0X04;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0XCE;0XC1;0XFF;0X03
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0XDF;
	SPI_RW(); //0XI9 <==0X9F
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0XFF;
	SPI_RW(); //0XI9 <==0XBF
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X11;
	SPI_RW();
	A = 0XFE;
	SPI_RW();
	A = 0XFF;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0X00;0X08;0X08;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X1E;
	SPI_RW(); //0XI9 <==0XDF
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X3E;
	SPI_RW(); //0XI9 <==0XFF
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X01;
	SPI_RW();
	A = 0X8B;
	SPI_RW();
	A = 0X30;
	SPI_RW();
	A = 0X08;
	SPI_RW(); //0XI8 <==0X1D;0XEE;0XEF;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X5E;
	SPI_RW(); //0XI9 <==0X1E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X7E;
	SPI_RW(); //0XI9 <==0X3E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X10;
	SPI_RW();
	A = 0XFF;
	SPI_RW();
	A = 0X85;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI8 <==0X07;0X5B;0X30;0X0A
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X1F;
	SPI_RW(); //0XI9 <==0X5E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X3F;
	SPI_RW(); //0XI9 <==0X7E
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X01;
	SPI_RW();
	A = 0X8B;
	SPI_RW();
	A = 0X30;
	SPI_RW();
	A = 0X0C;
	SPI_RW(); //0XI8 <==0X66;0XCC;0XFC;0X00
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X5F;
	SPI_RW(); //0XI9 <==0X1F
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X7F;
	SPI_RW(); //0XI9 <==0X3F
	SSV_CS = 1;
/*
	SSV_CS = 0;
	A = 0X18|W_REG;                 //写地址;
	SPI_RW();
	A = 0X01;
	SPI_RW();
	A = 0X0B;
	SPI_RW();
	A = 0X50;
	SPI_RW();
	A = 0X0C;
	SPI_RW(); //0XI8 <==0X01;0X08;0X50;0X0C
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X5F;
	SPI_RW(); //0XI9 <==0X5F
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X19|W_REG;                 //写地址;
	SPI_RW();
	A = 0X7F;
	SPI_RW(); //0XI9 <==0X7F
	SSV_CS = 1;
*/
	SSV_CS = 0;
	A = 0X19;                 //写地址;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XI9 <==0X25
//----------------------------------------------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F;                 //写地址;
	SPI_RW();
	A = 0X09;
	SPI_RW(); //0XIF <==0X09
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X18;                 //写地址;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0X00;
	SPI_RW();
	A = 0XF4;
	SPI_RW();
	A = 0XF8;
	SPI_RW(); //0XI8 <==0X00;0X00;0XFE;0XFE
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0X1F|W_REG;                 //写地址;
	SPI_RW();
	A = 0X00;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;
//----------------------------------------------------------------

	SSV_CS = 0;
	A = CFG_TOP|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A; 
	value |= (EN_CRC | CRC_2B);
	SSV_CS = 1;

	SSV_CS = 0;
	A = CFG_TOP|W_REG;                 //写地址;
	SPI_RW();
	A = value;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;

	SSV_CS = 0;
	A = EN_AA|W_REG;                 //写地址;
	SPI_RW();
	A = 0x01;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;

	SSV_CS = 0;
	A = EN_RXADDR|W_REG;                 //写地址;
	SPI_RW();
	A = 0x01;
	SPI_RW(); //0XIF <==0X00
	SSV_CS = 1;
    A = 75;
    SSV_SetChannel();
//#ifdef SSV_SET_RX
	SSV_SetPW();
//#endif
}
#endif

byte SSVADD[3];
void GET_Add(void)
{
  $ ADD_SET OUT;
    ADD_SET = 1;
  $ ADD_SET IN ; 


  if(ADD_SET == 1)
  {
  $ ADD_SET OUT;
     ADD_SET = 0; 
  $ ADD_SET IN ; 
	 if(ADD_SET == 0)  //悬空  注意单步执行检测不到悬空........
	 {
      SSVADD[0] = 0X3A;
	  SSVADD[1] = 0X94;
	  SSVADD[2] = 0XC2;
	 }
	 else              //上拉
	 {
      SSVADD[2] = 0X3A;
	  SSVADD[0] = 0X94;
	  SSVADD[1] = 0XC2;
	 }
  }
  else                 //下拉
  {
      SSVADD[2] = 0X3A;
	  SSVADD[1] = 0X94;
	  SSVADD[0] = 0XC2;
  }

}

/****************************************************************
//SSV7241  SET ADD
****************************************************************/
void SSV_SetAdd(void)
{
	   GET_Add();
//refine RF
//#ifdef SSV_SET_TX
		SSV_CS = 0;
		A = 0x10|W_REG;                 //写地址;
		SPI_RW(); //写地址
		A = 0x66;	
		SPI_RW(); //写地址值	
		A = SSVADD[0];	
		SPI_RW(); //写地址值
		A = SSVADD[1];	
		SPI_RW(); //写地址值
		A = SSVADD[2];	
		SPI_RW(); //写地址值
		A = 0x66;	
		SPI_RW(); //写地址值
		SSV_CS = 1;
	#ifdef READ_CHECK
		SSV_CS = 0;
		A = 0x10|R_REG;                 //读地址;
		SPI_RW(); //读地址
		A = 0xFF;	
		SPI_RW(); //读地址值	
		A = 0xFF;	
		SPI_RW(); //读地址值
		A = 0xFF;	
		SPI_RW(); //读地址值
		A = 0xFF;	
		SPI_RW(); //读地址值
		A = 0xFF;	
		SPI_RW(); //读地址值
		SSV_CS = 1;
	#endif
//#else
		SSV_CS = 0;
		A = 0x0A|W_REG;                 //写地址;
		SPI_RW(); //写地址
		A = 0x66;	
		SPI_RW(); //写地址值	
		A = SSVADD[0];	
		SPI_RW(); //写地址值
		A = SSVADD[1];	
		SPI_RW(); //写地址值
		A = SSVADD[2];	
		SPI_RW(); //写地址值
		A = 0x66;	
		SPI_RW(); //写地址值
		SSV_CS = 1;
	#ifdef READ_CHECK
		SSV_CS = 0;
		A = 0x0A|R_REG;                 //读地址;
		SPI_RW(); //读地址
		A = 0xFF;	
		SPI_RW(); //读地址值	
		A = 0xFF;	
		SPI_RW(); //读地址值
		A = 0xFF;	
		SPI_RW(); //读地址值
		A = 0xFF;	
		SPI_RW(); //读地址值
		A = 0xFF;	
		SPI_RW(); //读地址值
		SSV_CS = 1;
	#endif
//#endif
}
/****************************************************************
//SSV7241  SET SETUO_AW [3:0] 1 1 1 1
****************************************************************/
void SSV_SetAW(void)
{
	SSV_CS = 0;
	A = 0X03|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = 0x0f;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/****************************************************************
//SSV7241  SET RETRY
****************************************************************/
void SSV_SetRTY(void)
{
	SSV_CS = 0;
	A = 0X04|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = 0x13;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/****************************************************************
//SSV7241  SET RF
****************************************************************/
void SSV_SetRF(void)
{
	SSV_CS = 0;
	A = 0X06|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = 0x26;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/****************************************************************
//SSV7241  SET PW
****************************************************************/
void SSV_SetPW(void)
{
	SSV_CS = 0;
	A = 0X11|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = 0x01;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/****************************************************************
//SSV7241  POWER ON
****************************************************************/
void SSV_PowerOn(void)
{
	byte value;
//refine RF
	SSV_CS = 0;
	A = 0x19|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = 0x06;	
	SPI_RW(); //写地址值	
	A = 0xAA;	
	SPI_RW(); //写地址值
	A = 0xA2;	
	SPI_RW(); //写地址值
	A = 0xDB;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
#ifdef READ_CHECK
	SSV_CS = 0;
	A = 0x19|R_REG;                 //写地址;
	SPI_RW(); //写地址
	A = 0x06;	
	SPI_RW(); //写地址值	
	A = 0xAA;	
	SPI_RW(); //写地址值
	A = 0xA2;	
	SPI_RW(); //写地址值
	A = 0xDB;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
#endif
	SSV_CS = 0;
	A = CFG_TOP|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A; 
	value |=PWR_ON;
	SSV_CS = 1;

	SSV_CS = 0;
	A = CFG_TOP|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
	SSV_CS = 1;

#ifdef READ_CHECK
	SSV_CS = 0;
	A = CFG_TOP|R_REG;                 //写地址;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
#endif
}
/****************************************************************
//SSV7241  POWER OFF
****************************************************************/
void SSV_PowerOff(void)
{
	byte value;
	SSV_CS = 0;
	A = CFG_TOP|R_REG;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A; 
	value &=(~PWR_ON);
	SSV_CS = 1;

	SSV_CS = 0;
	A = CFG_TOP|W_REG;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
//refine RF
	SSV_CS = 1;

	SSV_CS = 0;
	A = 0x19|W_REG;
	SPI_RW(); //写地址
	A = 0x06;	
	SPI_RW(); //写地址值	
	A = 0xAA;	
	SPI_RW(); //写地址值
	A = 0x62;	
	SPI_RW(); //写地址值
	A = 0x43;	
	SPI_RW(); //写地址值

}
/*********************************************
//打开 RSSI indicator  RSSI1 -59DBM   RSSI2 -69DBM
*********************************************/
void SSV_EnableRSSI(void)
{
	byte value;
	SSV_CS = 0;
	A = RSSI|R_REG;                 //读地址
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A; 
	SSV_CS = 1;
	SSV_CS = 0;
	value |= EN_RSSI;   //<-----------------------------
	A = RSSI|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/*********************************************
//
*********************************************/
void SSV_DisableRSSI(void)
{
	byte value;
	SSV_CS = 0;
	A = RSSI|R_REG;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A; 	
	value &= (~EN_RSSI);//<---------------------------
	SSV_CS = 1;

	SSV_CS = 0;
	A = RSSI|W_REG;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/*********************************************
// Channel number is from 0 (2400) to 127 (2527)
//
*********************************************/
void SSV_SetChannel(void)
{
	byte channel;
	SSV_CS = 0;
	channel = A;
	A = RF_CH|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = channel;	
	SPI_RW(); //写地址值
	SSV_CS = 1;

#ifdef READ_CHECK
	SSV_CS = 0;
	A = RF_CH|R_REG;                 //写地址;
	SPI_RW(); //写地址	
	SPI_RW(); //写地址值
	SSV_CS = 1;
#endif
  //--------------------------------------------------->读出校验是正确的！
}

byte ID_CK;
/*********************************************
//检测是否为SSV7241
*********************************************/
void SSV_CheckID(void)
{
	ID_CK = 0;
	byte data[4];
	
//When power off, Chip ID can be RW 
	SSV_PowerOff();
//When RSSI disable, Chip ID can be RW 
	SSV_DisableRSSI();
	SSV_CS = 0;
	A = 0x18|R_REG;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	data[0] = A;
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	data[1] = A;
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	data[2] = A;
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	data[3] = A;

	if((data[0] == 0x41) && (data[1] == 0x72))
	ID_CK = 1;
	SSV_CS = 1;
}

#ifdef SSV_SET_RX
/*********************************************
//
*********************************************/
void SSV_isRXReady(void)
{
	byte state;
	SSV_CS = 0;
	RX_READY_FLAG = 0;       //接收标志清0
	A = STATUS|R_REG;                 //读地址
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	state = A & RX_DR;  
	SSV_CS = 1;
	if(state)
	RX_READY_FLAG = 1;       //接收标志置1
}
#endif
#ifdef SSV_SET_TX
void SSV_isTXFULL(void)
{
	byte state;
	SSV_CS = 0;
	TX_FULL_FLAG = 0;       //接收标志清0
	A = STATUS_FIFO|R_REG;                 //读地址
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	state = A & TX_FULL;  
	SSV_CS = 1;
	if(state)
	TX_FULL_FLAG = 1;       //接收标志置1
}
#endif
#ifdef SSV_SET_RX
void SSV_isRXEMPTY(void)
{
	byte state;
	SSV_CS = 0;
	RX_EMPTY_FLAG = 0;       //接收标志清0
	A = STATUS_FIFO|R_REG;                 //读地址
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	state = A & RX_EMPTY; 
	SSV_CS = 1;
	if(state)
	RX_EMPTY_FLAG = 1;       //接收标志置1
}
/***********************************************************************************
//清除接收到的封包中断
***********************************************************************************/
void SSV_ClearRX_DR(void)
{
	byte state;
	SSV_CS = 0;
	A = STATUS|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	state = A | RX_DR;  
	SSV_CS = 1;
	SSV_CS = 0;
	A = STATUS|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = state;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
#endif
#ifdef SSV_SET_TX
/***********************************************************************************
//清除发出的封包中断
***********************************************************************************/
void SSV_ClearTX_DS(void)
{
	byte state;
	SSV_CS = 0;
	A = STATUS|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	state = A | TX_DS;  
	SSV_CS = 1;
	SSV_CS = 0;
	A = STATUS|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = state;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/***********************************************************************************
//清除重发上限中断
***********************************************************************************/
void SSV_ClearMAX_RT(void)
{
	byte state;
	SSV_CS = 0;
	A = STATUS|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	state = A | MAX_RT; 
	SSV_CS = 1;
	SSV_CS = 0;
	A = STATUS|W_REG; 
	SPI_RW(); //写地址
	A = state;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
#endif
#ifdef SSV_SET_RX
void SSV_FlushRX(void)
{
	SSV_CS = 0;
	A = FLUSH_RX|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = D_NOP;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
#endif
#ifdef SSV_SET_TX
void SSV_FlushTX(void)
{
	SSV_CS = 0;
	A = FLUSH_TX|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = D_NOP;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
#endif
#ifdef SSV_SET_RX
/***********************************************************************************
//接收封包
***********************************************************************************/
void SSV_RxPacket(void)
{
	byte len = 0;
	word point;
	point = RX_BUFF; 
	RX_PACKET_FLAG = 0;
	//Check and Receive Data
	SSV_isRXReady();
	if(RX_READY_FLAG == 1)
	{
	/*	SSV_CS = 0;
		A = R_RX_PL_WID|R_REG;                 //读地址;    //数据包长度寄存器地址
		SPI_RW(); //读地址
		A = 0XFF;
		SPI_RW(); //读地址值存ACC
		len = A;            //获取数据包长度
		SSV_CS = 1;
		if(len==0)
			SSV_FlushRX();
		else
		{
   	      SSV_CS = 0;
  	      A = R_RX_PLOAD|R_REG;                 //读地址;
      	  SPI_RW(); //读地址
	
		  do
		  {
	        A = 0XFF;
			SPI_RW(); //读地址值存ACC
			*point = A;
			point++;
			if(point >= RX_LEN) break;
		  }
		  while(point < len);
		  SSV_CS = 1;
		}*/
        SSV_CS = 0;
		A = 0x61;// R_RX_PLOAD|R_REG;                 //读地址
		SPI_RW(); //读地址
		A = 0XFF;
		SPI_RW(); //读地址值存ACC
		SSV_RDKey = A;            //获取数据
		SSV_CS = 1;

		//Clear RX_DR status
		SSV_ClearRX_DR();		
		RX_PACKET_FLAG = 1;
	}
}
#endif
/***********************************************************************************
//设置dynamic ack
***********************************************************************************/
void SSV_Feature_DYN_ACK(void)
{
	byte value,Dyn_ack;
	Dyn_ack = A;
	/* Dynamic Ack Setting */
	SSV_CS = 0;
	A = FEATURE|R_REG;                 //读地址;   
	SPI_RW(); //读地址
    A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A; 
	SSV_CS = 1;
	if(Dyn_ack)
	{
		value |= EN_DYN_ACK;
		DYN_ACKOn = 1;
	}
	else
	{
		value &= ~EN_DYN_ACK;
		DYN_ACKOn = 0;
	}
	SSV_CS = 0;
	A = FEATURE|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
#ifdef SSV_SET_TX
/***********************************************************************************
//发送封包
***********************************************************************************/
void SSV_TxPacket(void)
{ 
    word point;
	BYTE len;
	SSV_CS = 0;
	point =TX_BUFF;
	//Check Queue is full or not and Send packet
	SSV_isTXFULL();
	SSV_CS = 1;
//	if(TX_FULL_FLAG == 0)
	{	
		if(!DYN_ACKOn)
		{
          SSV_CS = 0;
  	      A = W_TX_PLOAD|W_REG;                 //写地址
      	  SPI_RW(); //写地址
		  do
		  {
		  	A = *point ;
			point++;
			SPI_RW(); //ACC值写入TX FIFO

			if(point >= TX_LEN) break;
		  }
		  while(point < len);
     	  SSV_CS = 1;
		}  
/*		else
		{
		  SSV_CS = 0;
  	      A = W_TX_PLOAD_NOACK|W_REG;                 //写地址
      	  SPI_RW(); //写地址
		  do
		  {
		  	A = *point ;
			point++;
			SPI_RW(); //ACC值写入TX FIFO

			if(point >= TX_LEN) break;
		  }
		  while(point < len);
		  SSV_CS = 1;
		}
*/		
		SSV_CE = 1;
		.DELAY 20;   //DELAY 10US
		SSV_CE = 0;	
	}
}

/***********************************************************************************
//接收端不需回复ACK
***********************************************************************************/
void SSV_TxPacketWithoutAck(void)
{ 
    word point;
	BYTE len;
	SSV_CS = 0;
	point =TX_BUFF;
	//Check Queue is full or not and Send packet
	SSV_isTXFULL();
	SSV_CS = 1;
	if(TX_FULL_FLAG == 0)
	{	
		if(DYN_ACKOn)
		{
       	  SSV_CS = 0;
  	      A = W_TX_PLOAD|W_REG;                 //写地址
      	  SPI_RW(); //写地址
		  do
		  {
		  	A = *point ;
			point++;
			SPI_RW(); //ACC值写入TX FIFO

			if(point >= TX_LEN) break;
		  }
		  while(point < len);
	      SSV_CS = 1;
		}  
		else
		{
		  SSV_CS = 0;
  	      A = W_TX_PLOAD_NOACK|W_REG;                 //写地址
      	  SPI_RW(); //写地址
		  do
		  {
		  	A = *point ;
			point++;
			SPI_RW(); //ACC值写入TX FIFO

			if(point >= TX_LEN) break;
		  }
		  while(point < len);
		  SSV_CS = 1;
		}
		
		SSV_CE = 1;
		.DELAY 20;   //DELAY 10US
		SSV_CE = 0;	
	}
}

/***********************************************************************************
//允许ACK封包带数据
***********************************************************************************/
void SSV_TxACKPacket(void)
{ 
    word point;
	BYTE len;
	SSV_CS = 0;
	point =TX_BUFF;
	//Check Queue is full or not and Send packet
	SSV_isTXFULL();
	SSV_CS = 1;
	if(TX_FULL_FLAG == 0)
	{
		SSV_CS = 0;
  	    A = W_ACK_PLOAD|R_REG;                 //写地址;
        SPI_RW(); //写地址 
		do
		{
			A = *point ;
			point++;
			SPI_RW(); //写地址值存ACC
			if(point >= TX_LEN) break;
		}
		while(point < len);
	    SSV_CS = 1;
		SSV_CE = 1;
		.DELAY 20;   //DELAY 10US
		SSV_CE = 0;	
	}
}
#endif
#ifdef SSV_SET_RX
/***********************************************************************************
//开启RX模式
***********************************************************************************/
void SSV_RX_Mode(void)
{	  	     
    byte value;
	SSV_CS = 0;
	A = CFG_TOP|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A | RX_ON;  
	SSV_CS = 1;
	SSV_CS = 0;
	A = CFG_TOP|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
	SSV_CS = 1;

//	SSV_CE = 1;
}
#endif
#ifdef SSV_SET_TX
/***********************************************************************************
//开启TX模式
***********************************************************************************/
void SSV_TX_Mode(void)
{
    byte value;
	SSV_CS = 0;
	A = CFG_TOP|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A & (~RX_ON);  
	SSV_CS = 1;
	SSV_CS = 0;
	A = CFG_TOP|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
#endif
/***********************************************************************************
//设置dynamic payload
***********************************************************************************/
void SSV_Feature_DPL(void)
{
	byte value,Dpl;
    Dpl = A;	
	/* Dynamic Ack Setting */
	SSV_CS = 0;
	A = FEATURE|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A;  
	SSV_CS = 1;
	if(Dpl)
		value |= EN_DPL;
	else
		value &= ~EN_DPL;
	SSV_CS = 0;
	A = FEATURE|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}

/***********************************************************************************
//设置 ack payload
***********************************************************************************/
void SSV_Feature_ACK_PAY(void)
{
	byte value,Ack_pay;
	Ack_pay = A;
	/* Ack Payload Setting */
	SSV_CS = 0;
	A = FEATURE|R_REG;                 //读地址;
	SPI_RW(); //读地址
	A = 0XFF;
	SPI_RW(); //读地址值存ACC
	value = A;  
	SSV_CS = 1;
	if(Ack_pay)
	{
		value |= EN_ACK_PAY;
		ACK_PAYOn = 1;
	}
	else
	{
		value &= ~EN_ACK_PAY;
		ACK_PAYOn = 0;
	}
	SSV_CS = 0;	
	A = FEATURE|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = value;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/***********************************************************************************
//开启0dbm 模式
***********************************************************************************/
void SSV_Enable0dBm(void)
{
	/* refine RF TX power*/
	SSV_CS = 0;
	A = 0x1F|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = 0x01;	
	SPI_RW(); //写地址值	
	SSV_CS = 1;
	SSV_CS = 0;
	A = 0x18|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = 0x3F;	
	SPI_RW(); //写地址值	
	A = 0x17;	
	SPI_RW(); //写地址值	
	A = 0x01;	
	SPI_RW(); //写地址值	
	A = 0xDB;	
	SPI_RW(); //写地址值	
	SSV_CS = 1;
	SSV_CS = 0;
	A = 0x1F|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = 0x00;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/***********************************************************************************
//开启保住接收封包功率资讯
***********************************************************************************/
void SSV_EnableRSSIHoldPacketMode(void)
{
	/* refine RF mode*/
	SSV_CS = 0;
	A = 0x1F|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = 0x09;	
	SPI_RW(); //写地址值	
	SSV_CS = 1;
	SSV_CS = 0;
	A = 0x18|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = 0x00;	
	SPI_RW(); //写地址值	
	A = 0x00;	
	SPI_RW(); //写地址值	
	A = 0xFB;	
	SPI_RW(); //写地址值	
	A = 0xF6;	
	SPI_RW(); //写地址值	
	SSV_CS = 1;
	SSV_CS = 0;
	A = 0x1F|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = 0x00;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}
/***********************************************************************************
//关闭保住接收封包功率资讯
***********************************************************************************/
void SSV_DisableRSSIHoldPacketMode(void)
{
	/* refine RF mode*/
	SSV_CS = 0;
	A = 0x1F|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = 0x09;	
	SPI_RW(); //写地址值	
	SSV_CS = 1;
	SSV_CS = 0;
	A = 0x18|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = 0x00;	
	SPI_RW(); //写地址值	
	A = 0x00;	
	SPI_RW(); //写地址值	
	A = 0xFE;	
	SPI_RW(); //写地址值	
	A = 0xFE;	
	SPI_RW(); //写地址值	
	SSV_CS = 1;
	SSV_CS = 0;
	A = 0x1F|W_REG;                 //写地址;;
	SPI_RW(); //写地址
	A = 0x00;	
	SPI_RW(); //写地址值
	SSV_CS = 1;
}

#ifdef SSV_SET_TX

  #ifdef KEY_MULTI_FUNC
	    byte Key_State;        //按键状态
	    byte Key_Prev ;        //上一次按键
	    byte Key_Delay;        //按键连发时间
	    byte Key_Series;        //标志连发开始
		void Key_Init(void)
		{
		    Key_State =0;        //按键状态
		    Key_Prev =0;        //上一次按键
		    Key_Delay =0;        //按键连发时间
		    Key_Series =0;        //标志连发开始
   		}
   #else
	    byte Key_State;        //按键状态
	    byte Key_Prev ;        //上一次按键
		void Key_Init(void)
		{
		    Key_State =0;        //按键状态
		    Key_Prev =0;        //上一次按键
		}

   #endif
void Get_Key(void)
{
	static byte keyval;
	keyval = NO_KEY;                //NO_KEY = 0;
    if(KEY0 ==0)
	{
		keyval |= KEY_F;   //0X01
	}
    else if(KEY1 ==0)
	{
		keyval |= KEY_B;   //0X02     //前后不可组合
	}

    if(KEY2 ==0)
	{
		keyval |= KEY_L;   //0X04
	}
    else if(KEY3 ==0) 
	{
		keyval |= KEY_R;   //0X08     //左右不可组合
	}
    A = keyval;
}

void Key_Scan(void)
{
    byte Key_Press  = NO_KEY;           //按键值
    Key_Return = NO_KEY;               //按键返回值

    Get_Key();                          //获取键值
    Key_Press  = A;                     //传递键值

    switch (Key_State)
    {

    case 0:                            //按键初始态00
        if (Key_Press !=NO_KEY)        //有按键按下
        {
    #ifdef KEY_MULTI_FUNC 
			Key_Delay = 0;
            Key_Prev  = Key_Press;    //保存按键状态
    #endif
            Key_State = 1;            //转到按键确认
        }
        break;

    case 1:                          //按键确认态01

    #ifdef KEY_MULTI_FUNC 
        if ( Key_Press ==Key_Prev )  //确认和上次按键相同
        {
            Key_State = 2;          //判断按键长按

                                    //返回按键按下键值,按键按下就响应,如果想弹起来再响应
                                    //可以在弹起来后再返回按键值
       
            Key_Return = KEY_DOWN | Key_Prev;
        }
    #else
       if (Key_Press !=NO_KEY)
	   {
            Key_Return = Key_Press;
 	   }
    #endif
        else                       //按键抬起,是抖动,不响应按键
        {
            Key_State = 0;
        }
        break;

    case 2:                         //按键释放态10
        if (Key_Press == NO_KEY )   //按键释放了
        {
            Key_State = 0;
    #ifdef KEY_MULTI_FUNC
            Key_Delay = 0;
            Key_Series  = 0;
            Key_Return  = KEY_UP | Key_Prev;      //返回按键抬起值
    #endif
            break;
        }
    #ifdef KEY_MULTI_FUNC
        if ( Key_Press ==Key_Prev )
        {
            Key_Delay++;
            if ((Key_Series==1) && (Key_Delay>KEY_SERIES_DELAY))
            {
                Key_Delay  = 0;
                Key_Return = KEY_LIAN | Key_Press;  //返回连发的值
                Key_Prev   = Key_Press;             //记住上次的按键.
                break;
            }
            if (Key_Delay>KEY_SERIES_FLAG)
            {
                Key_Series = 1;
                Key_Delay  = 0;
                Key_Return = KEY_LONG | Key_Prev;   //返回长按后的值
                break;
            }
        }
     #endif

    default :
        break;
    }

}

#endif


byte Delaycnt;
void	FPPA0 (void)
{
	.ADJUST_IC	SYSCLK=IHRC/16 IHRC=16MHz, VDD=3V, Bandgap=OFF;

	//	Insert Initial Code
#ifdef SSV_SET_TX
	$ LED0    OUT,LOW;
	$ KEY0    IN,PULL;
	$ KEY1    IN,PULL;
	$ KEY2    IN,PULL;
#else
	$ MOTO_R    OUT,LOW;
	$ MOTO_L    OUT,LOW;
	$ MOTO_B    OUT,LOW;
	$ MOTO_F    OUT,LOW;
#endif
	$ ADD_SET IN,NoPull;
	$ SSV_CE  OUT,LOW;
//    $ T16M   SYSCLK,/64,BIT15;
    $ T16M   IHRC,/4,BIT14;       //0.82mS   16m/4=4M ---> 1/4M  0.25uS ---> 2^15*0.25=8192uS =0.82mS
	intrq = 0;
	inten.2 = 1;
	engint;
	SPI_Init();         //初始化 SPI接口
	SSV_CheckID();      //CHECK ID!
	SSV_CE = 0;
	SSV_Init();         //初始化SSV7241

	.delay 3700;       //最少延时5ms   7400/1/1000 = 7.4ms

#ifdef SSV_SET_RX
    SSV_FlushRX();
    SSV_ClearRX_DR();
#else
	Key_Return = 0;
	Key_Init();
	SSV_FlushTX();
	SSV_ClearTX_DS();
	SSV_ClearMAX_RT();
#endif

	SSV_CS = 0;
	A = STATUS|W_REG;                 //写地址;
	SPI_RW(); //写地址
	A = 0x70;	
	SPI_RW(); //写地址值
	SSV_CS = 1;

	SSV_SetAdd();
    SSV_SetAW();
	SSV_SetRTY();
	SSV_SetRF();
	SSV_PowerOn();      //SSV power on
	.delay 3700;       //最少延时5ms   7400/1/1000 = 7.4ms
#ifdef SSV_SET_RX
    SSV_RX_MODE();
#else
	SSV_TX_MODE();
    SSV_Feature_DYN_ACK();
	TX_BUFF[0]=0;
#endif
    .delay 37; 
	SSV_CE = 1;
	while (1)
	{
//		...
//		wdreset;

#ifdef SSV_SET_RX

	  //.delay 2500;
     SSV_RXPacket();   //循环接收测试
     if(SSV_RDKey == KEY_F)   {MOTO_F = 1; MOTO_B = 0; MOTO_L = 0; MOTO_R = 0; SSV_RDKey = 0; Delaycnt = 0;}
     if(SSV_RDKey == KEY_B)   {MOTO_F = 0; MOTO_B = 1; MOTO_L = 0; MOTO_R = 0; SSV_RDKey = 0; Delaycnt = 0;}
     if(SSV_RDKey == KEY_L)   {MOTO_F = 0; MOTO_B = 0; MOTO_L = 1; MOTO_R = 0; SSV_RDKey = 0; Delaycnt = 0;}
     if(SSV_RDKey == KEY_R)   {MOTO_F = 0; MOTO_B = 0; MOTO_L = 0; MOTO_R = 1; SSV_RDKey = 0; Delaycnt = 0;}

     if(SSV_RDKey == KEY_F_L) {MOTO_F = 1; MOTO_B = 0; MOTO_L = 1; MOTO_R = 0; SSV_RDKey = 0; Delaycnt = 0;}
     if(SSV_RDKey == KEY_B_L) {MOTO_F = 0; MOTO_B = 1; MOTO_L = 1; MOTO_R = 0; SSV_RDKey = 0; Delaycnt = 0;}
     if(SSV_RDKey == KEY_F_R) {MOTO_F = 1; MOTO_B = 0; MOTO_L = 0; MOTO_R = 1; SSV_RDKey = 0; Delaycnt = 0;}
     if(SSV_RDKey == KEY_B_R) {MOTO_F = 0; MOTO_B = 1; MOTO_L = 0; MOTO_R = 1; SSV_RDKey = 0; Delaycnt = 0;}
#else
     if(TX_BUFF[0])
	 {
       SSV_TXPacket();   //循环发送测试
       .delay 8000;
       TX_BUFF[0]= 0;
	 }
	 LED0 = 0;
	 if(Key_Return)
	 {
       TX_BUFF[0] |= Key_Return;
	 }
	 /*
	 if(Key_Return == KEY_F)    {TX_BUFF[0] = KEY_F;}
	 if(Key_Return == KEY_B)    {TX_BUFF[0] = KEY_B;}
	 if(Key_Return == KEY_L)    {TX_BUFF[0] = KEY_L;}
	 if(Key_Return == KEY_R)    {TX_BUFF[0] = KEY_R;}

	 if(Key_Return == KEY_F_L)  {TX_BUFF[0] = KEY_F_L;}
	 if(Key_Return == KEY_B_L)  {TX_BUFF[0] = KEY_B_L;}
	 if(Key_Return == KEY_F_R)  {TX_BUFF[0] = KEY_F_R;}
	 if(Key_Return == KEY_B_R)  {TX_BUFF[0] = KEY_B_R;}
	 */
	 if(TX_BUFF[0]) LED0 =1;
#endif


	}
}

void	Interrupt (void)
{
//	static byte cnt;
	pushaf;

	if (Intrq.T16)    //10ms
	{	//	T16 Trig
		//	User can add code
		Intrq.T16	=	0;
//		cnt++;
		Delaycnt++;
#ifdef SSV_SET_RX
  	    if(Delaycnt >= 20)
        {
          Delaycnt = 0;
		  MOTO_F = 0;
		  MOTO_B = 0; 
		  MOTO_L = 0;
		  MOTO_R = 0; 
        }
/*        if(cnt >= 50)
        {
	        cnt = 0;
			if(Kv_cnt)
			{
		  		Kv_cnt--;
		  		a = PB;
		  		a ^=0X01;
		  		PB = a;
        	}
        }
*/		
#else                        //TX
/*       if(cnt >= 250)
       {
         cnt = 0;
         TX_BUFF[0] ++;
         if(TX_BUFF[0]>0x0f)
		 TX_BUFF[0] = 0;
       }
*/	   
        Key_Scan();  //扫描按键10ms一次

#endif
		//...
	}

	popaf;
}


