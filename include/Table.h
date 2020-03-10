#ifndef TABLE_H_
#define TABLE_H_
#ifndef RULEOFFIVE_H
#define RULEOFFIVE_H

#include <vector>
#include "../include/Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    //------Rule of five-------//
    virtual ~Table();  //Destructor
    void clear();  //clear
    void copy(Table& other);  //Copy
    Table(Table& other);  //Copy Constructor
    Table(Table &&other);  //Move constructor
    Table& operator=(Table &other);  //Copy Assignment Operator
    Table& operator=(Table &&other);  // Move Assignment Operator

    int getCapacity() const;
    void addCustomer(Customer* customer);
   void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void addOrders(std::vector <OrderPair> cOrders);
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif
#endif