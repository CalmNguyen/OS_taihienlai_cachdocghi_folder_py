#include <iostream>
#include<string>
#include"ChuyenCoSo.h"
using namespace std;
int StranToDe(string s, int BaseBase)// trans base to de base
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] < '0' || s[i] > 'Z' || s[i] > '9' && s[i] < 'A')
        {
            cout << "fail";
            return -1;
        }
    }
    int sum = 0;
    for (int i=s.size()-1; i >= 0; i--)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            sum += (int(s[i])-48) * pow(BaseBase, s.size()-i-1);//int(1)= int('1')-48
        }               
        else
        {
            sum += (int(s[i]) - 55) * pow(BaseBase, s.size()-i-1);//int(10)=int('A')-55
        }
    }
    return sum;
}
string TransToOptions(int De, int BaseOption)
{
    if (BaseOption < 2 || BaseOption>36)
        return "oo";
    string result;
    string tempResult;
    int temp;
    while (De != 0)
    {
        temp = int(De) % BaseOption;
        De = De / BaseOption;
        if (temp >= 0 && temp <= 9)
            tempResult += char(temp+48);
        else
            tempResult += char(temp + 55);
    }
    int size = tempResult.size();
    for (int i = 0; i < tempResult.length(); i++)//swap string
    {
        result += tempResult[tempResult.length() - 1 - i];
    }
    return result;
}