// agent.h
#ifndef AGENT_H
#define AGENT_H


#include <iostream>
#include <fstream>
#include <vector>

class Agent {
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
};


#endif
