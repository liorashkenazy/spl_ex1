//
// Created by spl211 on 09/11/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Studio.h"
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

using namespace std;

// Constructor
Studio::Studio(const std::string &configFilePath):
        open(false), trainers(), workout_options(), actionsLog(), next_customer_id(0)
{
    bool num_trainers_read(false);
    bool capacities_read(false);
    int num_trainers(0);
    std::ifstream config_file;
    std::string cur_line;
    int workout_id(0);
    config_file.open(configFilePath);

    while (std::getline(config_file, cur_line)) {
        if (cur_line.length() == 0 || cur_line.at(0) == '#') {
            continue;
        }
        if (!num_trainers_read) {
            num_trainers = std::stoi(cur_line);
            num_trainers_read = true;
        }
        else if (!capacities_read) {
            size_t next_pos(0);
            size_t current_pos(0);
            for (int i = 0; i < num_trainers; i++) {
                // Read the current capacity, and extract the position of the next capacity
                trainers.push_back(new Trainer(std::stoi(cur_line.substr(current_pos), &next_pos)));
                // Skip over the ','
                current_pos += next_pos + 1;
            }
            capacities_read = true;
        }
        else {
            addWorkoutFromConfig(cur_line, workout_id);
            workout_id++;
        }
    }
}

// Copy constructor
Studio::Studio(const Studio &other) :
        open(other.open),
        trainers(),
        workout_options(other.workout_options),
        actionsLog(),
        next_customer_id(other.getCurrentCustomerId())
{
    for (Trainer *trainer:other.trainers) {
        trainers.push_back(new Trainer(*trainer));
    }
    copyActionsLog(other);
}

// Move constructor
Studio::Studio(Studio &&other) :
        open(other.open),
        trainers(),
        workout_options(other.workout_options),
        actionsLog(),
        next_customer_id(other.getCurrentCustomerId())
{
    for (Trainer *trainer:other.trainers) {
        trainers.push_back(trainer);
    }
    other.trainers.clear();
    for (BaseAction *action : other.actionsLog) {
        actionsLog.push_back(action);
    }
    other.actionsLog.clear();
}

// Copy assignment operator
Studio &Studio::operator=(const Studio& other) {
    if (this != &other) {
        clear();
        open = other.open;
        workout_options.clear();
        for (const Workout &workout:other.workout_options) {
            workout_options.push_back(workout);
        }
        next_customer_id = other.next_customer_id;
        for (Trainer *trainer:other.trainers) {
            trainers.push_back(new Trainer(*trainer));
        }
        copyActionsLog(other);
    }
    return *this;
}

// Move assignment operator
Studio &Studio::operator=(Studio &&other) {
    clear();
    open = other.open;
    workout_options.clear();
    for (const Workout &workout:other.workout_options) {
        workout_options.push_back(workout);
    }
    next_customer_id = other.next_customer_id;
    for (Trainer *trainer:other.trainers) {
        trainers.push_back(trainer);
    }
    other.trainers.clear();
    for (BaseAction *action : other.actionsLog) {
        actionsLog.push_back(action);
    }
    other.actionsLog.clear();
    return *this;
}

std::vector<Workout>& Studio::getWorkoutOptions() { return workout_options; }

void Studio::addWorkoutFromConfig(const std::string &workout_info, int workout_id)
{
    size_t first_delim_pos(workout_info.find(','));
    size_t second_delim_pos(workout_info.find(',', first_delim_pos+1));
    std::string workout_name(workout_info.substr(0, first_delim_pos));
    std::string workout_type(workout_info.substr(first_delim_pos + 2, second_delim_pos - (first_delim_pos + 2)));
    WorkoutType w_type;

    if (workout_type == Workout::typeToTypeStr[ANAEROBIC - ANAEROBIC]) {
        w_type = ANAEROBIC;
    } else if (workout_type == Workout::typeToTypeStr[MIXED - ANAEROBIC]) {
        w_type = MIXED;
    } else {
        w_type = CARDIO;
    }

    workout_options.emplace_back(Workout(workout_id,
                                         workout_name,
                                         std::stoi(workout_info.substr(second_delim_pos + 2)),
                                         w_type));
}

int Studio::getCurrentCustomerId() const {
    return next_customer_id;
}
void Studio::SetCurrentCustomerId(int increment_by) {
    next_customer_id += increment_by;
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    return trainers[tid];
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

void Studio::addActionToLog(BaseAction *action) {
    actionsLog.push_back(action);
}

BaseAction * Studio::parseAction(const std::string &action_str)
{
    std::string action_type = action_str.substr(0, action_str.find(' '));

    std::string data = action_str.find(' ') == string::npos ? "": action_str.substr(action_str.find(' ') + 1);
    if (action_type == OpenTrainer::name) {
        return OpenTrainer::createOpenTrainerAction(data, getCurrentCustomerId());
    }
    else if (action_type == Order::name) {
        return Order::createOrder(data);
    }
    else if (action_type == MoveCustomer::name) {
        return MoveCustomer::createMoveCustomer(data);
    }
    else if (action_type == Close::name) {
        return Close::createClose(data);
    }
    else if (action_type == CloseAll::name) {
        return new CloseAll();
    }
    else if (action_type == PrintWorkoutOptions::name) {
        return new PrintWorkoutOptions();
    }
    else if (action_type == PrintTrainerStatus::name) {
        return PrintTrainerStatus::createPrintTrainerStatus(data);
    }
    else if (action_type == PrintActionsLog::name) {
        return new PrintActionsLog();
    }
    else if (action_type == BackupStudio::name) {
        return new BackupStudio();
    }
    else if (action_type == RestoreStudio::name) {
        return new RestoreStudio();
    }
    return nullptr;
}

void Studio::start()
{
    open = true;
    cout << "Studio is now open!" << endl;

    while (open) {
        cout << "Please enter the next input" << endl;
        string current_input;
        getline(cin, current_input);
        BaseAction *next_action = parseAction(current_input);
        next_action->act(*this);
        addActionToLog(next_action);
    }
}

void Studio::clear() {
    for (Trainer *trainer: trainers) {
        delete trainer;
    }
    trainers.clear();
    for (BaseAction *action:actionsLog) {
        delete action;
    }
    actionsLog.clear();
}

void Studio::copyActionsLog(const Studio &other)
{
    for (BaseAction *action:other.getActionsLog()) {
        if (typeid(*action) == typeid(OpenTrainer)) {
            actionsLog.push_back(new OpenTrainer(*dynamic_cast<OpenTrainer *>(action)));
        }
        else if (typeid(*action) == typeid(Order)) {
            actionsLog.push_back(new Order(*dynamic_cast<Order *>(action)));
        }
        else if (typeid(*action) == typeid(MoveCustomer)) {
            actionsLog.push_back(new MoveCustomer(*dynamic_cast<MoveCustomer *>(action)));
        }
        else if (typeid(*action) == typeid(Close)) {
            actionsLog.push_back(new Close(*dynamic_cast<Close *>(action)));
        }
        else if (typeid(*action) == typeid(CloseAll)) {
            actionsLog.push_back(new CloseAll(*dynamic_cast<CloseAll *>(action)));
        }
        else if (typeid(*action) == typeid(PrintWorkoutOptions)) {
            actionsLog.push_back(new PrintWorkoutOptions(*dynamic_cast<PrintWorkoutOptions *>(action)));
        }
        else if (typeid(*action) == typeid(PrintTrainerStatus)) {
            actionsLog.push_back(new PrintTrainerStatus(*dynamic_cast<PrintTrainerStatus *>(action)));
        }
        else if (typeid(*action) == typeid(PrintActionsLog)) {
            actionsLog.push_back(new PrintActionsLog(*dynamic_cast<PrintActionsLog *>(action)));
        }
        else if (typeid(*action) == typeid(BackupStudio)) {
            actionsLog.push_back(new BackupStudio(*dynamic_cast<BackupStudio *>(action)));
        }
        else if (typeid(*action) == typeid(RestoreStudio)) {
            actionsLog.push_back(new RestoreStudio(*dynamic_cast<RestoreStudio *>(action)));
        }
    }
}

Studio::~Studio() {
    clear();
}

void Studio::close() {
    open = false;
}
