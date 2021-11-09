//
// Created by spl211 on 09/11/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{
public:
    Studio();
    Studio(const std::string &configFilePath): open(false) {
        bool num_trainers_read(false);
        bool capacities_read(false);
        int num_trainers(0);
        fstream config_file;
        string cur_line;
        int wrokout_id(0);
        std::stringstream cur_line_stream();
        config_file.open(configFilePath,ios::in);

        while (std::getline(&config_file, &cur_line)) {
            if (cur_line.length() == 0 || cur_line.at(0) == '#') {
                continue;
            }
            if (!num_trainers_read) {
                num_trainers = std::stoi(&cur_line);
                num_trainers_read = true;
            }
            else if (!capacities_read) {
                string current_token;
                cur_line_stream.str(cur_line);
                while (std::getline(&cur_line_stream, &current_token, ",")) {
                    trainers.push_back(new Trainer(std::stoi(current_token)));
                }
                capacities_read = true;
            }
            else {
                addWorkoutFromConfig(cur_line, workout_id);
                workout_id++;
            }
        }
    }
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;

    void addWorkoutFromConfig(const std::string &workout_info, int workout_id) {
        int first_delim_pos(workout_info.find(","));
        int second_delim_pos(workout_info.find(",", first_delim_pos));
        string workout_name(workout_info.substr(0, first_delim_pos));
        string workout_type(first_delim_pos + 2, second_delim_pos);
        WorkoutType w_type;
        int price(0);

        if (wourkout_type.compare("Anaeorbic") == 0) {
            w_type = WorkoutType.ANAEROBIC;
        } else if (wourkout_type.compare("Mixed") == 0) {
            w_type = WorkoutType.MIXED;
        } else {
            w_type = WorkoutType.CARDIO;
        }


    }
};