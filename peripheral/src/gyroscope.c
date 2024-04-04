#include "gyroscope.h"
#include "timer.h"

#define dt 0.01

int16 gyro_gyro_x,gyro_gyro_y,gyro_gyro_z;
int16 gyro_acc_x,gyro_acc_y,gyro_acc_z;


float fil_Acc_x,fil_Acc_y;		//定义处理后的加速度值
float fil_Gyro_z;				//定义处理后的角速度值
float Gyro_z=0;					//定义的处理角速度的中间变量
float Angle_Z=90;				//设置初始角度为90度
float Gyroscope_FIFO[11];
int gyro_i=0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ICM20602 自检
// 参数说明     void
// 返回参数     uint8           1-自检失败 0-自检成功
// 使用示例     icm20602_self_check();
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8 icm20602_self_check (void)
{
    uint8 dat = 0, return_state = 0;
    uint16 timeout_count = 0;

    while(0x12 != dat)                                                          // 判断 ID 是否正确
    {
        if(timeout_count ++ > ICM20602_TIMEOUT_COUNT)
        {
            return_state =  1;
            break;
        }
        dat = iic_read_reg(ICM20602_DEV_ADDR, ICM20602_WHO_AM_I);
        delay_ms(10);
    }
    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 ICM20602
// 参数说明     void
// 返回参数     uint8           1-初始化失败 0-初始化成功
// 使用示例     icm20602_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 icm20602_init (void)
{
    uint8 val = 0x0, return_state = 0;
    uint16 timeout_count = 0;

    delay_ms(10);                                                        // 上电延时

//#if ICM20602_USE_SOFT_IIC
//    soft_iic_init(&icm20602_iic_struct, ICM20602_DEV_ADDR, ICM20602_SOFT_IIC_DELAY, ICM20602_SCL_PIN, ICM20602_SDA_PIN);
//#else
//    spi_init(ICM20602_SPI, SPI_MODE0, ICM20602_SPI_SPEED, ICM20602_SPC_PIN, ICM20602_SDI_PIN, ICM20602_SDO_PIN, SPI_CS_NULL);
//    gpio_init(ICM20602_CS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
//#endif

    do
    {
        if(icm20602_self_check())
        {
            // 如果程序在输出了断言信息 并且提示出错位置在这里
            // 那么就是 ICM20602 自检出错并超时退出了
            // 检查一下接线有没有问题 如果没问题可能就是坏了
            
			while(1)
			{
				printf("icm20602 self check error");
				delay_ms(200);
			}
            return_state = 1;
            break;
        }

        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_1, 0x80);                     // 复位设备
        delay_ms(2);

        do
        {                                                                       // 等待复位成功
            val = iic_read_reg(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_1);
            if(timeout_count ++ > ICM20602_TIMEOUT_COUNT)
            {
                // 如果程序在输出了断言信息 并且提示出错位置在这里
                // 那么就是 ICM20602 自检出错并超时退出了
                // 检查一下接线有没有问题 如果没问题可能就是坏了
				while(1)
				{
					printf("icm20602 reset error.\r\n");
					delay_ms(200);
				}
                return_state = 1;
                break;
            }
        }while(0x41 != val);
        if(1 == return_state)
        {
            break;
        }

        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_1,0x01);                 // 时钟设置
        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_2,0x00);                 // 开启陀螺仪和加速度计
        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_CONFIG,0x01);                 // 176HZ 1KHZ
        iic_write_reg(ICM20602_DEV_ADDR, ICM20602_SMPLRT_DIV,0x07);                 // 采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
        
		iic_write_reg(ICM20602_DEV_ADDR, ICM20602_GYRO_CONFIG,ICM20602_GYR_SAMPLE);  // ±2000 dps
		// ICM20602_GYRO_CONFIG寄存器
        // 设置为:0x00 陀螺仪量程为:±250 dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x08 陀螺仪量程为:±500 dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x10 陀螺仪量程为:±1000dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
        
		iic_write_reg(ICM20602_DEV_ADDR, ICM20602_ACCEL_CONFIG,ICM20602_ACC_SAMPLE);  // ±8g
		// ICM20602_ACCEL_CONFIG寄存器
        // 设置为:0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
        // 设置为:0x08 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
        // 设置为:0x10 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
        // 设置为:0x18 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
       
		iic_write_reg(ICM20602_DEV_ADDR, ICM20602_ACCEL_CONFIG_2, 0x03);                 // Average 4 samples   44.8HZ   //0x23 Average 16 samples


    }while(0);
    return return_state;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Get_gyro_accdata(void)
{
    uint8 dat[6];
    iic_read_regs(ICM20602_DEV_ADDR, ICM20602_ACCEL_XOUT_H, dat, 6);  
    gyro_acc_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    gyro_acc_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    gyro_acc_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Get_gyro_gyro(void)
{
    uint8 dat[6];
    iic_read_regs(ICM20602_DEV_ADDR, ICM20602_GYRO_XOUT_H, dat, 6);  
    gyro_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    gyro_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    gyro_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}

/**************************************************************************
函数功能：递推平均滤波算法 处理角速度
入口参数：无
返回  值：无
**************************************************************************/
void Gyroscope_newValues()
{
	int16 sum=0;
	static int16 gyro[50],sum_gyro;
	static int gyro_flag=0,Gyro_flag;

	Get_gyro_gyro();		// 获取角速度数据
	if(gyro_flag==0)
	{		 
		gyro[gyro_i]=gyro_gyro_z;
		fil_Gyro_z=0.0;
		gyro_i++;
		if(gyro_i==99)
		{
			for(gyro_i=0;gyro_i<100;gyro_i++)
			{
				sum_gyro+=gyro[gyro_i];
			}
			gyro_flag=1;
		}
	} 
	if(gyro_flag==1)
	{
		Gyro_z = (float)(gyro_gyro_z-sum_gyro/100)/16.3835;
		if(abs(Gyro_z)<3)//角速度小于3时  默认为小车静止  
		{
			Gyro_z=0;
		}
		for(Gyro_flag=1;Gyro_flag<10;Gyro_flag++)
		{	
			Gyroscope_FIFO[Gyro_flag-1]=Gyroscope_FIFO[Gyro_flag];//FIFO 操作
		}
		Gyroscope_FIFO[9]=Gyro_z;
		for(Gyro_flag=0;Gyro_flag<10;Gyro_flag++)
		{	            
			sum+=Gyroscope_FIFO[Gyro_flag];//求当前数组的合，再取平均值
		}
		fil_Gyro_z=sum/10;
	}
}		

/**************************************************************************
函数功能：对角速度积分 得到角度
入口参数：无
返回  值：无
**************************************************************************/
void Get_angle()
{
	Gyroscope_newValues();			//使用陀螺仪获取角速度
	Angle_Z-=fil_Gyro_z*dt;
	if(Angle_Z>=360) Angle_Z=Angle_Z-360;
	if(Angle_Z<=-360) Angle_Z=Angle_Z+360;
}


