#include <iostream>
using namespace std;

class Base {
    public:
    virtual void draw() {
        cout << "Base";
    } 
    void base() {
        cout << "aas";
    }
};

class Derived : public Base {
    public:
    void draw() {
        cout << "derived";
    } 
    void derived() {
        cout << "hehe";
    }
};

class Derived2 : public Base {
    public:
    void draw1() {
        cout << "derived2";
    } 
    void derived1() {
        cout << "hehe";
    }
};

int main() {
    Derived* d = new Derived();
    d->draw();

    Derived2* d1 = dynamic_cast<Derived2*>(d);
    d1->draw1();
}