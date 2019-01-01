#include <stddef.h>
#include "random.h"
#include<iostream>
#include<string>
#include<mutex>
using namespace std;
//������Կ���
#define Debug

//������
const int MAX_LEVEL = 16;

//����key��value������
typedef string KeyType;
typedef string ValueType;

//������
typedef struct nodeStructure* Node;
struct nodeStructure
{
    KeyType* key;
    ValueType* value;
    Node forward[1];
};

//������Ծ��
typedef struct listStructure* List;
struct listStructure
{
    int level;
    Node header;
};

class SkipList
{
public:
    //��ʼ����ṹ
    SkipList():rnd_(0xdeadbeef)
    {
        NewList();
    }
    //�ͷ��ڴ�ռ�
    ~SkipList()
    {
        FreeList();
    }

    //����key����������value
    //�ҵ�������true
    //δ�ҵ�������false
    bool SlSearch(const KeyType& key,
                ValueType& value);

    //����key��value
    bool SlInsert(const KeyType& key,
                const ValueType& value);

    //ɾ��key,��������value
    //ɾ���ɹ�����true
    //δɾ���ɹ�����false
    bool SlDelete(const KeyType& key,
                ValueType& value);

    //�������Ԫ�ص���Ŀ
    int size()
    {
        return size_;
    }

    //��ӡ��ǰ����level
    int GetCurrentLevel();

    //��
    mutex mut;
private:
    //��ʼ����
    void NewList();

    //�ͷű�
    void FreeList();

    //����һ���µĽ�㣬���Ĳ���Ϊlevel
    void NewNodeWithLevel(const int& level,
                          Node& node);

    //�������һ��level
    int RandomLevel();

//    //���ݴ浽����
//    bool DumpDataToDisk();
//
//    //��ȡ��������
//    bool ReadDiskToData();
public:
    List list_;
    Node NIL_;
private:
    //�����а���Ԫ�ص���Ŀ
    size_t size_;
    //�����������
    Random rnd_;
};
