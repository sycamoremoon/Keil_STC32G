#include  "iic.h"
//定义是否使用软件IIC
#define 	USE_SOFT_IIC		(1)

#if	(USE_SOFT_IIC == 1)

//软件 IIC 驱动引脚定义，需要在iic.c初始化中对引脚配置
#define		SCL_GPIO_PIN	P15
#define 	SDA_GPIO_PIN	P14
#define 	IIC_GPIO					GPIO_P1
#define 	IIC_SCL_PIN   				GPIO_Pin_5				// 软件 IIC SCL 引脚
#define 	IIC_SDA_PIN  				GPIO_Pin_4				// 软件 IIC SDA 引脚

#define 	SDA0()       SDA_GPIO_PIN=0	//IO口输出低电平
#define 	SDA1()       SDA_GPIO_PIN=1	//IO口输出高电平  
#define 	SCL0()       SCL_GPIO_PIN=0	//IO口输出低电平
#define 	SCL1()       SCL_GPIO_PIN=1	//IO口输出高电平
#define 	ack 			1      	//主应答
#define 	no_ack 			0   	//从应答
	
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
//  @param      reg				存放目标寄存器地址的数组
//	@param		reg_num			寄存器数据字节数
//  @param      dat				写入的数据
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void simiic_write_reg(uint8 dev_add, uint8 * reg_add, uint8 reg_num, uint8 dat)
{
	simiic_start();
    send_ch( (dev_add<<1) | 0x00);   //发送器件地址加写位
    while(reg_num--)
    {
		send_ch(*reg_add);
		reg_add++;
    }
	send_ch( dat );   				 //发送需要写入的数据
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
uint8 simiic_read_reg(uint8 dev_add, uint8 * reg_add, uint8 reg_num)
{
	uint8 dat;
	simiic_start();
    send_ch( (dev_add<<1) | 0x00);  //发送器件地址加写位
    while(reg_num--)
    {
		send_ch(*reg_add);
		reg_add++;
    }
	
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
void simiic_read_regs(uint8 dev_add, uint8 * reg_add, uint8 reg_num, uint8 *dat_add, uint8 dat_num)
{
	simiic_start();
    send_ch( (dev_add<<1) | 0x00);  //发送器件地址加写位
    while(reg_num--)
    {
		send_ch(*reg_add);
		reg_add++;
    }
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

#else    //Use Hardware IIC

/*Hardware IIC code write here*/

#endif   //USE_SOFT_IIC


void iic_write_reg(uint8 dev_add, uint8 * reg_add, uint8 reg_num, uint8 dat)
{
#ifdef USE_SOFT_IIC
	simiic_write_reg(dev_add,reg_add,reg_num,dat);
#else	//use hardware IIC

#endif	//USE_SOFT_IIC
}

uint8 iic_read_reg(uint8 dev_add, uint8 * reg_add, uint8 reg_num)
{
#ifdef USE_SOFT_IIC
	simiic_read_reg(dev_add,reg_add,reg_num);
#else	//use hardware IIC

#endif	//USE_SOFT_IIC
}

void iic_read_regs(uint8 dev_add, uint8 * reg_add, uint8 reg_num, uint8 *dat_add, uint8 dat_num)
{
#ifdef USE_SOFT_IIC
	simiic_read_regs(dev_add,reg_add,reg_num,dat_add,dat_num);
#else	//use hardware IIC

#endif	//USE_SOFT_IIC
}

void iic_init(void)
{
#ifdef USE_SOFT_IIC
	simiic_init();
#else	//use hardware IIC

#endif	//USE_SOFT_IIC
}
