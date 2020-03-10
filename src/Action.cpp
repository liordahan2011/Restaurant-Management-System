#include "../include/Table.h"
#include "../include/Action.h"
#include "../include/Restaurant.h"
using namespace std;
extern Restaurant* backup;

BaseAction::BaseAction():errorMsg(),status(PENDING){}

BaseAction::~BaseAction() {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::printError() {
    if(getStatus()==ERROR)
        cout<< "Error: " + getErrorMsg()<<endl;
}

void BaseAction::copyStatus(const BaseAction &other) {
    if (other.getStatus() == ERROR)
        error(other.getErrorMsg());
    if (other.getStatus() == COMPLETED)
        complete();
}


OpenTable::OpenTable(int id, std::vector<Customer *> &customersList): tableId(id),customers(customersList) {};

OpenTable::~OpenTable() {
    for (Customer* c : customers)
        delete(c);
    customers.clear();
}

OpenTable::OpenTable(OpenTable &other): tableId(other.tableId), customers() {
    for (Customer* c : other.customers)
        customers.push_back(c->clone());
    if (other.getStatus() == COMPLETED)
        complete();
    else if (other.getStatus() == ERROR)
        error(other.getErrorMsg());
};

void OpenTable::act(Restaurant &restaurant) {
    Table* table = restaurant.getTable(tableId);
    if (table == nullptr || table->isOpen() == true)
        error("Table does not exist or is already open");
    else if (table->getCapacity() < (int)customers.size())
        error("There are no available seats for all customers");
    else {
        table->openTable();
        for (vector<Customer *>::const_iterator it = customers.begin(); it != customers.end(); it++)
            table->addCustomer((*it)->clone());
        complete();
    }
}

std::string OpenTable::toString() const {
    string toString= "open " + to_string(tableId);
    toString = toString + " ";
    for (vector<Customer*>::const_iterator it = customers.begin(); it != customers.end(); it++)
        toString = toString + (*it)->toString() + " ";
    if (getStatus() == ERROR)
        toString = toString + "Error: " + getErrorMsg();
    if (getStatus() == COMPLETED)
        toString = toString + "Completed";
    return toString;
}

OpenTable* OpenTable::clone() {
    vector<Customer*> newCustomers;
    for (Customer* c : customers)
        newCustomers.push_back(c->clone());
    OpenTable* ret = new OpenTable(tableId,newCustomers);
    ret->copyStatus(*this);
    return ret;
}

Order::Order(int id):tableId(id){}

void Order::act(Restaurant &restaurant) {
    Table* table = restaurant.getTable(tableId);
    if (table == nullptr || table->isOpen() == false)
        error("Table does not exist or is not open");
    else {
        table->order(restaurant.getMenu());
        complete();
    }
}

std::string Order::toString() const {
    string toString = "order " + to_string(tableId);
    if (getStatus()==ERROR)
        toString = toString + " Error: " + getErrorMsg();
    if (getStatus()==COMPLETED)
        toString = toString + " Completed";
    return toString;
}

Order *Order::clone() {
    Order* ret = new Order(this->tableId);
    ret->copyStatus(*this);
    return ret;
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTable(src),dstTable(dst), id(customerId){}

void MoveCustomer::act(Restaurant &restaurant) {
    Table* src = restaurant.getTable(srcTable);
    Table* dst = restaurant.getTable(dstTable);
    int dstCusSize = dst->getCustomers().size();
    if (src == nullptr || src->isOpen() == false || dst == nullptr || dst->isOpen() == false ||
        src->getCustomer(id) == nullptr || dst->getCapacity() == dstCusSize )
        error("Cannot move Customer");

    else {
        Customer* c1 = restaurant.getTable(srcTable)->getCustomer(id);
        vector<OrderPair> cOrders;
        for (vector<OrderPair>::iterator it = src->getOrders().begin(); it != src->getOrders().end(); it++)
            if ((*it).first == id)
                cOrders.push_back(*it);

        src->removeCustomer(id);

        //close table if its empty after we removed a customer
        if (src->getCustomers().size() == 0)
            src->closeTable();

        //add the customer to the dest table
        dst->addCustomer(c1);
        dst->addOrders(cOrders);
        complete();
    }
}

std::string MoveCustomer::toString() const {
    string toString = "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id);
    if (getStatus() == ERROR)
        toString = toString + " Error:" + getErrorMsg();
    if (getStatus() == COMPLETED)
        toString = toString + " Completed";
    return toString;
}

MoveCustomer *MoveCustomer::clone() {
    MoveCustomer* ret = new MoveCustomer(this->srcTable,this->dstTable,this->id);
    ret->copyStatus(*this);
    return ret;
}

Close::Close(int id):tableId(id) {}

void Close::act(Restaurant &restaurant) {
    Table* table = restaurant.getTable(tableId);
    if (table == nullptr || table->isOpen() == false)
        error("Table does not exist or is not open");
    else{
        cout<< "Table " + to_string(tableId) << " was closed. Bill " + to_string(table->getBill()) << "NIS"<<endl;
        table->closeTable();
        complete();
    }
}

std::string Close::toString() const {
    string toString = "close " + to_string(tableId);
    if (getStatus() == ERROR)
        toString = toString + " Error: " + getErrorMsg();
    if (getStatus() == COMPLETED)
        toString = toString + " Completed";
    return toString;
}

Close *Close::clone() {
    Close* ret = new Close(this->tableId);
    ret->copyStatus(*this);
    return ret;
}

CloseAll::CloseAll() {}

void CloseAll::act(Restaurant &restaurant) {
    for(int i=0; i<restaurant.getNumOfTables()-1; i++) {
        if (restaurant.getTable(i)->isOpen() == true)
            cout << "Table " + to_string(i) + " was closed. Bill " + to_string(restaurant.getTable(i)->getBill()) +
                    "NIS" << endl;
        restaurant.getTable(i)->closeTable();
    }
    complete();
}

std::string CloseAll::toString() const {
    return "closeall Completed";
}

CloseAll *CloseAll::clone() {
    CloseAll* ret = new CloseAll();
    ret->copyStatus(*this);
    return ret;
}

PrintMenu::PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant) {
    for (vector<Dish>::iterator it = restaurant.getMenu().begin(); it != restaurant.getMenu().end(); it++){
        cout << (*it).getName();
        if ((*it).getType()==VEG) cout<<" VEG ";
        else if((*it).getType()==SPC) cout<<" SPC ";
        else if((*it).getType()==BVG) cout<<" BVG ";
        else cout<<" ALC ";

        cout<<to_string((*it).getPrice()) + "NIS" <<endl;
    }
    complete();
}

std::string PrintMenu::toString() const {
    return "menu Completed";
}

PrintMenu *PrintMenu::clone() {
    PrintMenu* ret = new PrintMenu();
    ret->copyStatus(*this);
    return ret;
}

PrintTableStatus::PrintTableStatus(int id):tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table* table = restaurant.getTable(tableId);
    if (table->isOpen()){
        cout<< "Table " + to_string(tableId) + " status: open"<<endl;
        cout<< "Customers:" <<endl;
        for (Customer* c : table->getCustomers())
            cout<< to_string(c->getId()) + " " << c->getName()<<endl;
        cout<< "Orders:" <<endl;
        for (OrderPair ord : table->getOrders())
            cout<< ord.second.getName() + " "<< to_string(ord.second.getPrice()) + "NIS "<< to_string(ord.first)<<endl;
        cout<<"Current Bill: " + to_string(table->getBill()) +"NIS"<<endl;
    } else
        cout<< "Table " + to_string(tableId) + " status: closed"<<endl;
    complete();
}

std::string PrintTableStatus::toString() const {
    return "status " + to_string(tableId) + " Completed";
}

PrintTableStatus *PrintTableStatus::clone() {
    PrintTableStatus* ret = new PrintTableStatus(this->tableId);
    ret->copyStatus(*this);
    return ret;
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Restaurant &restaurant) {
    for (BaseAction* action : restaurant.getActionsLog())
        cout<<action->toString()<<endl;
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log Completed";
}

PrintActionsLog *PrintActionsLog::clone() {
    PrintActionsLog* ret = new PrintActionsLog();
    ret->copyStatus(*this);
    return ret;
}

BackupRestaurant::BackupRestaurant() {}

void BackupRestaurant::act(Restaurant &restaurant) {
    delete(backup);
    backup = new Restaurant(restaurant);
    complete();
}

std::string BackupRestaurant::toString() const {
    return "backup Completed";
}

BackupRestaurant *BackupRestaurant::clone() {
    BackupRestaurant* ret = new BackupRestaurant();
    ret->copyStatus(*this);
    return ret;
}

RestoreResturant::RestoreResturant() {}

void RestoreResturant::act(Restaurant &restaurant) {
    if (backup == nullptr)
        error("No backup available");
    else {
        restaurant = *backup;
        complete();
    }
}

std::string RestoreResturant::toString() const {
    string toString = "restore";
    if (getStatus() == COMPLETED)
        toString = toString + " Completed";
    if (getStatus() == ERROR)
        toString = toString + " Error: " + getErrorMsg();
    return toString;
}

RestoreResturant *RestoreResturant::clone() {
    RestoreResturant* ret = new RestoreResturant();
    ret->copyStatus(*this);
    return ret;
}
