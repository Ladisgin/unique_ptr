#include <iostream>
#include "unique_ptr.h"

using namespace std;

struct test {
    int a;
    int b;
    void f(){
        cout  << "++";
    }
};

int main()
{
    size_t count = 0;
    for (size_t i =0; i< 1000; ++i) {
        my_uniq::unique_ptr<int> aaa(new int(5), [&count, &i](int* a){std::cout << i << endl; count++; delete a;});
        my_uniq::unique_ptr<int> bbb(new int(5), std::default_delete<int>());
        auto kkk = my_uniq::make_unique<test>();
    }
    my_uniq::unique_ptr<test> ttt(new test, std::default_delete<test>());
    ttt->f();
    cout << endl << count;
    return 0;
}
