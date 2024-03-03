/**
 * @file cpost.c
 * @author Letter (nevermindzzt@gmail.com)
 * @brief c post
 * @version 1.0.0
 * @date 2020-10-31
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#include "cpost.h"


CpostHandler cposhHandlers[CPOST_MAX_HANDLER_SIZE] = {0};           //定义了一个总的句柄合集

/**
 * @brief cpost 添加handler
 * 
 * @param param 参数
 * 
 * @return signed char 0 添加成功 -1 添加失败
 */
signed char cpostAddHandler(CpostParam *param)
{
	size_t i;
    if (cpostIsInList(param->handler, param->attrs.paramDiff ? param->param : NULL) == 0)
    {
        switch (param->attrs.flag)
        {
        case CPOST_FLAG_CLEAR_FRONT:
            cpostRemove(param->handler, param->attrs.paramDiff ? param->param : NULL);
            break;

        case CPOST_FLAG_CANCEL_CURRENT:
            return 0;
            // break;

        case CPOST_FLAG_ADD_NEW:
            // do nothint
            break;

        default:
            break;
        }
    }

    for (i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler == NULL)
        {
            cposhHandlers[i].startTime = CPOST_GET_TICK();
            cposhHandlers[i].delay = param->delay;
            cposhHandlers[i].handler = (void (*)(void *))(param->handler);  //强制类型转化，返回值为一个指针，参数也是一个指针
            cposhHandlers[i].param = param->param;
            return 0;
        }
    }
    return -1;
}


/**
 * @brief 移除handler
 * 
 * @param handler handler
 * @param param 参数，传 `NULL` 表示不比较参数
 * 
 */
void cpostRemove(void *handler, void *param)
{
	size_t i;
    for ( i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler == handler
            && (param == NULL || param == cposhHandlers[i].param))
        {
            cposhHandlers[i].handler = NULL;
        }
    }
}

/**
 * @brief 移除所有handler
 * 
 */
void cpostRemoveAll(void)
{
	size_t i;
    for ( i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        cposhHandlers[i].handler = NULL;
    }
}


/**
 * @brief 判断handler是否已经在列表中
 * 
 * @param handler handler
 * @param param 参数，传 `NULL` 表示不比较参数
 * 
 * @return signed char 0 存在 -1 不存在
 */
signed char cpostIsInList(void *handler, void *param)
{
	size_t i;
    for ( i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler == handler
            && (param == NULL || param == cposhHandlers[i].param))
        {
            return 0;
        }
    }
    return -1;
}

/**
 * @brief cpost 处理
 * 
 */
void cpostProcess(void)
{
    size_t tick;
	size_t i;
    for ( i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler)
        {
            tick = CPOST_GET_TICK();
            if (cposhHandlers[i].delay == 0 || 
                (CPOST_MAX_TICK - cposhHandlers[i].startTime > cposhHandlers[i].delay
                    ? tick - cposhHandlers[i].startTime >= cposhHandlers[i].delay
                    : CPOST_MAX_TICK - cposhHandlers[i].startTime + tick >= cposhHandlers[i].delay))        //判断本次读取的系统时间是否和添加句柄的时间是否在一个周期里面
            {
                cposhHandlers[i].handler(cposhHandlers[i].param);
                cposhHandlers[i].handler = NULL;
            }
        }
    }
}


/*
*   @brief cpostparam 结构体生成
*	目前只支持用地址传递一个参数
*/
CpostParam * cpostparaminit(void * handler,void * param,size_t delay,unsigned char flag,unsigned char paramDiff)
{
	CpostParam cpostparam;                 //定义了一个参数结构体
    size_t i;
    for ( i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler == NULL)
        {
            cpostparam.handler = handler;
            cpostparam.param = param;
            cpostparam.delay = delay;
            cpostparam.attrs.flag = flag;
            cpostparam.attrs.paramDiff = paramDiff;
            return &cpostparam;
        }
    }
    return 0;
}