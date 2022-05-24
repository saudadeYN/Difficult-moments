#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Timer.h"

using namespace std;

const string vowels{ "AEIOUaeiou" };

template<typename T>
void Swap(T* a, T* b)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template<typename T>
void SortPointers(vector<T*>& v)
{
    sort(v.begin(), v.end(), [](const auto& v1, const auto& v2)
        {
            return *v1 < *v2;
        });
}

void task1() {
    int a(1);
    int b(9);

    cout <<  a << ' ' <<  b << endl;
    cout << &a << ' ' << &b << endl;
    Swap(&a, &b);
    cout <<  a << ' ' <<  b << endl;
    cout << &a << ' ' << &b << endl;
}

void task2() {
    int length = 5;
    vector<int*> vec;
    int* a = new int[length] {2, 1, 3, 5, 4};

    for (int i = 0; i < length; ++i) {
        vec.push_back(&a[i]);
    }

    for (int i = 0; i < length; ++i) {
        cout << *vec[i] << ' ';
    }

    cout << endl;

    SortPointers(vec);

    for (int i = 0; i < length; ++i) {
        cout << *vec[i] << ' ';
    }
    delete[] a;
    cout << endl;
}

void method1(const string& s)
{
    Timer timer("Method 1");
    size_t count = count_if(s.begin(), s.end(), [&](const auto& c)
        {
            return vowels.find(c) != string::npos;
        });
    cout << count << endl;
    timer.print();
}

void method2(const string& s)
{
    Timer timer("Method 2");
    size_t count = count_if(s.begin(), s.end(), [&](const auto& c)
        {
            for (size_t i = 0; i < vowels.size(); ++i)
            {
                if (vowels[i] == c)
                    return true;
            }
            return false;
        });
    cout << count << endl;
    timer.print();
}

void method3(const string& s)
{
    size_t count = 0;
    Timer timer("Method 3");
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (vowels.find(s[i]) != string::npos)
        {
            ++count;
        }
    }
    cout << count << endl;
    timer.print();
}

void method4(const string& s)
{
    size_t count = 0;
    Timer timer("Method 4");
    for (size_t i = 0; i < s.size(); ++i)
    {
        for (size_t j = 0; j < vowels.size(); ++j)
        {
            if (vowels[j] == s[i])
                ++count;
        }
    }
    cout << count << endl;
    timer.print();
}

void task3()
{
    ifstream file("War_and_peace.txt");
    file.seekg(0, ios::end);
    size_t size = file.tellg();
    file.seekg(0);
    string s(size, ' ');
    file.read(&s[0], size);

    method1(s);
    method2(s);
    method3(s);
    method4(s);
}

int main() {
    task1();
    cout << "-----------------------------------" << endl;
    task2();
    cout << "-----------------------------------" << endl;
    task3();

    return 0;
}