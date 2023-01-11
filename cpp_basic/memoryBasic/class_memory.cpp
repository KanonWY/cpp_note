// #include <iostream>
// #include <typeinfo>

class X {
  public:
    int x;
    virtual ~X() {}
    virtual void printX() {}
};

class Y {
  public:
    int y;
    virtual ~Y() {}
    virtual void printY() {}
};

class Z : public X, public Y {
  public:
    int z;
    ~Z() override {}
    void printX() override {}
    void printY() override {}
    void printZ() {}
};

class M : public X, public Y {
  public:
    int m;
    ~M() override {}
    void printX() override {}
    void printY() override {}
    virtual void printM() {}
};

int main() {
    X x;
    Y y;
    Z z;
    M m;
    return 0;
}
