#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义参数类型
typedef enum {
    INT,
    FLOAT,
    STRING
} ParamType;

// 定义参数结构体
typedef struct {
    ParamType type;
    void* value;
} Param;

// 定义事件结构体
typedef struct {
    void (*callback)(int, Param*);  // 回调函数指针
    int paramCount;                 // 参数数量
    Param* params;                  // 参数数组
} Event;

// 初始化参数结构体
void initializeParam(Param* param, ParamType type, void* value) {
    param->type = type;
    param->value = value;
}

// 初始化事件结构体
void initializeEvent(Event* event, void (*callback)(int, Param*), int paramCount, Param* params) {
    event->callback = callback;
    event->paramCount = paramCount;

    // 复制参数数组
    event->params = malloc(paramCount * sizeof(Param));
    memcpy(event->params, params, paramCount * sizeof(Param));
}

// 释放事件结构体资源
void releaseEvent(Event* event) {
    free(event->params);
}

// 回调函数示例
void callbackFunction(int paramCount, Param* params) {
    printf("Received %d parameters:\n", paramCount);
    for (int i = 0; i < paramCount; ++i) {
        switch (params[i].type) {
            case INT:
                printf("Type: INT, Value: %d\n", *((int*)params[i].value));
                break;
            case FLOAT:
                printf("Type: FLOAT, Value: %f\n", *((float*)params[i].value));
                break;
            case STRING:
                printf("Type: STRING, Value: %s\n", (char*)params[i].value);
                break;
            default:
                printf("Unknown type\n");
        }
    }
}

int main() {
    // 准备参数
    int intValue = 42;
    float floatValue = 3.14;
    char stringValue[] = "Hello";

    // 创建参数数组
    Param params[] = {
        {INT, &intValue},
        {FLOAT, &floatValue},
        {STRING, stringValue}
    };

    // 创建事件并初始化
    Event myEvent;
    initializeEvent(&myEvent, callbackFunction, sizeof(params) / sizeof(params[0]), params);

    // 触发事件，调用回调函数
    raiseEvent(&myEvent);

    // 释放资源
    releaseEvent(&myEvent);

    return 0;
}
