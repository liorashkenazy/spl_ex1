//
// Created by niv on 11/19/21.
//
#include <iostream>
#include "Studio.h"
#include "Workout.h"
#include "Action.h"
#include "../include/Action.h"

BaseAction::BaseAction() {

}

PrintWorkoutOptions::PrintWorkoutOptions() : BaseAction() {

}

void BaseAction::complete() {
    status = COMPLETED;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void PrintWorkoutOptions::act(Studio& studio) {
    for (const Workout& opt : studio.getWorkoutOptions()) {
        std::cout << opt.toString() << std::endl;
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    return "Print workout options";
}

OpenTrainer *OpenTrainer::createOpenTrainerAction(const std::string &data, int next_customer_id) {
    std::vector<Customer *> customers;
    size_t current_customer_index(0);
    int trainer_id = std::stoi(data, &current_customer_index);
    int current_id = next_customer_id;

    while (current_customer_index != std::string::npos) {
        // Skip the space
        current_customer_index++;

        size_t next_delim_index = data.find(',', current_customer_index);
        std::string c_name = data.substr(current_customer_index, next_delim_index - current_customer_index);

        current_customer_index = data.find(' ', next_delim_index);
        std::string c_type = data.substr(next_delim_index + 1, current_customer_index - (next_delim_index + 1));

        if (c_type == CUSTOMER_TYPE_CHEAP_STR) {
            customers.emplace_back(new CheapCustomer(c_name, current_id));
        }
        else if (c_type == CUSTOMER_TYPE_FULL_BODY_STR) {
            customers.emplace_back(new FullBodyCustomer(c_name, current_id));
        }
        else if (c_type == CUSTOMER_TYPE_SWEATY_STR) {
            customers.emplace_back(new SweatyCustomer(c_name, current_id));
        }
        else if (c_type == CUSTOMER_TYPE_HEAVY_MUSCLE_STR) {
            customers.emplace_back(new HeavyMuscleCustomer(c_name, current_id));
        }

        current_id++;
    };

    return new OpenTrainer(trainer_id, customers);
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(customersList) {}

void OpenTrainer::act(Studio &studio) {}

std::string OpenTrainer::toString() const { return "Heyyy"; }