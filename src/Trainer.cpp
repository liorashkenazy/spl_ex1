//
// Created by spl211 on 09/11/2021.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include "Trainer.h"
#include "Customer.h"
#include "Workout.h"

Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false), customersList(), orderList(), salary(0) {}

int Trainer::getCapacity() const{
    return capacity;
}

void
Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    std::vector<Workout> workouts(workout_options);
    for (int workout_id : workout_ids) {
        orderList.push_back(OrderPair(customer_id, workout_options.at(workout_id)));
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
        if(cur.first == id)
            cur.first = -1;
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
    open = false;
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
            ret += std::to_string(customer->getId());
            ret += " " + customer->getName();
        }

        ret += "\nOrders:";
        for (const OrderPair &order : orderList) {
            ret += "\n";
            ret += " " + order.second.getName();
            ret += " " + std::to_string(order.second.getPrice()) + "NIS";
            ret += " " + std::to_string(order.first);
        }
        ret += "\nCurrent Trainer's Salary: ";
        ret += std::to_string(salary);
        ret += "NIS";
    }

    return ret;
}
