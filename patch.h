// patch.h
#ifndef PATCH_H
#define PATCH_H

#include "parameters.h"
#include "agent.h"
#include "turtle.h"
#include <memory>
#include <vector>

class Patch : public Agent{
private:
    std::vector<std::shared_ptr<Turtle>> turtles_here;

    int patch_type;
    double s1p_level, cxcl13_level, ccl19_level, ebi2_level;
    double il2, il4, il6, il10, il12, il15, il21;
    double if_g, if_a, tnf_a, tgf_b;
    double temp_var; // placeholder for diffusion totals before updating

public:
    Patch():Agent(0,0){setColor("red");}; // base patch color is red
    Patch(int xCoord, int yCoord) : Agent(xCoord, yCoord) {setColor("red");};

    void add_turtle(std::shared_ptr<Turtle> turtle_to_add);
    void remove_turtle(std::shared_ptr<Turtle> turtle_to_remove);
    const std::vector<std::shared_ptr<Turtle>>& getTurtlesHere() const {return turtles_here;}
    void display();
    bool is_at_capacity();

    void setTempVar(double newVal){temp_var=newVal;};
    double getTempVar() const {return temp_var;};

    void setPatchType(int type) { patch_type = type; }
    int getPatchType() const { return patch_type; }

    // Getter and setter for each new variable
    // s1p_level
    void setS1pLevel(double level) { s1p_level = level; }
    double getS1pLevel() const { return s1p_level; }

    // cxcl13_level
    void setCxcl13Level(double level) { cxcl13_level = level; }
    double getCxcl13Level() const { return cxcl13_level; }

    // ccl19_level
    void setCcl19Level(double level) { ccl19_level = level; }
    double getCcl19Level() const { return ccl19_level; }

    // ebi2_level
    void setEbi2Level(double level) { ebi2_level = level; }
    double getEbi2Level() const { return ebi2_level; }

    // il2
    void setIl2(double value) { il2 = value; }
    double getIl2() const { return il2; }

    // il4
    void setIl4(double value) { il4 = value; }
    double getIl4() const { return il4; }

    // il6
    void setIl6(double value) { il6 = value; }
    double getIl6() const { return il6; }

    // il10
    void setIl10(double value) { il10 = value; }
    double getIl10() const { return il10; }

    // il12
    void setIl12(double value) { il12 = value; }
    double getIl12() const { return il12; }

    // il15
    void setIl15(double value) { il15 = value; }
    double getIl15() const { return il15; }

    // il21
    void setIl21(double value) { il21 = value; }
    double getIl21() const { return il21; }

    // if_g
    void setIfG(double value) { if_g = value; }
    double getIfG() const { return if_g; }

    // if_a
    void setIfA(double value) { if_a = value; }
    double getIfA() const { return if_a; }

    // tnf_a
    void setTnfA(double value) { tnf_a = value; }
    double getTnfA() const { return tnf_a; }

    // tgf_b
    void setTgfB(double value) { tgf_b = value; }
    double getTgfB() const { return tgf_b; }
    
    // patch is the same iff the x and y positions are equal
    bool operator== (Patch& that){return (this->getX()==that.getX() && this->getY()==that.getY());};

    // Add more patch specific functions below for the particular implementation of a net logo model being ported
};

#endif
