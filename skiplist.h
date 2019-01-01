#include <stddef.h>
#include "random.h"
#include<iostream>
#include<string>
#include<mutex>
using namespace std;
//定义调试开关
#define Debug

//最大层数
const int MAX_LEVEL = 16;

//定义key和value的类型
typedef string KeyType;
typedef string ValueType;

//定义结点
typedef struct nodeStructure* Node;
struct nodeStructure
{
    KeyType* key;
    ValueType* value;
    Node forward[1];
};

//定义跳跃表
typedef struct listStructure* List;
struct listStructure
{
    int level;
    Node header;
};

class SkipList
{
public:
    //初始化表结构
    SkipList():rnd_(0xdeadbeef)
    {
        NewList();
    }
    //释放内存空间
    ~SkipList()
    {
        FreeList();
    }

    //搜索key，保存结果至value
    //找到，返回true
    //未找到，返回false
    bool SlSearch(const KeyType& key,
                ValueType& value);

    //插入key和value
    bool SlInsert(const KeyType& key,
                const ValueType& value);

    //删除key,保存结果至value
    //删除成功返回true
    //未删除成功返回false
    bool SlDelete(const KeyType& key,
                ValueType& value);

    //链表包含元素的数目
    int size()
    {
        return size_;
    }

    //打印当前最大的level
    int GetCurrentLevel();

    //锁
    mutex mut;
private:
    //初始化表
    void NewList();

    //释放表
    void FreeList();

    //创建一个新的结点，结点的层数为level
    void NewNodeWithLevel(const int& level,
                          Node& node);

    //随机生成一个level
    int RandomLevel();

//    //数据存到磁盘
//    bool DumpDataToDisk();
//
//    //读取磁盘数据
//    bool ReadDiskToData();
public:
    List list_;
    Node NIL_;
private:
    //链表中包含元素的数目
    size_t size_;
    //随机器生成器
    Random rnd_;
};
