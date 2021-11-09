//
// Created by spl211 on 09/11/2021.
//
#include <string>
#include "Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):
        id(w_id), name(w_name), price(w_price), type(w_type) {}