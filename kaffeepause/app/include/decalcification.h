#pragma once

class Decalcification {
private:
    int timer;
    bool tabletInside;

public:
    // Constructor
    Decalcification(int timer, bool tabletInside);
    ~Decalcification();
    // testing 123


    // Getter for tablet_inside
    bool getTabletInside();

    // Setter for tablet_inside
    void setTabletInside(bool tabletInside);
protected :
    // Method for decalcification
    bool decalcification(int timer);
};

