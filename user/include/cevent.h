/**
 * @file cevent.h
 * @author Letter (nevermindzzt@gmail.com)
 * @brief c event
 * @version 1.0.0
 * @date 2020-11-15
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#ifndef __CEVENT_H__
#define __CEVENT_H__

#define     CEVENT_VERSION          "1.0.1"

/**
 * @brief cevent 速度优化
 *        打开这个开关需要同时配置 `CEVENT_BUFFER_SIZE` 或者 `CEVENT_MALLOC`
 *        开启后，cevent 会使用一部分内存，将所有注册的监听进行重排序，建立索引表，
 *        这样每次触发事件时就不需要进行遍历，能节省调用时间
 */
#define     CEVENT_SPEED_OPTIMIZE   1

/**
 * @brief cevent buffer
 *        `CEVENT_SPEED_OPTIMIZE` 开启时生效，这块内存用作速度优化
 *        buffer 大小计算为 `(maxEvent + 1) * 2 + ceventNum`
 *        (定义的最大事件号 + 1) * 2 + 注册的监听数量
 *        需要保证定义的数量大于这个数
 *        如果 `CEVENT_BUFFER_SIZE` 设置为 `0`, 则表示使用 `CEVENT_MALLOC` 分配内存
 */
#define     CEVENT_BUFFER_SIZE      64

#if defined (__C251__) || (__C51__)
#define ABSOLUTE_ADDR       0x0800
#endif

/**
 * @brief 内存分配函数
 *        `CEVENT_SPEED_OPTIMIZE` 打开且 `CEVENT_BUFFER_SIZE` 配置为 `0` 时，
 *        需要配置这个宏
 */
#define     CEVENT_MALLOC(x)        

#ifndef SECTION
    #if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 6000000)
        #define SECTION(x)                  __attribute__((used, section(x)))
    #elif defined(__ICCARM__) || defined(__ICCRX__)
        #define SECTION(x)                  @ x
    #elif defined(__GNUC__)
        #define SECTION(x)                  __attribute__((section(x)))
    #else
        #define SECTION(x)
    #endif
#endif

/**
 * @brief 导出事件
 * 
 * @param _event 事件
 * @param _func 注册函数
 * @param _params参数,参数传递的时候要以地址的形式传递
 * @note cEventParam##_event##_func和cEvent##_event##_func是定义的两个变量，一个是存放指针的数组，一个是类型为Cevent的变量
 */
#if defined (__C251__) || (__C51__)
    #define CEVENT_EXPORT(_event, _func, ...) \
        void * xdata cEventParam##_event##_func[] = {(void *)_func,##__VA_ARGS__}; \
		CEvent edata cEvent##_event##_func = \
        { \
            cEventParam##_event##_func, \
            sizeof(cEventParam##_event##_func) / sizeof(void *), \
            _event, \
        }
#else 
    #define CEVENT_EXPORT(_event, _func, ...) \
        const void *cEventParam##_event##_func[] = {(void *)_func, ##__VA_ARGS__}; \
        const CEvent SECTION("cEvent") cEvent##_event##_func = \
        { \
            .param = cEventParam##_event##_func, \
            .paramNum = sizeof(cEventParam##_event##_func) / sizeof(void *), \
            .event = _event, \
        }
#endif
/**
 * @brief 导出事件(解决命名冲突)
 *        一般情况下不需要调用这个宏导出事件，当需要对同一个事件调用同一个函数时
 *        需要使用这个宏解决命名冲突
 * 
 * @param _alias 命名
 * @param _event 事件
 * @param _func 注册函数
 * @param ... 参数
 */
#if defined (__C251__) || (__C51__)
    #define CEVENT_EXPORT_ALIAS(_alias,_event, _func, ...) \
        const void * xdata cEventParam##_event##_func##_alias[] = {(void *)_func, ##__VA_ARGS__}; \
        const  cEvent##_event##_func##_alias = \
        { \
            param = cEventParam##_event##_func##_alias, \
            paramNum = sizeof(cEventParam##_event##_func##_alias) / sizeof(void *), \
            event = _event, \
        }
#else 
    #define CEVENT_EXPORT_ALIAS(_alias, _event, _func, ...) \
            const void *cEventParam##_event##_func##_alias[] = {(void *)_func, ##__VA_ARGS__}; \
            const CEvent SECTION("cEvent") cEvent##_event##_func##_alias = \
            { \
                .param = cEventParam##_event##_func##_alias, \
                .paramNum = sizeof(cEventParam##_event##_func##_alias) / sizeof(void *), \
                .event = _event, \
            }
#endif

/**
 * @brief CEvent
 */
typedef struct 
{
    void **param;                         /**< 参数(包括函数)指向一个数组，数组中存放了指向函数地址的指针和指向参数的指针 */
    unsigned char paramNum;               /**< 参数数量 */
    unsigned short event;                 /**< 监听事件 */
} CEvent;

CEvent setparams(void ** params,const unsigned short _event);

unsigned char get_lastcevent(CEvent* event_list);

void ceventInit(void);

void ceventPost(unsigned short event);
#endif

