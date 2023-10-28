#include<iostream>

#include "../src/safe_queue.hpp"


using namespace std;

class Base
{
public:
    Base() { cout << "create Base" << endl; }

    virtual void fun() {
        cout << "fun Base" << endl;
    }

    virtual ~Base() { cout << "delete Base" << endl; }
};

class Der : public Base
{
public:
    Der() { cout << "create Der" << endl; }

    virtual void fun() {
        cout << "fun Der" << endl;
    }

    ~Der() { cout << "Delete Der" << endl; }
};


int main(int argc, char const* argv[])
{
    Base* b = new Der;
    
    ((Der*)b)->fun();
    
    b->Base::fun();
    
    delete b;

    return 0;
}