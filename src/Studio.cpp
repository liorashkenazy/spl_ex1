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

void Studio::addWorkoutFromConfig(const std::string &workout_info, int workout_id)
{
    int first_delim_pos(workout_info.find(","));
    int second_delim_pos(workout_info.find(",", first_delim_pos));
    std::string workout_name(workout_info.substr(0, first_delim_pos));
    std::string workout_type(first_delim_pos + 2, second_delim_pos);
    WorkoutType w_type;
    int price(0);

    if (workout_type.compare("Anaeorbic") == 0) {
        w_type = ANAEROBIC;
    } else if (workout_type.compare("Mixed") == 0) {
        w_type = MIXED;
    } else {
        w_type = CARDIO;
    }

    price = std::stoi(workout_info.substr(second_delim_pos + 2));
    workout_options.push_back(Workout(workout_id, workout_name, price, w_type));
}

Studio::Studio(const std::string &configFilePath): open(false)
{
    bool num_trainers_read(false);
    bool capacities_read(false);
    int num_trainers(0);
    int current_delim_pos(0);
    std::fstream config_file;
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
            while (cur_line.find(",", current_delim_pos) != std::string::npos) {
                trainers.push_back(new Trainer(std::stoi(cur_line.substr(current_delim_pos,
                                                                            cur_line.find(",", current_delim_pos)))));
                current_delim_pos = cur_line.find(",", current_delim_pos) + 1;
            }
            trainers.push_back(new Trainer(std::stoi(cur_line.substr(current_delim_pos))));
            capacities_read = true;
        }
        else {
            addWorkoutFromConfig(cur_line, workout_id);
            workout_id++;
        }
    }
}