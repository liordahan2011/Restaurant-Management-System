#include "../include/Table.h"
#include <stdexcept>
#include <iostream>

using namespace std;

Table::Table(int t_capacity):capacity(t_capacity),open(false),customersList(),orderList(){}

int Table::getCapacity() const{return capacity;}

void Table::addCustomer(Customer* costumer){
    customersList.push_back(costumer);
}

void Table::removeCustomer(int id) {
    vector<OrderPair> tmp;
    //remove customer
    vector<Customer *>::iterator it = customersList.begin();
    for ( ; it != customersList.end(); ) {
        if ((*it)->getId() == id)
            customersList.erase(it);
        else
            ++it;
    }

    //remove dishes of customer
    for (vector<OrderPair>::iterator it2 = orderList.begin(); it2 != orderList.end(); ++it2) {
        if ((*it2).first != id) {
            tmp.push_back(*it2);
        }
    }
    orderList.clear();
    for (vector<OrderPair>::iterator it3 = tmp.begin(); it3 != tmp.end(); it3++) {
            orderList.push_back(*it3);
    }
}

Customer* Table::getCustomer(int id) {
    for (vector<Customer *>::iterator it = customersList.begin(); it != customersList.end(); ++it) {
        if ((*it)->getId() == id)
            return *it;
    }
    return nullptr;
    }

vector<Customer*>& Table:: getCustomers(){
    return customersList;
}

vector<OrderPair>& Table:: getOrders(){
    return orderList;
}

void Table::order(const vector<Dish> &menu){
    vector<int> ordersID;
    for (vector<Customer*>::iterator it = customersList.begin(); it != customersList.end(); ++it){
        ordersID = (*it)->order(menu);
        string cName=(*it)->getName();
        for (vector<int >::iterator it2 = ordersID.begin(); it2 != ordersID.end(); ++it2) {
            for (vector<Dish>::const_iterator it3 = menu.begin(); it3 != menu.end(); ++it3)
                if (*it2 == it3->getId()) {
                    orderList.push_back(OrderPair((*it)->getId(), *it3));
                    cout << cName + " ordered " + it3->getName() + "\n";
                }
        }
    }
};

void Table::openTable() {
    open = true;
}

void Table::closeTable() {
    open = false;
    for (vector<Customer *>::iterator it = customersList.begin(); it != customersList.end(); ++it) {
            delete(*it);
    }
    customersList.clear();
    orderList.clear();
}

int Table::getBill() {
    int totalBill = 0;
    for (vector<OrderPair>::iterator it = orderList.begin(); it != orderList.end(); ++it)
        totalBill = (it->second).getPrice() + totalBill;
    return totalBill;
}
//
bool Table::isOpen() {
    return open;
}

////----------Rule of five------------//

//Destructor
Table::~Table() {
    clear();
}

////clear
void Table::clear() {
    vector<Customer*>::iterator it;
    for ( it = customersList.begin(); it != customersList.end();it++){
        if (*it != nullptr) {
            delete *it;
        }
    }
    customersList.clear();
    orderList.clear();
}

////copy
void Table::copy(Table &other) {
    this->capacity = other.capacity;
    this->open = other.open;
    for (vector<OrderPair>::iterator it = other.orderList.begin() ; it != other.orderList.end(); it++ ) {
        this->orderList.push_back(OrderPair(it->first, it->second));
    }
    for (vector<Customer*>::iterator it = other.customersList.begin() ; it != other.customersList.end(); it++ ) {
        customersList.push_back((*it)->clone());
    }
}

////Copy Constructor
Table::Table(Table &other): capacity(0), open(false), customersList(), orderList() {

    copy(other);
}

////Move constructor
Table::Table(Table &&other):  capacity(0), open(false), customersList(), orderList() {
    copy(other);
    other.clear();
}

////Copy Assignment Operator
Table& Table::operator=(Table &other) {
    if (this == &other)
        return *this;
    clear();
    copy(other);
    return *this;
}

////Move Assignment Operator
Table& Table::operator=(Table &&other) {
    if (this != &other) {
        clear();
        copy(other);
        other.clear();
    }
    return *this;
}

void Table::addOrders(std::vector<OrderPair> cOrders) {
    for(vector<OrderPair>:: iterator it=cOrders.begin(); it!=cOrders.end(); it++)
        orderList.push_back(*it);
}















