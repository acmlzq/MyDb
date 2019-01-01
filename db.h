#include "skiplist.h"
#include "lru.h"
#include<thread>
#include<condition_variable>
#include<boost/crc.hpp>

class db
{
public:
    db():LRU(1000)
    {
        is_running=true;
        Read();
        _thread = thread(Guard,this);
        _thread.detach();
    }
    bool Dump();
    bool Read();

    void GetAll();
    bool Search(const KeyType& key,ValueType& value);
    bool Insert(const KeyType& key,const ValueType& value);
    bool Delete(const KeyType& key,ValueType& value);
    int size();
    void stop();
private:
    SkipList SL;
    LruCache LRU;
    string dirname="data.dat";
    thread _thread;
    void Guard();
    condition_variable cond;
    bool is_running;
};


