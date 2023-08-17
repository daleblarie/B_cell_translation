// agent.h
#ifndef AGENT_H
#define AGENT_H

#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

class Agent: std::enable_shared_from_this<Agent> {
  protected:
    Agent(int x, int y) : x(x), y(y) {}
  private:
    std::string color = "";
    std::string shape = "";
    int size=1;
    int alpha = 255;
    int x;
    int y;
    bool visible = true;
  public:
    virtual double getX() const {return x;};
    virtual double getY() const {return y;};
    virtual void setX(int newX){x=newX;};
    virtual void setY(int newY){y=newY;};
    void setColor(std::string newColor){color = newColor;};
    std::string getColor() const {return color;};
    void setShape(std::string newShape){shape = newShape;};
    std::string getShape() const {return shape;};
    void setOpacity(int newAlpha){alpha = newAlpha;};
    int getOpacity() const {return alpha;};

    void setSize(int newSize){size = newSize;};
    int getSize() const {return size;};

    void setVisible(bool status){visible = status;}
    bool getVisible() const {return visible;}
    
    std::weak_ptr<Agent> createWeakPtr() {return shared_from_this();}

    template <typename agent>
    double angle_to(agent& target){
      std::cout<<"getting angle"<<std::endl;
      std::cout<<x<<std::endl;
      std::cout<<y<<std::endl;
      int targetx = target.getX();
      int targety = target.getY();
      std::cout<<"getting angle2"<<std::endl;
      std::cout<<targetx<<std::endl;
      std::cout<<targety<<std::endl;
      int delta_x = x - targetx;
      int delta_y = y - targety;
      std::cout<<"getting angle3"<<std::endl;
      std::cout<<delta_x<<std::endl;
      std::cout<<delta_y<<std::endl;
      
      double angle_radian = atan(delta_x/delta_y);
      std::cout<<"getting angle4"<<std::endl;

      return angle_radian * M_PI/180;
    }

};


#endif
