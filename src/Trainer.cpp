//
// Created by spl211 on 09/11/2021.
//

#include <vector>
#include <iostream>
#include <algorithm>
#include "Trainer.h"
#include "Customer.h"
#include "Workout.h"

Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false), customersList(), orderList(), salary(0) {}

Trainer::Trainer(const Trainer &other) :
        capacity(other.getCapacity()),
        open(other.open),
        customersList(),
        orderList(other.orderList),
        salary(other.salary)
{
    copyCustomer(other.customersList);
}

// Move constructor
Trainer::Trainer(Trainer&& other) :
        capacity(other.getCapacity()),
        open(other.open),
        customersList(),
        orderList(other.orderList),
        salary(other.salary)
{
    for(Customer *customer : other.customersList) {
        customersList.push_back(customer);
    }
    other.customersList.clear();
}

// Copy assignment operator
Trainer& Trainer::operator=(const Trainer& other) {
    if(this != &other){
        capacity = other.getCapacity();
        open = other.open;
        freeCustomers();
        customersList.clear();
        copyCustomer(other.customersList);
        orderList.clear();
        for(OrderPair order:other.orderList){
            orderList.push_back(OrderPair(order.first, order.second));
        }
        salary = other.salary;
    }
    return *this;
}

// Move assignment operator
Trainer& Trainer::operator=(Trainer&& other) {
    capacity = other.getCapacity();
    open = other.open;
    freeCustomers();
    customersList.clear();
    for(Customer *customer : other.customersList) {
        customersList.push_back(customer);
    }
    other.customersList.clear();
    orderList.clear();
    for(OrderPair order:other.orderList){
        orderList.push_back(OrderPair(order.first, order.second));
    }
    salary = other.salary;
    return *this;
}



void Trainer::copyCustomer (std::vector<Customer*> otherCustomersList) {
    for (Customer *customer:otherCustomersList) {
        if (typeid(*customer) == typeid(SweatyCustomer)) {
            customersList.push_back(new SweatyCustomer(*dynamic_cast<SweatyCustomer *>(customer)));
        }
        else if (typeid(*customer) == typeid(FullBodyCustomer)) {
            customersList.push_back(new FullBodyCustomer(*dynamic_cast<FullBodyCustomer *>(customer)));
        }
        else if (typeid(*customer) == typeid(CheapCustomer)) {
            customersList.push_back(new CheapCustomer(*dynamic_cast<CheapCustomer *>(customer)));
        }
        else {
            customersList.push_back(new HeavyMuscleCustomer(*dynamic_cast<HeavyMuscleCustomer *>(customer)));
        }
    }
}

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    std::vector<Workout> workouts(workout_options);
    for (int workout_id : workout_ids) {
        orderList.push_back(OrderPair(customer_id, workout_options.at(workout_id)));
        salary += workout_options.at(workout_id).getPrice();
    }
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

Customer *Trainer::getCustomer(int id) {
    for (Customer *customer: customersList) {
        if (customer->getId() == id) {
            return customer;
        }
    }
    return nullptr;
}

void Trainer::removeCustomer(int id) {
    // Remove the customer's orders
    std::vector<OrderPair> new_orderList;
    for(OrderPair &cur:orderList){
        if (cur.first == id) {
            cur.first = -1;
            salary -= cur.second.getPrice();
        }
    }
    // Remove the customer
    customersList.erase(std::remove(customersList.begin(), customersList.end(), getCustomer(id)));
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    orderList.clear();
    freeCustomers();
    customersList.clear();
    open = false;
    std::cout << "Salary " << std::to_string(getSalary()) << "NIS" << std::endl;
}

bool Trainer::isOpen() {
    return open;
}

int Trainer::getSalary() {
    return salary;
}

std::string Trainer::toString() const {
    std::string ret = "status: ";
    ret += open ? "open" : "closed";
    if (open) {
        ret += "\nCustomers:";

        for (const Customer* customer: customersList) {
            ret += "\n";
            ret += customer->toString();
        }

        ret += "\nOrders:";
        for (const OrderPair &order : orderList) {
            if (order.first == -1) {
                continue;
            }
            ret += "\n";
            ret += order.second.getName();
            ret += " " + std::to_string(order.second.getPrice()) + "NIS";
            ret += " " + std::to_string(order.first);
        }
        ret += "\nCurrent Trainer's Salary: ";
        ret += std::to_string(salary);
        ret += "NIS";
    }

    return ret;
}

void Trainer::freeCustomers() {
    for (Customer *customer: customersList) {
        delete customer;
    }
}

Trainer::~Trainer() {
    freeCustomers();
}

