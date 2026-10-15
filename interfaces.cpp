#include <iostream>

class Entity{
  public:
    virtual getid(){return 9;}
};

class Player : public Entity{
  private:
    char name;
  public:
    Player(const char& name):name(name){}

    int getid(){return name;}
};

int main(){
  return 0;
}
