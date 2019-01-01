#ifndef LRU_H_INCLUDED
#define LRU_H_INCLUDED

#include<unordered_map>
#include<list>
#include<algorithm>
#include<string>
using namespace std;
class LruCache
{
public:
    LruCache(int capacity):cap(capacity) {}

    bool get(const string& key,string &value)
    {
        auto it=m.find(key);
        if(it==m.end()) return false;
        l.splice(l.begin(),l,it->second); //将it->second 插入到 l.begin()前面
        value=it->second->second;
        return true;
//        return it->second->second;
    }

    bool put(const string &key, const string &value)
    {
        auto it=m.find(key);
        if(it!=m.end()) l.erase(it->second);
        l.push_front(make_pair(key,value));
        m[key]=l.begin();
        if(m.size()>cap)
        {
            auto it=l.rbegin()->first;
            m.erase(it);
            l.pop_back();
        }
    }
    void rem(const string &key)
    {
        auto it=m.find(key);
        if(it!=m.end())
        {
            l.erase(it->second);
            m.erase(key);
        }
    }
private:
    int cap;
    list<pair<string,string>>l;
    unordered_map<string,list<pair<string,string>>::iterator>m;
};



#endif // LRU_H_INCLUDED
