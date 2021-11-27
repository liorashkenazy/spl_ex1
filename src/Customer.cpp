//
// Created by niv on 11/20/21.
//
#include <vector>
#include <algorithm>
#include <iostream>
#include "Customer.h"


/************** Customer ***********/
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

Customer::~Customer() {}

/************** SweatyCustomer ***********/
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
    return std::to_string(getId()) + " " + getName();
}


/************** CheapCustomer ***********/
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {}

// A comparator that returns the cheapest workout
bool compareCheapPrice(const Workout &a, const Workout &b)
{
    // Explicitly choose the workout with the lower id before ones with higher ids if they are the same price.
    if (a.getPrice() == b.getPrice()) {
        return a.getId() < b.getId();
    }
    return a.getPrice() < b.getPrice();
}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
     std::vector<int> orders;
     auto cheapest = std::min_element(workout_options.begin(), workout_options.end(), compareCheapPrice);
     orders.push_back(cheapest->getId());
     return orders;
}

std::string CheapCustomer::toString() const {
    return std::to_string(getId()) + " " + getName();
}


/************** HeavyMuscleCustomer ***********/
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {}

// A comparator that returns the most expensive option
bool compareMostExpensiveOption(std::pair<int, int> a, std::pair<int, int> b)
{
    // Explicitly choose workout options with lower ids first, in case the price is the same
    if (a.second == b.second) {
        return a.first < b.first;
    }
    return a.second > b.second;
}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> orders;
    std::vector<std::pair<int, int>> anaerobic_options;
    for (const Workout &workout : workout_options) {
        if (workout.getType() == ANAEROBIC) {
            anaerobic_options.emplace_back(workout.getId(), workout.getPrice());
        }
    }

    std::sort(anaerobic_options.begin(), anaerobic_options.end(), compareMostExpensiveOption);
    for (auto opt : anaerobic_options) {
        orders.push_back(opt.first);
    }

    return orders;
}

std::string HeavyMuscleCustomer::toString() const {
    return std::to_string(getId()) + " " + getName();
}


/************** FullBodyCustomer ***********/
FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> orders;
    std::pair<int, int> cheapest_cardio(-1, 0);
    std::pair<int, int> most_expensive_mixed(-1, 0);
    std::pair<int, int> cheapest_anaerobic(-1, 0);

    for (const Workout &workout : workout_options) {
        if (workout.getType() == CARDIO) {
            if (cheapest_cardio.first == -1 || cheapest_cardio.second > workout.getPrice()) {
                cheapest_cardio.first = workout.getId();
                cheapest_cardio.second = workout.getPrice();
            }
        }
        else if (workout.getType() == MIXED) {
            if (most_expensive_mixed.first == -1 || most_expensive_mixed.second < workout.getPrice()) {
                most_expensive_mixed.first = workout.getId();
                most_expensive_mixed.second = workout.getPrice();
            }
        }
        else if (workout.getType() == ANAEROBIC) {
            if (cheapest_anaerobic.first == -1 || cheapest_anaerobic.second > workout.getPrice()) {
                cheapest_anaerobic.first = workout.getId();
                cheapest_anaerobic.second = workout.getPrice();
            }
        }
    }

    // Insert the appropriate option to the orders vector, if such option exist
    if (cheapest_cardio.first != -1) {
        orders.push_back(cheapest_cardio.first);
    }
    if (most_expensive_mixed.first != -1) {
        orders.push_back(most_expensive_mixed.first);
    }
    if (cheapest_anaerobic.first != -1) {
        orders.push_back(cheapest_anaerobic.first);
    }

    return orders;
}

std::string FullBodyCustomer::toString() const {
    return std::to_string(getId()) + " " + getName();
}
