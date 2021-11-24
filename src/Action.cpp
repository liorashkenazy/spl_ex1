//
// Created by niv on 11/19/21.
//
#include <iostream>
#include "Studio.h"
#include "Workout.h"
#include "Action.h"

extern Studio *backup;

/************** BaseAction ***********/
BaseAction::BaseAction() : errorMsg(), status(ERROR) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    this->errorMsg = errorMsg;
    std::cout << "Error: " << errorMsg << std::endl;
    status = ERROR;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}


/************** OpenTrainer ***********/
const std::string OpenTrainer::name = "open";

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) :
    trainerId(id),
    customers(customersList),
    action_args() {}

OpenTrainer *OpenTrainer::createOpenTrainerAction(const std::string &data, int next_customer_id) {
    std::vector<Customer *> customers;
    int trainer_id = std::stoi(data);
    int current_id = next_customer_id;
    size_t current_customer_index = data.find(' ');

    while (current_customer_index != std::string::npos && current_customer_index < data.size() - 1) {
        // Skip the space
        current_customer_index++;

        size_t next_delim_index = data.find(',', current_customer_index);
        std::string c_name = data.substr(current_customer_index, next_delim_index - current_customer_index);

        current_customer_index = data.find(' ', next_delim_index);
        std::string c_type = data.substr(next_delim_index + 1, current_customer_index - (next_delim_index + 1));

        if (c_type == CUSTOMER_TYPE_CHEAP_STR) {
            customers.push_back(new CheapCustomer(c_name, current_id));
        }
        else if (c_type == CUSTOMER_TYPE_FULL_BODY_STR) {
            customers.push_back(new FullBodyCustomer(c_name, current_id));
        }
        else if (c_type == CUSTOMER_TYPE_SWEATY_STR) {
            customers.push_back(new SweatyCustomer(c_name, current_id));
        }
        else if (c_type == CUSTOMER_TYPE_HEAVY_MUSCLE_STR) {
            customers.push_back(new HeavyMuscleCustomer(c_name, current_id));
        }

        current_id++;
    }

    OpenTrainer *new_action = new OpenTrainer(trainer_id, customers);
    new_action->action_args = data;
    return new_action;
}

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
        // Transfer ownership
        pTrainer->addCustomer(pCustomer);
        increment_by++;
    }
    customers.clear();

    pTrainer->openTrainer();
    studio.SetCurrentCustomerId(increment_by);
    complete();
}

std::string OpenTrainer::toString() const
{
    return name + " " + action_args + " " + (getStatus() == COMPLETED ? "completed" : "Error: " + getErrorMsg());
}

OpenTrainer::~OpenTrainer()
{
    for (Customer *customer:customers) {
        delete customer;
    }
}


/************** Order ***********/
const std::string Order::name = "order";

Order::Order(int id) : trainerId(id) {}

Order *Order::createOrder(const std::string &data) {
    return new Order(std::stoi(data));
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

std::string Order::toString() const
{
    return name + " " + std::to_string(trainerId) + " " +
           (getStatus() == COMPLETED ? "completed" : "Error: " + getErrorMsg());
}


/************** MoveCustomer ***********/
const std::string MoveCustomer::name = "move";

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {}

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
    return name + " " +
            std::to_string(srcTrainer) + " " +
            std::to_string(dstTrainer) + " " +
            std::to_string(id) + " " + (getStatus() == COMPLETED ? "completed" : "Error: " + getErrorMsg());
}


/************** Close ***********/
const std::string Close::name = "close";

Close::Close(int id) : trainerId(id) {}

Close *Close::createClose(const std::string& data) {
    return new Close(std::stoi(data));
}

void Close::act(Studio &studio) {
    if (studio.getNumOfTrainers() <= trainerId || trainerId < 0 || !studio.getTrainer(trainerId)->isOpen()) {
        error("Trainer does not exist or is not open");
        return;
    }

    std::cout << "Trainer " << std::to_string(trainerId) << " closed. ";
    studio.getTrainer(trainerId)->closeTrainer();
    complete();
}

std::string Close::toString() const {
    return name + " " + std::to_string(trainerId) + " " +
           (getStatus() == COMPLETED ? "completed" : "Error: " + getErrorMsg());
}


/************** CloseAll ***********/
const std::string CloseAll::name = "closeall";

CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio)
{
    for (int i = 0; i < studio.getNumOfTrainers(); i++) {
        Trainer *trainer = studio.getTrainer(i);
        if (trainer->isOpen()) {
            std::cout << "Trainer " << std::to_string(i) << " closed. ";
            trainer->closeTrainer();
        }
    }
    studio.close();
    complete();
}

std::string CloseAll::toString() const {
    return name + " completed";
}


/************** PrintWorkoutOptions ***********/
const std::string PrintWorkoutOptions::name = "workout_options";

PrintWorkoutOptions::PrintWorkoutOptions() : BaseAction() {}

void PrintWorkoutOptions::act(Studio& studio) {
    for (const Workout& opt : studio.getWorkoutOptions()) {
        std::cout << opt.toString() << std::endl;
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    return name + " completed";
}


/************** PrintTrainerStatus ***********/
const std::string PrintTrainerStatus::name = "status";

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {}

PrintTrainerStatus *PrintTrainerStatus::createPrintTrainerStatus(const std::string& data) {
    return new PrintTrainerStatus(std::stoi(data));
}

void PrintTrainerStatus::act(Studio &studio) {
    if (studio.getNumOfTrainers() > trainerId && trainerId >= 0) {
        std::cout << "Trainer " + std::to_string(trainerId) << " ";
        std::cout << studio.getTrainer(trainerId)->toString() << std::endl;
    }
    complete();
}

std::string PrintTrainerStatus::toString() const {
    return name + " " + std::to_string(trainerId) + " " +
           (getStatus() == COMPLETED ? "completed" : "Error: " + getErrorMsg());
}


/************** PrintActionsLog ***********/
const std::string PrintActionsLog::name = "log";

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio) {
    for (BaseAction *action : studio.getActionsLog()) {
        std::cout << action->toString() << std::endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return name + " completed";
}


/************** BackupStudio ***********/
const std::string BackupStudio::name = "backup";

BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio) {
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }

    backup = new Studio(studio);
    complete();
}

std::string BackupStudio::toString() const {
    return name + " completed";
}


/************** RestoreStudio ***********/
const std::string RestoreStudio::name = "restore";

RestoreStudio::RestoreStudio() {}

void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr) {
        error("No backup available");
        return;
    }
    studio = *backup;
    complete();
}

std::string RestoreStudio::toString() const {
    return name + (getStatus() == COMPLETED ? "completed" : "Error: " + getErrorMsg());
}
