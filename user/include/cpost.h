/**
 * @file cpost.h
 * @author Letter (nevermindzzt@gmail.com)
 * @brief c post
 * @version 1.0.0
 * @date 2020-10-31
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#ifndef __CPOST_H__
#define __CPOST_H__

#define     CPOST_VERSION               "1.0.3"

#include <stddef.h>
#include	"STC32G_Timer.h"

#define     CPOST_FLAG_CLEAR_FRONT      0           // post列表中，存在相同handler, 清除之前的post
#define     CPOST_FLAG_CANCEL_CURRENT   1           // post列表中，存在相同handler, 取消当前post
#define     CPOST_FLAG_ADD_NEW          2           // 添加新post，不论post列表中是否存在相同handler

#define     CPOST_PARAM_DIFF_DISALBE    0           // 判断是否相同的handler时，忽略参数
#define     CPOST_PARAM_DIFF_ENABLE     1           // 判断是否相同的handler时，包括参数的比较

/**
 * @brief 最大handler数量
 */
#define     CPOST_MAX_HANDLER_SIZE      16

/**
 * @brief 获取系统tick函数
 */
#define     CPOST_GET_TICK()            T0_Get_TIck()

/**
 * @brief tick最大值
 */
#define     CPOST_MAX_TICK              0xFFFF

struct Attrs
{
unsigned char flag : 2;         //结构体位段定义两个位存状态,flag表示当添加相同handler的时候应该如何处理，
unsigned char paramDiff: 1;     //paramdiff表示进行比较handler是否相等时考虑参数与否        
};

typedef struct
{
    size_t startTime;
    size_t delay;
    void (*handler)(void *);            //定义了一个返回值为void的函数指针，接受参数为一个任意类型的指针
    void *param;                        //定义了一个任意类型的指向函数参数的指针                      
} CpostHandler;

typedef struct
{
    void *handler;
    void *param;
    size_t delay;
    struct Attrs attrs;
} CpostParam;


/**
 * @brief cpsot
 * 
 * @param ... 参数列表
 * @note 参数按照`CpostParam`结构体进行传递
 *       按顺序传递时，直接按照`cpost(handler, param，delay)`传参
 *       不按顺序时，需要指定成员名，比如`cpost(handler, .delay=5000)`
 *       属性参数必须指定成员名，比如`cpost(handler, .attrs.flag=CPOST_FLAG_CANCEL_CURRENT)`
 *       在中断中按照个数抛出任务
 * @return signed char 0 成功 -1 失败
 */
#define cpost(handler,param,delay,flag,paramdiff) \
        cpostAddHandler(cpostparaminit(handler,param,delay,flag,paramdiff))   //宏定义传递可变参数
// #define cpost(arg...) 
//         cpostAddHandler(&((CpostParam){arg}))   //等价用arg代替
#define cpostDelay(_handler, _delay) \
        cpost(_handler, .delay=_delay)          //成员列表初始化不按顺序需要用.指定成员名

#define cpostEx(_handler, _param) \
        cpost(_handler, _param)                 //按顺序不需要指定成员

#define cpostDelayEx(_handler, _param, _delay) \
        cpost(_handler, _param, _delay)

CpostParam * cpostparaminit(void * handler,void * param,size_t delay,unsigned char flag,unsigned char paramDiff);

signed char cpostAddHandler(CpostParam *param);

void cpostProcess(void);

void cpostRemove(void *handler, void *param);

void cpostRemoveAll(void);

signed char cpostIsInList(void *handler, void *param);
#endif
