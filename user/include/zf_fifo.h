/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		fifo
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/



#ifndef __ZF_FIFO_H
#define __ZF_FIFO_H


typedef struct
{
    unsigned char *buffer;                                                          // 缓存指针
    unsigned long    head;                                                             // 缓存头指针 总是指向空的缓存
    unsigned long    end;                                                              // 缓存尾指针 总是指向非空缓存（缓存全空除外）
    unsigned long    size;                                                             // 缓存剩余大小
    unsigned long    max;                                                              // 缓存总大小
}fifo_struct;

typedef enum
{
    FIFO_SUCCESS,

    FIFO_BUFFER_NULL,
    FIFO_SPACE_NO_ENOUGH,
    FIFO_DATA_NO_ENOUGH,
}fifo_state_enum;

typedef enum
{
    FIFO_READ_AND_CLEAN,
    FIFO_READ_ONLY,
}fifo_operation_enum;

fifo_state_enum fifo_init           (fifo_struct *fifo, unsigned char *buffer_addr, unsigned long size);
void            fifo_head_offset    (fifo_struct *fifo, unsigned long offset);
void            fifo_end_offset     (fifo_struct *fifo, unsigned long offset);
void            fifo_clear          (fifo_struct *fifo);
unsigned long          fifo_used           (fifo_struct *fifo);

fifo_state_enum fifo_read_buffer    (fifo_struct *fifo, unsigned char *dat, unsigned long *length, fifo_operation_enum flag);
fifo_state_enum fifo_write_buffer   (fifo_struct *fifo, unsigned char *dat, unsigned long length);

#endif


