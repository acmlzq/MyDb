#include "db.h"
#include<algorithm>
#include<windows.h>

string toString(int num)
{
    string s="";
    while(num) s+=(num%10+'0'),num/=10;
    reverse(s.begin(),s.end());
    return s;
}

const int MAXLEN=15;

int main()
{
    db d;
    d.GetAll();
    d.Insert(toString(12),toString(12));
    d.Insert(toString(10),toString(10));
//    Sleep(1000);
    d.stop();
    return 0;
}


