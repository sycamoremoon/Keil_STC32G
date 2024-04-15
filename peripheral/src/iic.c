#include  "iic.h"

//#pragma userclass (near=CEVENT)	
//	CEVENT_EXPORT(0,iic_init,NULL);
//#pragma userclass (near=default)
//	
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

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void iic_delay_us(uint16 x)	//33.1776Mhz
{
    uint8 i;
    while(x--)
    {
		i = 9;
		while (--i);
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
uint8 wait(void)
{
    uint16 count = 0;
    uint8 ret = IIC_SEND_OK;
    while (!(I2CMSST & 0x40))
    {
        iic_delay_us(1);
        if(count++ >= 30)//等待超过30us，则退出等待。
        {
            ret = IIC_SEND_FAIL;
            break;
        }
    }
    I2CMSST &= ~0x40;
    return ret;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
uint8 start(void)
{
    uint8 ret;
    I2CMSCR = 0x01;                             //发送start命令
    ret = wait();
    return ret;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
uint8 send_data(char dat)
{
    uint8 ret;
    I2CTXD = dat;                               //写数据到数据缓冲区
    I2CMSCR = 0x02;                             //发送SEND命令
    ret = wait();
    return ret;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
uint8 recv_ack(void)
{
    uint8 ret;
    I2CMSCR = 0x03;                             //发送读ACK命令
    ret = wait();
    return ret;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
char recv_data(void)							//接收数据
{
    I2CMSCR = 0x04;                             //发送RECV命令
    wait();
    return I2CRXD;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
uint8 send_ack(void)
{
	uint8 ret;
    I2CMSST = 0x00;                             //设置ACK信号
    I2CMSCR = 0x05;                             //发送ACK命令
    ret = wait();
    return ret;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
uint8 send_nak(void)
{
	uint8 ret;
    I2CMSST = 0x01;                             //设置NAK信号
    I2CMSCR = 0x05;                             //发送ACK命令
    ret = wait();
    return ret;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
uint8 stop(void)
{
    uint8 ret;
    I2CMSCR = 0x06;                             //发送stop命令
    ret = wait();
    return ret;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      写入一个字节数据到I2C设备指定寄存器地址
//  @param      iic_n       IIC模块(IIC_1,IIC_2,IIC_3,IIC_0)
//  @param      slaveid     从机地址(7位地址)
//  @param      reg         从机寄存器地址
//  @param      dat         需要发送的数据
//  @return                 返回的状态值 0：成功  1：失败
//  @since      v2.0
//  Sample usage:       	iic_write_reg(0x2D, 0x50,2);     //写入数据2到0x50地址，从机地址为0x2D
//-------------------------------------------------------------------------------------------------------------------
uint8 hardiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
    if(start() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(send_data((dev_add<<1) | 0x00) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(recv_ack() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(send_data(reg) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(recv_ack() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(send_data(dat) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(recv_ack() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(stop() != IIC_SEND_OK)
        return IIC_SEND_FAIL;


    return IIC_SEND_OK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取I2C设备指定地址寄存器的数据
//  @param      iic_n        I2C通道号及引脚
//  @param      dev_add     从机地址(7位地址)
//  @param      reg         从机寄存器地址
//  @param      dat         数据地址
//  @return                 读取的寄存器值
//  @since      v1.0
//  Sample usage:       	uint8 value = iic_read_reg(i2c0, 0x2D, 0x50);//读取0x50地址的数据，从机地址为0x2D
//-------------------------------------------------------------------------------------------------------------------
uint8 hardiic_read_reg(uint8 dev_add, uint8 reg, uint8 *dat)
{
	if(start() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
    if(send_data((dev_add<<1) | 0x00) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(recv_ack() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
    if(send_data(reg) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(recv_ack() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
	
   if(start() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
   
    if(send_data((dev_add<<1) | 0x01) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
    if(recv_ack() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	

    *dat = recv_data(); //读取数据

	
    if(send_nak() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
    if(stop() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
    return IIC_SEND_OK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取I2C设备指定地址寄存器的数据
//  @param      iic_n       I2C通道号及引脚
//  @param      dev_add     从机地址(7位地址)
//  @param      reg         从机寄存器地址
//  @param      dat         读取的数据存储的地址
//  @param      num         读取字节数
//  @return     void
//  @since      v1.0
//  Sample usage:       	uint8 value = i2c_read_reg(i2c0, 0x2D, 0x50, 10, buf);//读取0x50地址的数据，从机地址为0x2D开始的10个字节
//-------------------------------------------------------------------------------------------------------------------
uint8 hardiic_read_reg_bytes(uint8 dev_add, uint8 reg
						, uint8 *dat, uint8 num)
{

	if(start() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
    if(send_data((dev_add<<1) | 0x00) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(recv_ack() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
    if(send_data(reg) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
    if(recv_ack() != IIC_SEND_OK)
        return IIC_SEND_FAIL;

	if(start() != IIC_SEND_OK)
        return IIC_SEND_FAIL;
		
	if(send_data((dev_add<<1) | 0x01) != IIC_SEND_OK)
		return IIC_SEND_FAIL;
	if(recv_ack() != IIC_SEND_OK)
		return IIC_SEND_FAIL;

    while(--num)
    {
        *dat = recv_data(); //读取数据
		if(send_ack() != IIC_SEND_OK)
		{
			return IIC_SEND_FAIL;
		}
        dat++;
    }
	
	*dat = recv_data();
	
	if(send_nak() != IIC_SEND_OK)
		return IIC_SEND_FAIL;
	
	if(stop() != IIC_SEND_OK)
		return IIC_SEND_FAIL;
	
	return IIC_SEND_OK;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      交换I2C设备指定地址寄存器的数据
//  @param      dev_add     	 设备地址(7位地址)
//  @param      write_add        写入设备地址
//  @param      write_num        写入数据的个数
//  @param      read_add         读取字节地址
//  @param      read_num         读取字节数
//  @return     void
//  @since      v1.0
//-------------------------------------------------------------------------------------------------------------------
uint8 hardiic_transfer(const uint8 dev_add, const uint8 *write_add, uint8 write_num, uint8 *read_add, uint8 read_num)
{
	if(start() != IIC_SEND_OK)
		return IIC_SEND_FAIL;
	
	if(send_data((dev_add<<1) | 0x00) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
	if(recv_ack() != IIC_SEND_OK)
			return IIC_SEND_FAIL;
	
	while(write_num --)
    {
		if(send_data(*write_add++) != IIC_SEND_OK)
			return IIC_SEND_FAIL;
		if(recv_ack() != IIC_SEND_OK)
			return IIC_SEND_FAIL;
    }
	
	if(start() != IIC_SEND_OK)
		return IIC_SEND_FAIL;
	
	if(send_data((dev_add<<1) | 0x01) != IIC_SEND_OK)
        return IIC_SEND_FAIL;
	
	if(recv_ack() != IIC_SEND_OK)
		return IIC_SEND_FAIL;
	
    while(--read_num)
    {
        *read_add = recv_data(); //读取数据
		if(send_ack() != IIC_SEND_OK)
		{
			return IIC_SEND_FAIL;
		}
        read_add++;
    }
	
	*read_add = recv_data();
	
	if(send_nak() != IIC_SEND_OK)
		return IIC_SEND_FAIL;
	
	if(stop() != IIC_SEND_OK)
		return IIC_SEND_FAIL;
	
	return IIC_SEND_OK;
}


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
	
	// 配置IIC寄存器
	P_SW2 &= ~(0x03<<4);
    P_SW2 |= 1<<7;	//将EAXFR寄存器置1，这样才能使用特殊功能寄存器为扩展SFR，访问逻辑地址位于 XDATA 区域
    switch(IIC_GPIO)
    {
    case GPIO_P1:
        P_SW2 |= (0x00<<4);	//SCL:P1.5	SDA:P1.4
        break;
    case GPIO_P2:
        P_SW2 |= (0x01<<4);	//SCL:P2.5	SDA:P2.4
        break;
    case GPIO_P3:
        P_SW2 |= (0x02<<4);	//SCL:P7.7	SDA:P7.6
        break;
	default : 
		break;
    }
    I2CCFG |= 1<<6;		//主机模式
    I2CCFG |= 1<<7;		//使能IIC
    I2CCFG |= WAIT_TIME;//速度设置为等待wait_time*2+1个时钟
    I2CMSST = 0x00;		//主机状态寄存器
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
	hardiic_write_reg(dev_add, reg, dat);
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
	hardiic_transfer(dev_add, write_add, write_num, read_add, read_num);
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
	hardiic_read_reg(dev_add, reg, &recive);
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
	hardiic_read_reg_bytes(dev_add, reg, dat_add, dat_num);
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
