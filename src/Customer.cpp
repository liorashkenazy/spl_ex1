//
// Created by niv on 11/20/21.
//
#include <vector>
#include <algorithm>
#include <iostream>
#include "Customer.h"
#include "../include/Customer.h"

//#include "Workout.h"

Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> orders;

    for (const Workout &workout:workout_options) {
        if (workout.getType() == CARDIO) {
            orders.push_back(workout.getId());
        }
    }
    return orders;
}

std::string SweatyCustomer::toString() const {
    return "Sweaty Customer";
}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {}

bool compareCheapPrice(const Workout &a, const Workout &b)
{
    return a.getPrice() <= b.getPrice();
}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
     std::vector<int> orders;
     auto cheapest = std::min_element(workout_options.begin(), workout_options.end(), compareCheapPrice);
     orders.push_back(cheapest->getId());
     return orders;
}

std::string CheapCustomer::toString() const {
    return std::__cxx11::string();
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {}

bool compareMostExpensiveOption(std::pair<int, int> a, std::pair<int, int> b)
{
    return a.second > b.second;
}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> orders;
    std::vector<std::pair<int, int>> anaerobic_options;
    for (const Workout &workout : workout_options) {
        if (workout.getType() == ANAEROBIC) {
            anaerobic_options.push_back(std::pair<int,int>(workout.getId(), workout.getPrice()));
        }
    }

    std::sort(anaerobic_options.begin(), anaerobic_options.end(), compareMostExpensiveOption);
    for (auto opt : anaerobic_options) {
        orders.push_back(opt.first);
    }

    return orders;
}

std::string HeavyMuscleCustomer::toString() const {
    return std::__cxx11::string();
}

bool compareCheapestOption(std::pair<int, int> a, std::pair<int, int> b)
{
    return !compareMostExpensiveOption(a,b);
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> orders;
    std::vector<std::pair<int, int>> cardio_options;
    std::vector<std::pair<int, int>> mix_options;
    std::vector<std::pair<int, int>> anaerobic_options;
    for (const Workout &workout : workout_options) {
        if (workout.getType() == CARDIO) {
            cardio_options.push_back(std::pair<int,int>(workout.getId(), workout.getPrice()));
        }
        else if (workout.getType() == MIXED){
            mix_options.push_back(std::pair<int,int>(workout.getId(), workout.getPrice()));
        }
        else if (workout.getType() == ANAEROBIC){
            anaerobic_options.push_back(std::pair<int,int>(workout.getId(), workout.getPrice()));
        }
    }
    if (!cardio_options.empty()){
        orders.push_back(std::min_element(cardio_options.begin(), cardio_options.end(), compareCheapestOption)->first);
    }
    if (!mix_options.empty()){
        orders.push_back(std::max_element(mix_options.begin(), mix_options.end(), compareCheapestOption)->first);
    }
    if (!anaerobic_options.empty()){
        orders.push_back(std::min_element(anaerobic_options.begin(), anaerobic_options.end(), compareCheapestOption)->first);
    }
    return orders;

}

std::string FullBodyCustomer::toString() const {
    return std::__cxx11::string();
}
