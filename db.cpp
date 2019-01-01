#include "db.h"
#include<fstream>

bool db::Dump()
{
    ofstream outfile(dirname,ios::binary);
    if(!outfile)
    {
        cout<<"open error"<<endl;
        return false;
    }
    Node x=SL.list_->header;
    while(x->forward[0]!=SL.NIL_)
    {
        x=x->forward[0];
        int key_size=(*(x->key)).size(),val_size=(*(x->value)).size();
        string get_key=*(x->key),get_val=*(x->value);
        outfile.write((char*)&key_size,sizeof(key_size));
        outfile.write((char*)&val_size,sizeof(val_size));
        outfile.write((char*)get_key.c_str(),key_size);
        outfile.write((char*)get_val.c_str(),val_size);
        boost::crc_32_type crc32;
        crc32.reset();
        crc32.process_bytes(get_val.c_str(),get_val.length());
        unsigned crc_num=crc32.checksum();
        outfile.write((char*)&crc_num,sizeof(crc_num));
    }
    outfile.close();
    return true;
}

bool db::Read()
{
    ifstream infile(dirname,ios::binary);
    if(!infile)
    {
        cout<<"open error"<<endl;
        return false;
    }
    while(infile.peek()!=EOF)
    {
        int key_size,val_size;
        string get_key,get_val;
        infile.read((char*)&key_size,sizeof(key_size));
        infile.read((char*)&val_size,sizeof(val_size));
        get_key.resize(key_size);
        get_val.resize(val_size);
        infile.read((char*)get_key.c_str(),key_size);
        infile.read((char*)get_val.c_str(),val_size);

        unsigned int crc_num;
        infile.read((char*)&crc_num,sizeof(crc_num));

        boost::crc_32_type crc32;
        crc32.reset();
        crc32.process_bytes(get_val.c_str(),get_val.length());
        unsigned int crc_ans=crc32.checksum();
        if(crc_ans==crc_num)
        {
            SL.SlInsert(get_key,get_val);
            LRU.put(get_key,get_val);
        }
    }
    infile.close();
    return true;
}


void db::GetAll()
{
    Node x=SL.list_->header;
    while(x->forward[0]!=SL.NIL_)
    {
        x=x->forward[0];
        cout<<*(x->key)<<" => "<<*(x->value)<<endl;
    }
}
bool db::Search(const KeyType& key,ValueType& value)
{
    if(LRU.get(key,value))
        return true;
    return SL.SlSearch(key,value);
}

bool db::Insert(const KeyType& key,const ValueType& value)
{
    LRU.put(key,value);
    return SL.SlInsert(key,value);
}
bool db::Delete(const KeyType& key,ValueType& value)
{
    LRU.rem(key);
    return SL.SlDelete(key,value);
}


int db::size()
{
    return SL.size();
}
void db::Guard()
{
    while(is_running)
    {
        SL.mut.lock();
        Dump();
        SL.mut.unlock();
    }
    cond.notify_one();
    cout<<"write over~~~"<<endl;
}
void db::stop()
{
    mutex mut1;
    unique_lock<mutex> lk(mut1);
    is_running=false;
    cond.wait(lk);
}

