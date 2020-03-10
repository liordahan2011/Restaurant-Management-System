#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "../include/Dish.h"
#include "../include/Table.h"
#include "../include/Action.h"


class Restaurant{
public:
    ////---------Rule Of Five----------//
    ~Restaurant();
    Restaurant(Restaurant& other);
    Restaurant(Restaurant&& other);
    Restaurant& operator=(Restaurant&);
    Restaurant& operator=(Restaurant&&);
    void clear();
    void copy(Restaurant& other);

    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    Customer* createCustomer (std::string curr, int id);
    void setOpen (bool status);
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif
