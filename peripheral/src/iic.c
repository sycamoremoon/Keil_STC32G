#include  "iic.h"

#if	(USE_SOFT_IIC == 1)

static uint16 simiic_delay_time=100;   //ICM等传感器应设置为100

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC延时 时间设置
//  @return     void						
//  @since      v1.0
//  Sample usage:				如果IIC通讯失败可以尝试增加simiic_delay_time的值
//-------------------------------------------------------------------------------------------------------------------
void simiic_delay_set(uint16 time)
{
    simiic_delay_time = time;           //ICM等传感器应设置为100
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC延时
//  @return     void						
//  @since      v1.0
//  Sample usage:				如果IIC通讯失败可以尝试增加simiic_delay_time的值
//-------------------------------------------------------------------------------------------------------------------
void simiic_delay(void)
{
	uint16 delay_time;
    delay_time = simiic_delay_time;
	while(delay_time--);
}


//内部使用，用户无需调用
void simiic_start(void)
{
	SDA1();
	SCL1();
	simiic_delay();
	SDA0();
	simiic_delay();
	SCL0();
}

//内部使用，用户无需调用
void simiic_stop(void)
{
	SDA0();
	SCL0();
	simiic_delay();
	SCL1();
	simiic_delay();
	SDA1();
	simiic_delay();
}

//主应答(包含ack:SDA=0和no_ack:SDA=0)
//内部使用，用户无需调用
void simiic_sendack(unsigned char ack_dat)
{
    SCL0();
	simiic_delay();
	if(ack_dat) SDA0();
    else    	SDA1();

    SCL1();
    simiic_delay();
    SCL0();
    simiic_delay();
}


static int sccb_waitack(void)
{
    SCL0();
	simiic_delay();
	
	SCL1();
    simiic_delay();
	
    if(SDA_GPIO_PIN)           //应答为高电平，异常，通信失败
    {
        SCL0();
        return 0;
    }
    SCL0();
	simiic_delay();
    return 1;
}

//字节发送程序
//发送c(可以是数据也可是地址)，送完后接收从应答
//不考虑从应答位
//内部使用，用户无需调用
void send_ch(uint8 c)
{
	uint8 i = 8;
    while(i--)
    {
        if(c & 0x80)	SDA1();//SDA 输出数据
        else			SDA0();
        c <<= 1;
        simiic_delay();
        SCL1();                //SCL 拉高，采集信号
        simiic_delay();
        SCL0();                //SCL 时钟线拉低
    }
	sccb_waitack();
}

//字节接收程序
//接收器件传来的数据，此程序应配合|主应答函数|使用
//内部使用，用户无需调用
uint8 read_ch(uint8 ack_x)
{
    uint8 i;
    uint8 c;
    c=0;
    SCL0();
    simiic_delay();
    SDA1();             
    for(i=0;i<8;i++)
    {
        simiic_delay();
        SCL0();         //置时钟线为低，准备接收数据位
        simiic_delay();
        SCL1();         //置时钟线为高，使数据线上数据有效
        simiic_delay();
        c<<=1;
        if(SDA_GPIO_PIN) 
        {
            c+=1;   //读数据位，将接收的数据存c
        }
    }
	SCL0();
	simiic_delay();
	simiic_sendack(ack_x);
	
    return c;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC写数据到设备寄存器函数
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg_add			存放目标寄存器地址的数组
//	@param		reg_num			寄存器数据字节数
//  @param      dat				写入的数据
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void simiic_write_reg(uint8 dev_add, uint8 reg_add, uint8 dat)
{
	simiic_start();
    send_ch( (dev_add<<1) | 0x00);   //发送器件地址加写位
	send_ch(reg_add);
	send_ch( dat );   				 //发送需要写入的数据
	simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC写数据到设备寄存器函数
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg_add			存放目标寄存器地址的数组
//	@param		reg_num			寄存器数据字节数
//  @param      dat_add			写入的数据数组的地址
//	@param		dat_num			写入的数据字节个数
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void simiic_write_regs(uint8 dev_add, uint8 reg, uint8 * dat_add, uint8 dat_num)
{
	simiic_start();
    send_ch((dev_add<<1) | 0x00);   //发送器件地址加写位
	send_ch(reg);
	while(dat_num--)
    {
		send_ch(*dat_add);   				 //发送需要写入的数据
		dat_add++;
    }
	simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC从设备寄存器读取数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				存放目标寄存器地址的数组
//	@param		reg_num			寄存器数据字节数
//  @return     uint8			返回寄存器的数据			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint8 simiic_read_reg(uint8 dev_add, uint8 reg_add)
{
	uint8 dat;
	simiic_start();
    send_ch( (dev_add<<1) | 0x00);  //发送器件地址加写位
	send_ch(reg_add);
	
	simiic_start();
	send_ch( (dev_add<<1) | 0x01);  //发送器件地址加读位
	dat = read_ch(no_ack);   				//读取数据
	simiic_stop();
	
	return dat;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC读取多字节数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				存放目标寄存器地址的数组
//	@param		reg_num			寄存器数据字节数
//  @param      dat_add			数据保存的地址指针
//  @param      dat_num			读取字节数量
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 dat_num)
{
	simiic_start();
    send_ch( (dev_add<<1) | 0x00);  //发送器件地址加写位
	send_ch(reg);

	simiic_start();
	send_ch( (dev_add<<1) | 0x01);  //发送器件地址加读位
    while(--dat_num)
    {
        *dat_add = read_ch(ack); //读取数据
        dat_add++;
    }
    *dat_add = read_ch(no_ack); //读取数据
	simiic_stop();
}
#define DL1B_DEV_ADDR                                           ( 0x52 >> 1 )
//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC读写传输数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      write_add			发送数据存放缓冲区
//	@param		write_num			发送缓冲区长度
//  @param      read_add			读取数据存放缓冲区
//  @param      read_num			读取缓冲区长度
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void simiic_transfer(const uint8 dev_add, const uint8 *write_add, uint8 write_num, uint8 *read_add, uint8 read_num)
{
	simiic_start();
    send_ch(dev_add << 1);
    while(write_num --)
    {
        send_ch(*write_add ++);
    }
    simiic_start();
    send_ch(dev_add << 1 | 0x01);
    while(read_num --)
    {
		// 前面7位需要回复ack，最后1位不需要回复ack.
        *read_add ++ = read_ch(read_num != 0);
    }
    simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      模拟IIC端口初始化
//  @param      NULL
//  @return     void	
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void simiic_init(void)
{
	//输出模式配置为开漏输出
	GPIO_InitTypeDef IIC_PIN;
	IIC_PIN.Mode = GPIO_OUT_OD;		
	IIC_PIN.Pin = IIC_SCL_PIN;
	GPIO_Inilize(IIC_GPIO,&IIC_PIN);
	
	IIC_PIN.Pin = IIC_SDA_PIN;
	GPIO_Inilize(IIC_GPIO,&IIC_PIN);	
	
	//开漏输出设置上拉电阻
	switch (IIC_GPIO){
		case GPIO_P1 :P1_PULL_UP_ENABLE(IIC_SCL_PIN | IIC_SDA_PIN);
			break;
		case GPIO_P2 :P2_PULL_UP_ENABLE(IIC_SCL_PIN | IIC_SDA_PIN);
			break;
		case GPIO_P3 :P3_PULL_UP_ENABLE(IIC_SCL_PIN | IIC_SDA_PIN);
			break;
		default : 
			break;
	}
}

#else  //Use Hardware IIC

/*Hardware IIC codes write here*/

//========================================================================
// 函数: void I2C_Init(I2C_InitTypeDef *I2Cx)
// 描述: I2C初始化程序.
// 参数: I2Cx: 结构参数,请参考I2C.h里的定义.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================

void hardiic_init()
{
	//输出模式配置为开漏输出
	GPIO_InitTypeDef IIC_PIN;
	IIC_PIN.Mode = GPIO_OUT_OD;		
	IIC_PIN.Pin = IIC_SCL_PIN;
	GPIO_Inilize(IIC_GPIO,&IIC_PIN);
	
	IIC_PIN.Pin = IIC_SDA_PIN;
	GPIO_Inilize(IIC_GPIO,&IIC_PIN);	
	
	//开漏输出设置上拉电阻
	switch (IIC_GPIO){
		case GPIO_P1 :P1_PULL_UP_ENABLE(IIC_SCL_PIN | IIC_SDA_PIN);
			break;
		case GPIO_P2 :P2_PULL_UP_ENABLE(IIC_SCL_PIN | IIC_SDA_PIN);
			break;
		case GPIO_P3 :P3_PULL_UP_ENABLE(IIC_SCL_PIN | IIC_SDA_PIN);
			break;
		default : 
			break;
	}
	Zf_IIC_init(IIC_2, IIC2_SCL_P25,IIC2_SDA_P24,500);
}

#endif


/* ============================== 可调用函数 ============================ */

//-------------------------------------------------------------------------------------------------------------------
//  @brief      1. IIC写数据到设备寄存器函数
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg_add			存放 目标寄存器地址 的数组
//	@param		reg_num			寄存器数据字节数
//  @param      dat				写入的数据
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void iic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
#if USE_SOFT_IIC
	simiic_write_reg(dev_add, reg, dat);				/* 设备地址(低七位地址)，存放目标寄存器地址的数组，寄存器数据字节数，写入的数据 */
#else	//use hardware IIC
	Zf_IIC_write_reg(dev_add, reg, dat);
#endif	//USE_SOFT_IIC
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      IIC写多字节数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg_add			存放目标寄存器地址的数组
//	@param		reg_num			寄存器数据字节数
//  @param      dat_add			要写入的数据保存的地址指针
//  @param      dat_num			读取字节数量
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void iic_write_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 dat_num)
{
#if USE_SOFT_IIC
	simiic_write_regs(dev_add, reg, dat_add, dat_num);
#else	//use hardware IIC
	//I2C_WriteNbyte(dev_add, reg_add, reg_num, dat_add, dat_num);
#endif	//USE_SOFT_IIC
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      IIC读写传输数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      write_add			发送数据存放缓冲区
//	@param		write_num			发送缓冲区长度
//  @param      read_add			读取数据存放缓冲区
//  @param      read_num			读取缓冲区长度
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void iic_transfer(const uint8 dev_add, const uint8 *write_add, uint8 write_num, uint8 *read_add, uint8 read_num)
{
#if USE_SOFT_IIC
	simiic_transfer(dev_add, write_add, write_num, read_add, read_num);
#else	//use hardware IIC

#endif	//USE_SOFT_IIC
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      IIC读数据到设备寄存器函数
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg_add			存放目标寄存器地址的数组
//	@param		reg_num			寄存器数据字节数
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint8 iic_read_reg(uint8 dev_add, uint8 reg)
{
	uint8 recive = 0;
#if USE_SOFT_IIC
	recive = simiic_read_reg(dev_add, reg);
#else	//use hardware IIC
	Zf_IIC_read_reg(dev_add, reg, &recive);
#endif	//USE_SOFT_IIC
	return recive;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      IIC读取多字节数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg_add			存放目标寄存器地址的数组
//	@param		reg_num			寄存器数据字节数
//  @param      dat_add			数据保存的地址指针
//  @param      dat_num			读取字节数量
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void iic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 dat_num)
{
#if USE_SOFT_IIC
	simiic_read_regs(dev_add, reg, dat_add, dat_num);
#else	//use hardware IIC
	Zf_IIC_read_reg_bytes(dev_add, reg, dat_add, dat_num);
#endif	//USE_SOFT_IIC
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      4. IIC端口初始化
//  @param      NULL
//  @return     void	
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void iic_init(void)
{
#if USE_SOFT_IIC
	simiic_init();
#else	//use hardware IIC
	hardiic_init();
#endif	//USE_SOFT_IIC
}
