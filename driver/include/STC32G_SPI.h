/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#ifndef	__STC32G_SPI_H
#define	__STC32G_SPI_H

#include	"config.h"

//========================================================================
//                               SPI����
//========================================================================

#define		SPI_SSIG_Set(n)					SSIG = n		/* SS���Ź��ܿ��� */
#define		SPI_Start(n)						SPEN = n		/* SPIʹ�ܿ���λ */
#define		SPI_FirstBit_Set(n)			DORD = n		/* ���ݷ���/����˳�� MSB/LSB */
#define		SPI_Mode_Set(n)					MSTR = n		/* SPI����ģʽ���� */
#define		SPI_CPOL_Set(n)					CPOL = n		/* SPIʱ�Ӽ��Կ��� */
#define		SPI_CPHA_Set(n)					CPHA = n		/* SPIʱ����λ���� */
#define		SPI_Clock_Select(n)			SPCTL = (SPCTL & ~0x03) | (n)					/* SPIʱ��Ƶ��ѡ�� */

#define		SPI_ClearFlag()					{SPIF = 1; WCOL = 1;}		/* д 1 ��� SPIF��WCOL ��־ */

#define		HSSPI_Enable(n)					HSSPI_CFG2 |= 0x20       //ʹ��SPI����ģʽ
#define		HSSPI_Disable(n)				HSSPI_CFG2 &= ~0x20      //�ر�SPI����ģʽ

//========================================================================
//                              ��������
//========================================================================

#define	SPI_BUF_LENTH	128
#define	SPI_BUF_type	edata

sbit  SPI_SS    = P1^2;
sbit  SPI_MOSI  = P1^3;
sbit  SPI_MISO  = P1^4;
sbit  SPI_SCLK  = P1^5;

sbit  SPI_SS_2    = P2^2;
sbit  SPI_MOSI_2  = P2^3;
sbit  SPI_MISO_2  = P2^4;
sbit  SPI_SCLK_2  = P2^5;

sbit  SPI_SS_3    = P7^4;
sbit  SPI_MOSI_3  = P7^5;
sbit  SPI_MISO_3  = P7^6;
sbit  SPI_SCLK_3  = P7^7;

sbit  SPI_SS_4    = P3^5;
sbit  SPI_MOSI_4  = P3^4;
sbit  SPI_MISO_4  = P3^3;
sbit  SPI_SCLK_4  = P3^2;

#define	SPI_Mode_Master     1
#define	SPI_Mode_Slave      0
#define	SPI_CPOL_High       1
#define	SPI_CPOL_Low        0
#define	SPI_CPHA_1Edge      0
#define	SPI_CPHA_2Edge      1
#define	SPI_Speed_4         0
#define	SPI_Speed_8         1
#define	SPI_Speed_16        2
#define	SPI_Speed_2         3
#define	SPI_MSB             0
#define	SPI_LSB             1

typedef struct
{
	u8	SPI_Enable;     //SPI����, ENABLE,DISABLE
	u8	SPI_SSIG;       //Ƭѡλ, ENABLE(SSȷ�������ӻ�), DISABLE(����SS���Ź���)
	u8	SPI_FirstBit;   //SPI_MSB, SPI_LSB
	u8	SPI_Mode;       //SPI_Mode_Master, SPI_Mode_Slave
	u8	SPI_CPOL;       //SPI_CPOL_High,   SPI_CPOL_Low
	u8	SPI_CPHA;       //SPI_CPHA_1Edge,  SPI_CPHA_2Edge
	u8	SPI_Speed;      //SPI_Speed_4, SPI_Speed_8, SPI_Speed_16, SPI_Speed_2
} SPI_InitTypeDef;


extern	bit B_SPI_Busy; //����æ��־
extern	u8 	SPI_RxCnt;
extern	u8 	SPI_RxTimerOut;
extern	u8 	SPI_BUF_type SPI_RxBuffer[SPI_BUF_LENTH];


void	SPI_Init(SPI_InitTypeDef *SPIx);
void	SPI_SetMode(u8 mode);
void	SPI_WriteByte(u8 dat);
u8 SPI_ReadByte(void);

#endif

