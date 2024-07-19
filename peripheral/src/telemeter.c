#include "telemeter.h"

//#pragma userclass (near=CEVENT)	
//	CEVENT_EXPORT(0,dl1b_init,NULL);
//#pragma userclass (near=default)
/*********************************************************************************************************************
* 文件名称          zf_device_dl1b
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MM32F527X_E9P
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* 接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   SCL                 查看 zf_device_dl1b.h 中 DL1B_SCL_PIN  宏定义
*                   SDA                 查看 zf_device_dl1b.h 中 DL1B_SDA_PIN  宏定义
*                   XS                  查看 zf_device_dl1b.h 中 DL1B_XS_PIN  宏定义
*                   VCC                 5V 电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/


uint8 dl1b_init_flag = 0;
uint8 dl1b_finsh_flag = 0;			//用来标志测距模块是否成功获得距离数据

#define dl1b_transfer_8bit_array(tdata, tlen, rdata, rlen)      (dl1b_iic_transfer_8bit_array((tdata), (tlen), (rdata), (rlen)))


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口传输 8bit 数组 先写后读取
// 参数说明     *write_data     发送数据存放缓冲区
// 参数说明     write_len       发送缓冲区长度
// 参数说明     *read_data      读取数据存放缓冲区
// 参数说明     read_len        读取缓冲区长度
// 返回参数     void            
// 使用示例     iic_transfer_8bit_array(IIC_1, addr, data, 64, data, 64);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void dl1b_iic_transfer_8bit_array (const uint8 *write_data, uint8 write_len, uint8 *read_data, uint8 read_len)
{
	iic_transfer(DL1B_DEV_ADDR,write_data,write_len,read_data,read_len);
}




//-------------------------------------------------------------------------------------------------------------------
// 函数简介     返回以毫米为单位的范围读数
// 参数说明     void
// 返回参数     void
// 使用示例     dl1b_get_distance();
// 备注信息     在开始单次射程测量后也调用此函数
//-------------------------------------------------------------------------------------------------------------------
uint16 dl1b_get_distance (void)
{     
	uint8 Data_Buffer[3];
	int16 dl1b_distance_temp = 0;
	uint16 dl1b_distance_mm = 3333;
    if(dl1b_init_flag)
    {
        Data_Buffer[0] = DL1B_GPIO__TIO_HV_STATUS >> 8;
        Data_Buffer[1] = DL1B_GPIO__TIO_HV_STATUS & 0xFF;
        dl1b_transfer_8bit_array(Data_Buffer, 2, &Data_Buffer[2], 1);

        if(Data_Buffer[2])
        {

            Data_Buffer[0] = DL1B_SYSTEM__INTERRUPT_CLEAR >> 8;
            Data_Buffer[1] = DL1B_SYSTEM__INTERRUPT_CLEAR & 0xFF;
            Data_Buffer[2] = 0x01;
            dl1b_transfer_8bit_array(Data_Buffer, 3, Data_Buffer, 0);// clear Interrupt

            Data_Buffer[0] = DL1B_RESULT__RANGE_STATUS >> 8;
            Data_Buffer[1] = DL1B_RESULT__RANGE_STATUS & 0xFF;
            dl1b_transfer_8bit_array(Data_Buffer, 2, &Data_Buffer[2], 1);
            
            if(0x89 == Data_Buffer[2])
            {
                Data_Buffer[0] = DL1B_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0 >> 8;
                Data_Buffer[1] = DL1B_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0 & 0xFF;
                dl1b_transfer_8bit_array(Data_Buffer, 2, Data_Buffer, 2);
                dl1b_distance_temp = Data_Buffer[0];
                dl1b_distance_temp = (dl1b_distance_temp << 8) | Data_Buffer[1];
                
                if(dl1b_distance_temp > 4000 || dl1b_distance_temp < 0)
                {
                    dl1b_distance_mm = 4444;
                    dl1b_finsh_flag = 0;
                }
                else
                {
                    dl1b_distance_mm = dl1b_distance_temp;
                    dl1b_finsh_flag = 1;
                }
            }
            else
            {
                dl1b_distance_mm = 5555;
                dl1b_finsh_flag = 0;
            }
        }
        else
        {
            dl1b_distance_mm = 6666;
            dl1b_finsh_flag = 0;
			dl1b_init();
        }
    }
	return dl1b_distance_mm;
}



//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 DL1B
// 参数说明     void
// 返回参数     uint8           1-初始化失败 0-初始化成功
// 使用示例     dl1b_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 dl1b_init (void)
{
    uint8   return_state    = 0;
    uint8   Data_Buffer[2 + sizeof(dl1b_default_configuration)]; 
    uint16  time_out_count  = 0;
	do
    {
        delay_ms(50);
        DL1B_XS_PIN = 0;
        delay_ms(10);
        DL1B_XS_PIN = 1;
        delay_ms(50);

        Data_Buffer[0] = DL1B_FIRMWARE__SYSTEM_STATUS >> 8;
        Data_Buffer[1] = DL1B_FIRMWARE__SYSTEM_STATUS & 0xFF;
        dl1b_transfer_8bit_array(Data_Buffer, 2, &Data_Buffer[2], 1);
        return_state = (0x01 == (Data_Buffer[2] & 0x01)) ? (0) : (1);
        if(1 == return_state)
        {
            break;
        }

        Data_Buffer[0] = DL1B_I2C_SLAVE__DEVICE_ADDRESS >> 8;
        Data_Buffer[1] = DL1B_I2C_SLAVE__DEVICE_ADDRESS & 0xFF;
        memcpy(&Data_Buffer[2], (uint8 *)dl1b_default_configuration, sizeof(dl1b_default_configuration));
        dl1b_transfer_8bit_array(Data_Buffer, 2 + sizeof(dl1b_default_configuration), Data_Buffer, 0);

        while(1)
        {
            Data_Buffer[0] = DL1B_GPIO__TIO_HV_STATUS >> 8;
            Data_Buffer[1] = DL1B_GPIO__TIO_HV_STATUS & 0xFF;
            dl1b_transfer_8bit_array(Data_Buffer, 2, &Data_Buffer[2], 1);
            if(0x00 == (Data_Buffer[2] & 0x01))
            {
                time_out_count = 0;
                break;
            }
            if(DL1B_TIMEOUT_COUNT < time_out_count ++)
            {
                return_state = 1;
                break;
            }
            delay_ms(1);
        }

        dl1b_init_flag = 1;
    }while(0);
	
    return return_state;
}
