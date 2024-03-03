/**
 * @file cevent.c
 * @author Letter (nevermindzzt@gmail.com)
 * @brief c event
 * @version 1.0.0
 * @date 2020-11-15
 * 
 * @copyright (c) 2020 Letter
 * 
 */
 
#include "cevent.h"
#include <stddef.h>
#pragma userclass (near=CEVENT__pp)	
    CEvent ** pofp;
#pragma userclass (near=default)
#pragma userclass (near=CEVENT__p)	
    CEvent *p;
#pragma userclass (near=default)
#pragma userclass (near=size__pp)	
    size_t * sizep;
#pragma userclass (near=default)
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 6000000)
    extern const size_t cEvent$$Base;
    extern const size_t cEvent$$Limit;
#elif defined(__ICCARM__) || defined(__ICCRX__)
    #pragma section="cEvent"
#elif defined(__GNUC__)
    extern const size_t _cevent_start;
    extern const size_t _cevent_end;
#elif defined(__C251__)

#endif

#if (CEVENT_SPEED_OPTIMIZE == 1 && CEVENT_BUFFER_SIZE > 0)
static size_t ceventBuffer[CEVENT_BUFFER_SIZE] = {0};                       //ceventbuffer是size_t类型的数组，偏移的时候以size_t的大小为基准
#endif

/**
 * @brief cevent注册表
 */
struct 
{
#if (CEVENT_SPEED_OPTIMIZE == 1)
    size_t *eventBase;                 /**< 事件基址 */
#else
    CEvent *base;                       /**< 表基址 */
    size_t count;                       /**< 表大小 */
#endif
} ceventTable;                          //没有结构体标签，只能定义一个变量


#if defined(__C251__) || (__C51__)
/**
 * @brief 获取数组中最后连续存放的cevent序号
 */
unsigned char get_lastcevent(CEvent* event_list)
{
    unsigned char i;
    for(i=0;(i<(CEVENT_BUFFER_SIZE/2));i++)
    {
        if(*((*(event_list+i)).param)==NULL)			//param是一个类型为void* 的数组，应该再次取地址获得第一个参数：函数指针
            break;
    }
    return i;                 //返回下一个空余位置序号
}
#endif
/**
 * @brief cevent初始化
 */
void ceventInit(void)               //初始化直接去相应的内存找相关的信息，比如事件个数，事件基址等在编译的时候就已经确定在存储器中
{
    CEvent *base;                   //事件表的段基址
    size_t count;                   //事件的个数
	size_t i,j;
	size_t *cur;
	size_t maxEvent = 0;
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 6000000)
    base = (CEvent *)(&cEvent$$Base);
    count = ((size_t)(&cEvent$$Limit) - (size_t)(&cEvent$$Base)) / sizeof(CEvent);
#elif defined(__ICCARM__) || defined(__ICCRX__)
    base = (CEvent *)(__section_begin("cEvent"));
    count = ((size_t)(__section_end("cEvent")) - (size_t)(__section_begin("cEvent")))
            / sizeof(CEvent);
#elif defined(__GNUC__)
    base = (CEvent *)(&_cevent_start);
    count = ((size_t)(&_cevent_end) - (size_t)(&_cevent_start)) / sizeof(CEvent);
#elif defined(__C251__) || (__C51__)
	
    base = ABSOLUTE_ADDR;
    count = get_lastcevent((CEvent *)ABSOLUTE_ADDR);
    // count = 3;
#else
//    #error not supported compiler, please use command table mode
#endif

#if CEVENT_SPEED_OPTIMIZE == 1

    for (i = 0; i < count; i++)                 //事件标号可以由用户任意设定
    {                                           //但是事件数量在设置之后是一定的(count)
        if (base[i].event > maxEvent) {
            maxEvent = base[i].event;
        }
    }
    maxEvent += 1;                              //为了正确确定循环次数和cur的初始位置，需要加一
#if CEVENT_BUFFER_SIZE <= 0
    ceventBuffer = CEVENT_MALLOC(((maxEvent << 1) + count) * sizeof(size_t));
#endif /** CEVENT_BUFFER_SIZE <= 0 */
    ceventTable.eventBase = (size_t *) ceventBuffer;       //重新排序建立索引表ceventTable，eventbase是指针类型占用四个字节
    cur = ceventBuffer + maxEvent;                          //cur是指向size_t的指针，size_t占用两个字节，所以
    for (i = 0; i < maxEvent; i++)
    {
        ceventTable.eventBase[i] = cur;                 //这里含义是将buffer分成两个部分，前半部分是索引，可以根据事件号快速找到位置
        for (j = 0; j < count; j++)                     //前半部分的指针指向后半部分，后半部分存放着实际的Cevnet地址，相同事件号存储方式是连续的
        {                                               //不同事件号之间间隔一个NULL指针，所以需要buffer为2*maxevent+count
            if (base[j].event == i) {
                *cur++ = (size_t ) &base[j];            //本来以base为基准的地址是一个4字节的信息，强制转化为两个字节存入ceventbuffer
            }                                           
        }                                               
        *cur++ = NULL;
    }
#else /** CEVENT_SPEED_OPTIMIZE == 1 */
    ceventTable.base = base;
    ceventTable.count = count;
#endif
}

/**
 * @brief cevent运行
 * 
 * @param cevent cevent
 * 
 */
static void ceventRun(CEvent *cevent)
{
	void (*function)(void *,...) 	    reentrant;
	void (*function_void)();
    if (cevent->paramNum < 1)
    {
        return;
    }
	function = (void (*)(void *,...))(cevent->param[0]);
	if(cevent->param[1] == NULL) cevent->paramNum = 1;
    switch (cevent->paramNum)
	{
		case 1:
			function_void =(void (*)())(cevent->param[0]);
			function_void();
			break;
		case 2:
			function(cevent->param[1]);
			break;
		case 3:
			function(cevent->param[1], cevent->param[2]);
			break;
		case 4:
			function(cevent->param[1], cevent->param[2], cevent->param[3]);
			break;
		case 5:
			function(cevent->param[1], cevent->param[2], cevent->param[3], cevent->param[4]);
			break;
		case 6:
			function(cevent->param[1], cevent->param[2], cevent->param[3], cevent->param[4],
					 cevent->param[5]);
			break;
		case 7:
			function(cevent->param[1], cevent->param[2], cevent->param[3], cevent->param[4],
					 cevent->param[5], cevent->param[6]);
			break;
		case 8:
			function(cevent->param[1], cevent->param[2], cevent->param[3], cevent->param[4],
					 cevent->param[5], cevent->param[6], cevent->param[7]);
			break;
		default:
			break;
    }
}

/**
 * @brief cevent处理
 * 
 * @param event 事件
 * 
 */
static void ceventHandler(unsigned short event)
{
	size_t i;
#if CEVENT_SPEED_OPTIMIZE == 1
    // CEvent **cevent = (CEvent **) ceventTable.eventBase[event];
    size_t * cevent = (size_t *) ceventTable.eventBase[event];                   //强制使指针指向的数据类型仅占用2个字节
    while (*cevent != NULL)                                                     //使其在自增加一的时候能够正确获取地址信息
    {
        ceventRun((CEvent *)*cevent++);
    }
#else
    for ( i= 0; i < ceventTable.count; i++)
    {
        if (ceventTable.base[i].event == event)
        {
            ceventRun(&(ceventTable.base[i]));
        }
    }
#endif
}

/**
 * @brief 广播event事件
 * 
 * @param event 事件
 * 
 */
void ceventPost(unsigned short event)
{
    ceventHandler(event);
}

