#include "skiplist.h"
#include "time.h"
#include <assert.h>
#include <stdlib.h>
#include <windows.h>

void DebugOutput(const string& information)
{
#ifdef Debug
    cout << information << endl;
#endif
}

void SkipList::NewNodeWithLevel(const int& level,Node& node)
{

    //�½��ռ��С
    int total_size = sizeof(nodeStructure) + level*sizeof(Node);
    //����ռ�
    node = (Node)malloc(total_size);
    node->key = NULL,node->value=NULL;
    assert(node != NULL);
}


void SkipList::NewList()
{
    //����NIL���
    NewNodeWithLevel(0, NIL_);
    NIL_->key = new string;
//    *(NIL_->key) = "zzzzzzzzzzz";

    //��������List
    list_ = (List)malloc(sizeof(listStructure));
    list_->level = 0;

    //����ͷ���
    NewNodeWithLevel(MAX_LEVEL,list_->header);
    for(int i = 0; i < MAX_LEVEL; ++i)
    {
        list_->header->forward[i] = NIL_;
    }
    //��������Ԫ�ص���Ŀ
    size_ = 0;

}

void SkipList::FreeList()
{
    Node p = list_->header;
    Node q;
    while(p != NIL_)
    {
        q = p->forward[0];
        if(p->key!=NULL)
            free(p->key);
        if(p->value!=NULL)
            free(p->value);
        free(p);
        p = q;
    }
    free(p);
    free(list_);
}


bool SkipList::SlSearch(const KeyType& key,
                        ValueType& value)
{
    Node x = list_->header;
    int i;
    for(i = list_->level; i >= 0; --i)
    {
        while(x->forward[i]!=NIL_ && *(x->forward[i]->key) < key)
        {
            x = x->forward[i];
        }
    }
    x = x->forward[0];
    if(x!=NIL_ && *(x->key) == key)
    {
        value = *(x->value);
        return true;
    }
    else
    {
        return false;
    }
}


bool SkipList::SlInsert(const KeyType& key,
                        const ValueType& value)
{
    Node update[MAX_LEVEL];
    int i;
    Node x = list_->header;
    //Ѱ��key��Ҫ�����λ��
    //�����Լkey��λ����Ϣ
    for(i = list_->level; i >= 0; --i)
    {
        while(x->forward[i]!=NIL_ && *(x->forward[i]->key) < key)
        {
            x = x->forward[i];
        }

        update[i] = x;
    }

    x = x->forward[0];
    //���key�Ѿ�����
    if(x!=NIL_ && *(x->key) == key)
    {
        *(x->value) = value;
        return false;
    }
    else
    {
        //��������½��Ĳ���
        int level = RandomLevel();
        //Ϊ�˽�ʡ�ռ䣬���ñȵ�ǰ��������1�Ĳ���
        if(level > list_->level)
        {
            level = ++list_->level;
            update[level] = list_->header;
        }
        //�����µĽ��
        Node newNode;
        NewNodeWithLevel(level, newNode);
        newNode->key = new string;
        newNode->value = new string;
        *(newNode->key) = key;
        *(newNode->value) = value;


        mut.lock();
        //����forwardָ��
        for(int i = level; i >= 0; --i)
        {
            x = update[i];
            newNode->forward[i] = x->forward[i];
            x->forward[i] = newNode;
        }
        mut.unlock();
        //����Ԫ����Ŀ
        ++size_;

        return true;
    }
}


bool SkipList::SlDelete(const KeyType& key,
                        ValueType& value)
{
    Node update[MAX_LEVEL];
    int i;
    Node x = list_->header;
    //Ѱ��Ҫɾ���Ľ��
    for(i = list_->level; i >= 0; --i)
    {
        while(x->forward[i]!=NIL_ && *(x->forward[i]->key) < key)
        {
            x = x->forward[i];
        }

        update[i] = x;
    }

    x = x->forward[0];
    //��㲻����
    if(x!=NIL_ && *(x->key) != key)
    {
        return false;
    }
    else
    {
        value = *(x->value);
        //����ָ��
        mut.lock();
        for(i = 0; i <= list_->level; ++i)
        {
            if(update[i]->forward[i] != x)
                break;
            update[i]->forward[i] = x->forward[i];
        }
        mut.unlock();
        //ɾ�����
        delete x->key;
        delete x->value;
        free(x);
        //����level��ֵ���п��ܻ�仯����ɿռ���˷�
        while(list_->level > 0
                && list_->header->forward[list_->level] == NIL_)
        {
            --list_->level;
        }

        //��������Ԫ����Ŀ
        --size_;

        return true;
    }
}


int SkipList::RandomLevel()
{
    int level = static_cast<int>(rnd_.Uniform(MAX_LEVEL));
    if(level == 0)
    {
        level = 1;
    }
    return level;
}

int SkipList::GetCurrentLevel()
{
    return list_->level;
}





