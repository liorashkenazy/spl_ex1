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

std::vector<Workout>& Studio::getWorkoutOptions() { return workout_options; }

void Studio::addWorkoutFromConfig(const std::string &workout_info, int workout_id)
{
    size_t first_delim_pos(workout_info.find(','));
    size_t second_delim_pos(workout_info.find(',', first_delim_pos+1));
    std::string workout_name(workout_info.substr(0, first_delim_pos));
    std::string workout_type(workout_info.substr(first_delim_pos + 2, second_delim_pos - (first_delim_pos + 2)));
    WorkoutType w_type;

    if (workout_type == ANAEROBIC_TYPE_STR) {
        w_type = ANAEROBIC;
    } else if (workout_type == MIXED_TYPE_STR) {
        w_type = MIXED;
    } else {
        w_type = CARDIO;
    }

    workout_options.emplace_back(Workout(workout_id,
                                         workout_name,
                                         std::stoi(workout_info.substr(second_delim_pos + 2)),
                                         w_type));
}

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
void Studio::getAll()
{
    PrintWorkoutOptions print_action;
    std::cout << "Number of trainers: " << trainers.size() << std::endl;
    for (size_t i = 0; i < trainers.size(); i++) {
        std::cout << "Trainer[" << i << "], capacity: " << trainers.at(i)->getCapacity() << std::endl;
    }

    print_action.act(*this);
}

int Studio::getCurrentCustomerId() const {
    return next_customer_id;
}
