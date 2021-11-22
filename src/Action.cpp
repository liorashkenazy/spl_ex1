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

void BaseAction::error(std::string errorMsg) {
    this->errorMsg = errorMsg;
    std::cout << "Error: " << errorMsg;
    status = ERROR;
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

void OpenTrainer::act(Studio &studio)
{
    int increment_by(0);
    Trainer *pTrainer;
    if (studio.getNumOfTrainers() <= trainerId || trainerId < 0 || studio.getTrainer(trainerId)->isOpen()) {
        error("Workout session does not exist or is already open.");
        return;
    }

    pTrainer = studio.getTrainer(trainerId);
    for (Customer *pCustomer: customers) {
        if (pTrainer->getCapacity() <= pTrainer->getCustomers().size()) {
            break;
        }
        pTrainer->addCustomer(pCustomer);
        increment_by++;
    }
    pTrainer->openTrainer();
    studio.SetCurrentCustomerId(increment_by);
    complete();
}

std::string OpenTrainer::toString() const { return "Heyyy"; }

Order *Order::createOrder(const std::string &data) {
    return new Order(std::stoi(data));
}

std::string Order::toString() const {
    return std::__cxx11::string();
}

void Order::act(Studio &studio) {
    if (studio.getNumOfTrainers() <= trainerId || trainerId < 0 || !studio.getTrainer(trainerId)->isOpen()) {
        error("Trainer does not exist or is not open.");
        return;
    }

    Trainer *pTrainer = studio.getTrainer(trainerId);
    const std::vector<Workout> &workout_options = studio.getWorkoutOptions();
    for (Customer *customer : pTrainer->getCustomers()) {
        std::vector<int> orders = customer->order(studio.getWorkoutOptions());
        pTrainer->order(customer->getId(), orders, workout_options);
        for (int order : orders) {
            std::cout << customer->getName() << " Is Doing " << workout_options.at(order).getName() << std::endl;
        }
    }
    complete();
}

Order::Order(int id) : trainerId(id) {}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio)
{
    if (studio.getNumOfTrainers() <= srcTrainer || srcTrainer < 0 || !studio.getTrainer(srcTrainer)->isOpen() ||
        studio.getNumOfTrainers() <= dstTrainer || dstTrainer < 0 || !studio.getTrainer(dstTrainer)->isOpen() ||
        studio.getTrainer(dstTrainer)->getCapacity() <= studio.getTrainer(dstTrainer)->getCustomers().size() ||
        studio.getTrainer(srcTrainer)->getCustomer(id) == nullptr) {
        error("Cannot move customer");
        return;
    }

    Trainer *pSrcTrainer = studio.getTrainer(srcTrainer);
    Trainer *pDstTrainer = studio.getTrainer(dstTrainer);
    Customer *pCustomer = pSrcTrainer->getCustomer(id);
    std::vector<int> orders;
    for (const OrderPair &order : pSrcTrainer->getOrders()) {
        if (order.first == id) {
            orders.push_back(order.second.getId());
        }
    }

    // Transfer ownership
    pSrcTrainer->removeCustomer(id);
    pDstTrainer->addCustomer(pCustomer);

    pDstTrainer->order(id, orders, studio.getWorkoutOptions());

    if (pSrcTrainer->getCustomers().size() == 0) {
        pSrcTrainer->closeTrainer();
    }

    complete();
}

std::string MoveCustomer::toString() const {
    return "Move Action";
}

MoveCustomer *MoveCustomer::createMoveCustomer(const std::string &data) {
    size_t next_pos(0);
    size_t current_pos(0);
    int source_tid = std::stoi(data, &next_pos);
    current_pos += next_pos + 1;
    int dest_tid = std::stoi(data.substr(current_pos), &next_pos);
    current_pos += next_pos + 1;
    int customer_id = std::stoi(data.substr(current_pos));

    return new MoveCustomer(source_tid, dest_tid, customer_id);
}
