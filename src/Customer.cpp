#include <iostream>
#include "../include/Customer.h"

using namespace std;

Customer::Customer(string c_name, int c_id):name(c_name),id(c_id),numOfOrders(0),orders(){}

string Customer:: getName() const{
    return name;
}

int Customer::getId() const {
    return id;
}

int Customer::getNumOfOrders() {
    return numOfOrders;
}

void Customer::addNumOfOrders() {
    numOfOrders++;
}

std::vector<int> Customer::getOrders() {
    return orders;
}

void Customer::setNumOfOrders(int num) {
    numOfOrders = num;
}

void Customer::setOrders(std::vector<int> orders) {
    this->orders = orders;
}

Customer::~Customer() {}

//---------vegeterian customer----------//

VegetarianCustomer::VegetarianCustomer(string name, int id):Customer(name, id){}

vector<int> VegetarianCustomer::order(const vector<Dish> &menu) {
    vector<int> vegOrder;
    int currVEG = -1;
    int currBEV = -1;
    int priceBEV = -1;
    if (getNumOfOrders() == 0) {
        for (vector<Dish>::const_iterator it = menu.begin(); it != menu.end(); ++it) {
            if (it->getType() == VEG) {
                if (currVEG == -1) {
                    currVEG = it->getId();
                } else if (it->getId() < currVEG)
                    currVEG = it->getId();
            }
            if (it->getType() == BVG) {
                if (currBEV == -1) {
                    currBEV = it->getId();
                    priceBEV = it->getPrice();
                } else if (it->getPrice() >= priceBEV) {
                    if (it->getPrice() > priceBEV) {
                        currBEV = it->getId();
                        priceBEV = it->getPrice();
                    } else if (it->getId() < currBEV) {
                        currBEV = it->getId();
                        priceBEV = it->getPrice();
                    }
                }
            }
        }
        if (currBEV != (-1) && currVEG != (-1)) {
            vegOrder.push_back(currVEG);
            vegOrder.push_back(currBEV);
            setOrders(vegOrder);
            addNumOfOrders();
        }
    }
    addNumOfOrders();
    return getOrders();
}


string VegetarianCustomer::toString() const{
    return getName() + ",veg";
}

VegetarianCustomer* VegetarianCustomer::clone() {
    VegetarianCustomer *ans = new VegetarianCustomer(this->getName(), this->getId());
    ans->setOrders(getOrders());
    ans->setNumOfOrders(getNumOfOrders());
    return ans;
}

//---------cheap customer----------//

CheapCustomer::CheapCustomer(string name, int id) : Customer(name, id) {}

vector<int> CheapCustomer::order(const vector<Dish> &menu) {
    vector<int> chpOrder;
    int cheapstID= -1;
    int cheapestPRICE = -1;
    if (getNumOfOrders() == 0) {
        for (vector<Dish>::const_iterator it = menu.begin(); it != menu.end(); ++it) {
            if (cheapstID == -1) {
                cheapstID = it->getId();
                cheapestPRICE = it->getPrice();
            } else if (it->getPrice() <= cheapestPRICE) {
                if (it->getPrice() < cheapestPRICE) {
                    cheapstID = it->getId();
                    cheapestPRICE = it->getPrice();
                } else if (it->getId() < cheapstID) {
                    cheapstID = it->getId();
                    cheapestPRICE = it->getPrice();
                }
            }
        }
        if (cheapstID != -1) {
            chpOrder.push_back(cheapstID);
            setOrders(chpOrder);
            addNumOfOrders();
        }
    }else
        setOrders(vector<int>());
    return getOrders();
}

string CheapCustomer::toString() const {
    return getName() + ",chp";
}

CheapCustomer* CheapCustomer::clone() {
    CheapCustomer *ans = new CheapCustomer(this->getName(), this->getId());
    ans->setOrders(getOrders());
    ans->setNumOfOrders(getNumOfOrders());
    return ans;
}

//---------spicy customer----------//

SpicyCustomer::SpicyCustomer(string name, int id) : Customer(name, id) {}

vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    vector<int> spcOrder;
    int spcID = -1;
    int spcPrice = -1;
    int bvgID = -1;
    int bvgPrice = -1;

    if (getNumOfOrders() == 0) {
        for (vector<Dish>::const_iterator it = menu.begin(); it != menu.end(); ++it) {
            if (it->getType() == SPC) {         // spicy Dishes
                if (spcID == -1) {
                    spcID = it->getId();
                    spcPrice = it->getPrice();
                } else if (it->getPrice() >= spcPrice) {
                    if (it->getPrice() > spcPrice) {
                        spcID = it->getId();
                        spcPrice = it->getPrice();
                    } else if (it->getId() < spcID) {
                        spcID = it->getId();
                        spcPrice = it->getPrice();
                    }
                }
            }
        }
        if (spcID != -1) {
            spcOrder.push_back(spcID);
            setOrders(spcOrder);
            addNumOfOrders();
        }
    }
    else if(getNumOfOrders() == 1) {
        for (vector<Dish>::const_iterator it = menu.begin(); it != menu.end(); ++it) {
            if (it->getType() == BVG) {      // cheapest non-alcoholi BVG
                if (bvgID == -1) {
                    bvgID = it->getId();
                    bvgPrice = it->getPrice();
                } else if (it->getPrice() <= bvgPrice) {
                    if (it->getPrice() < bvgPrice) {
                        bvgID = it->getId();
                        bvgPrice = it->getPrice();
                    } else if (it->getId() < bvgID) {
                        bvgID = it->getId();
                        bvgPrice = it->getPrice();
                    }
                }
            }
        }
        if (bvgID != -1) {
            spcOrder.push_back(bvgID);
            setOrders(spcOrder);
            addNumOfOrders();
        }
    }
    return getOrders();
}

string SpicyCustomer::toString() const {
    return getName() +  ",spc";}

SpicyCustomer* SpicyCustomer::clone() {
    SpicyCustomer *ans = new SpicyCustomer(this->getName(), this->getId());
    ans->setOrders(getOrders());
    ans->setNumOfOrders(getNumOfOrders());
    return ans;
}

//---------alcoholic customer----------//

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name, id) {}

vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    if (getNumOfOrders() == 0) {
        typedef std::pair<int, int> idPricePair;
        vector<idPricePair> alcOrders;
        vector<int> ans;
        //create vector of ALC drinks from the menu <id , price>
        for (int i = 0; i < (int)menu.size(); i++) {
            if (menu[i].getType() == ALC) {
                if (alcOrders.size() == 0) {
                    alcOrders.push_back(idPricePair(menu[i].getId(), menu[i].getPrice()));
                } else {
                    int j = 0;
                    while (j< (int)alcOrders.size() && menu[i].getPrice() >= (int)alcOrders[j].second)
                        j++;
                    if (j >= (int)alcOrders.size())
                        alcOrders.push_back(idPricePair(menu[i].getId(), menu[i].getPrice()));
                    else
                        alcOrders.insert(alcOrders.begin() + j, idPricePair(menu[i].getId(), menu[i].getPrice()));
                }
            }
        }
        for (int i = 0; i < (int)alcOrders.size(); i++)
            ans.push_back(alcOrders[i].first);
        setOrders(ans);
    } else if (getNumOfOrders() >= (int)getOrders().size())
        return vector<int>();

    addNumOfOrders();
    return vector<int>(1, getOrders()[getNumOfOrders() - 1]);
}

string AlchoholicCustomer::toString() const {
    return getName() +  ",alc";
};

AlchoholicCustomer* AlchoholicCustomer::clone() {
    AlchoholicCustomer *ans = new AlchoholicCustomer(this->getName(), this->getId());
    ans->setOrders(getOrders());
    ans->setNumOfOrders(getNumOfOrders());
    return ans;
}
