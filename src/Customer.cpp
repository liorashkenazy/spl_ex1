//
// Created by niv on 11/20/21.
//

#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    return std::vector<int>();
}

SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}

std::string SweatyCustomer::toString() const {
    return "Sweaty Customer";
}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    return std::vector<int>();
}

std::string CheapCustomer::toString() const {
    return std::__cxx11::string();
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    return std::vector<int>();
}

std::string HeavyMuscleCustomer::toString() const {
    return std::__cxx11::string();
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    return std::vector<int>();
}

std::string FullBodyCustomer::toString() const {
    return std::__cxx11::string();
}
