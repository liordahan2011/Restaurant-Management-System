
#include "../include/Restaurant.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

Restaurant::Restaurant():open(false), tables(), menu(), actionsLog(){};

Restaurant::Restaurant(const std::string &configFilePath):open(false), tables(), menu(), actionsLog() {
    string line;
    string token;
    ifstream myfile;
    myfile.open(configFilePath, ios::in);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            //create tables due to the given capacity
            if ((int)line.find("#tables description") != -1) {
                getline(myfile, line);
                if ((int)line.front() != '#' && line.size() != 0) {
                    istringstream tokenStream(line);
                    while (getline(tokenStream, token, ','))
                        tables.push_back(new Table(stoi(token)));
                }
            }
            //creates the menu due to the given dishes
            if ((int)line.find("#Menu") != -1) {
                int i = 0;
                string name;
                int price;
                DishType type = VEG;
                while (getline(myfile, line)) {
                    if ((int)line.front() != '#' && line.size() != 0) {
                        istringstream tokenStream(line);
                        (getline(tokenStream, token, ',')); // first string -> name of dish
                        name = token;
                        (getline(tokenStream, token, ',')); // second string -> type of dish
                        if (token == "VEG")
                            type = VEG;
                        if (token == "SPC")
                            type = SPC;
                        if (token == "BVG")
                            type = BVG;
                        if (token == "ALC")
                            type = ALC;
                        (getline(tokenStream, token, ',')); // third string -> price
                        price = stoi(token);
                        menu.push_back(Dish(i, name, price, type));
                        i++;
                    }
                }
            }
        }
        myfile.close();
    }
};


void Restaurant::start() {
    open = true;
    cout << "Restaurant is now open!" << endl;
    string token;
    int Tid;
    string line;
    int customerId=0;
    while (getline(cin, line)) {
        istringstream tokenStream(line);
        getline(tokenStream, token, ' ');
        //openTable
        if (token == "open") {
            getline(tokenStream, token, ' ');
            vector<Customer *> customers;
            Tid = stoi(token);
            while (getline(tokenStream, token, ' ')|| getline(tokenStream, token)) {
                string tmp =token;
                Customer* c = createCustomer(tmp, customerId);
                customers.push_back(c);
                customerId++;
            }
            OpenTable *open = new OpenTable(Tid, customers);
            open->act(*this);
            actionsLog.push_back(open);
            open->printError();
            //Order
        } else if (token == "order") {
            getline(tokenStream, token);
            int id = stoi(token);
            BaseAction *order = new Order(id);
            order->act(*this);
            actionsLog.push_back(order);
            order->printError();
            //MoveCustomer
        } else if (token == "move") {
            getline(tokenStream, token, ' ');
            int src = stoi(token);
            getline(tokenStream, token, ' ');
            int dst = stoi(token);
            getline(tokenStream, token);
            int id = stoi(token);
            BaseAction *move = new MoveCustomer(src, dst, id);
            move->act(*this);
            actionsLog.push_back(move);
            move->printError();
            //Close
        } else if (token == "close") {
            getline(tokenStream, token);
            BaseAction *close = new Close(stoi(token));
            close->act(*this);
            actionsLog.push_back(close);
            close->printError();
            //CloseAll
        } else if (token == "closeall") {
            BaseAction *closeall = new CloseAll();
            closeall->act(*this);
            actionsLog.push_back(closeall);
            open = false;
            break;
        } else if (token == "menu") {
            BaseAction *printMenu = new PrintMenu();
            printMenu->act(*this);
            actionsLog.push_back(printMenu);
        } else if (token == "status") {
            getline(tokenStream, token);
            BaseAction *status = new PrintTableStatus(stoi(token));
            status->act(*this);
            actionsLog.push_back(status);
        } else if (token == "log") {
            BaseAction *log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
        } else if (token == "backup") {
            BaseAction *backup = new BackupRestaurant();
            backup->act(*this);
            actionsLog.push_back(backup);
        } else if (token == "restore") {
            BaseAction *restore = new RestoreResturant();
            restore->act(*this);
            actionsLog.push_back(restore);
            restore->printError();
        }
    }
}

Customer* Restaurant:: createCustomer (string curr, int id) {
    string token;
    istringstream tokenStream(curr);
    string name;
    getline(tokenStream, token, ',');
    name = token;
    if (getline(tokenStream, token, ' ') || getline(tokenStream, token)) {
        if (token == "veg") {
            Customer *curr = new VegetarianCustomer(name, id);
            return curr;
        }
        if (token == "spc") {
            Customer *curr = new SpicyCustomer(name, id);
            return curr;
        }
        if (token == "chp") {
            Customer *curr = new CheapCustomer(name, id);
            return curr;
        }
        if (token == "alc") {
            Customer *curr = new AlchoholicCustomer(name, id);
            return curr;
        }
    }
    return nullptr;
}

int Restaurant::getNumOfTables() const {
    return tables.size();
};

Table* Restaurant::getTable(int ind) {
    if (ind>=(int)tables.size())
        return nullptr;
    return tables[ind];
};

std::vector<Dish> &Restaurant::getMenu() {
    return menu;
};

const std::vector<BaseAction *> &Restaurant::getActionsLog() const {
    return actionsLog;
}

void Restaurant::setOpen(bool status) {
    open = status;
}

////----------------RULE OF FIVE --------------/////

Restaurant::~Restaurant() {
    clear();
}

void Restaurant::clear() {
    menu.clear();
    for (Table* table : tables)
        delete(table);
    tables.clear();
    for (BaseAction* action : actionsLog)
        delete (action);
    actionsLog.clear();
}

void Restaurant::copy(Restaurant &other) {
    open = other.open;
    for (Dish d : other.menu)
        menu.push_back(Dish(d.getId(),d.getName(),d.getPrice(),d.getType()));

    for (Table* oTable : other.tables)
        tables.push_back(new Table(*oTable));

    for (BaseAction* action : other.actionsLog)
        actionsLog.push_back(action->clone());
}

Restaurant::Restaurant(Restaurant &other):open(false), tables(), menu(), actionsLog() {
    copy(other);
}

Restaurant::Restaurant(Restaurant &&other):open(false), tables(), menu(), actionsLog() {
    copy(other);
    other.clear();
}

Restaurant &Restaurant::operator=(Restaurant &other) {
    if (this != &other){
        clear();
        copy(other);
    }
    return *this;
}

Restaurant &Restaurant::operator=(Restaurant &&other) {
    if (this != &other){
        clear();
        copy(other);
        other.clear();
    }
    return *this;
}

