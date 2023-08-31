// agent.h
#ifndef AGENT_H
#define AGENT_H

#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "parameters.h"

class Agent: std::enable_shared_from_this<Agent> {
  protected:
    Agent(int x, int y) : x(x), y(y) {}
  private:
    std::string color = "";
    std::string secondaryColor = "";
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
    void setSecondaryColor(std::string newColor){secondaryColor = newColor;};
    std::string getSecondaryColor() const {return secondaryColor;};
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
      // std::cout<<"getting angle1"<<std::endl;
      // std::cout<<"Self Location ("<<x<<", "<<y<<")"<<std::endl;
      int targetx = target.getX();
      int targety = target.getY();
      // std::cout<<"getting angle2"<<std::endl;
      // std::cout<<"Target Location ("<<targetx<<", "<<targety<<")"<<std::endl;
      int delta_x = targetx - x;
      int delta_y = y - targety;
      // std::cout<<"getting angle3"<<std::endl;
      // std::cout<<"Delta Positions ("<<delta_x<<", "<<delta_y<<")"<<std::endl;
      if ((delta_x + WORLD_WIDTH) < abs(delta_x)){
        delta_x = (delta_x + WORLD_WIDTH)% WORLD_WIDTH;
      }
      if ((delta_y + WORLD_HEIGHT) < abs(delta_y)){
        // delta_x = (delta_x + WORLD_WIDTH)% WORLD_WIDTH;
        delta_y = (delta_y + WORLD_HEIGHT)% WORLD_HEIGHT;
      }

      if ((abs(delta_x - WORLD_WIDTH)) < abs(delta_x)){
        delta_x -= WORLD_WIDTH;
      }
      if ((abs(delta_y - WORLD_WIDTH)) < abs(delta_y)){
        delta_y -= WORLD_HEIGHT;
      }
      // std::cout<<"getting angle4"<<std::endl;
      // std::cout<<"Delta Positions ("<<delta_x<<", "<<delta_y<<")"<<std::endl;
      // std::cout<<"getting angle5"<<std::endl;
      // std::cout<<"Angle To neighbor 0-TL(degrees)"<<atan2(1,-1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 1-T(degrees)"<<atan2(1,0) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 2-TR(degrees)"<<atan2(1,1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 3-L(degrees)"<<atan2(0,-1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 4-R(degrees)"<<atan2(0,1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 5-BL(degrees)"<<atan2(-1,-1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 6-B(degrees)"<<atan2(-1,0) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 7-BR(degrees)"<<atan2(-1,1) * 180/M_PI<<std::endl;
      // double angle_radian = atan(delta_x/delta_y);
      double angle_radian = atan2(delta_y, delta_x);
      while (angle_radian<0){angle_radian+=2*M_PI;}
      // if (angle_radian<-1*M_PI){angle_radian+=2*M_PI;}
      // std::cout<<"Angle To (degrees)"<<angle_radian * 180/M_PI<<std::endl;
      return angle_radian *  180/M_PI;
    }

};


#endif
