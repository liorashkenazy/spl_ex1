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

#define ANEROBIC_TYPE_STR "Anaerobic"
#define MIXED_TYPE_STR "Mixed"

void Studio::addWorkoutFromConfig(const std::string &workout_info, int workout_id)
{
    int first_delim_pos(workout_info.find(","));
    int second_delim_pos(workout_info.find(",", first_delim_pos+1));
    std::string workout_name(workout_info.substr(0, first_delim_pos));
    std::string workout_type(workout_info.substr(first_delim_pos + 2, second_delim_pos - (first_delim_pos + 2)));
    WorkoutType w_type;
    int price(0);

    if (workout_type.compare(ANEROBIC_TYPE_STR) == 0) {
        w_type = ANAEROBIC;
    } else if (workout_type.compare(MIXED_TYPE_STR) == 0) {
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
//void Studio::getAll()
//{
//    int numOfTrainers(5);
//    numOfTrainers = trainers.size();
//    std::cout <<numOfTrainers<< std::endl;
//    std::cout <<(trainers.at(0)->getCapacity())<< std::endl;
//    std::cout <<"id: " << workout_options.at(0).getId()<< " name: "<<workout_options.at(0).getName()
//    << " price: "<<workout_options.at(0).getPrice() << " type: " << workout_options.at(0).getType()<< std::endl;
//
//}