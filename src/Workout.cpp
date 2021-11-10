//
// Created by spl211 on 09/11/2021.
//
#include <string>
#include "Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):
        id(w_id), name(w_name), price(w_price), type(w_type) {}


int Workout::getId() const{
    return this->id;
};

std::string Workout::getName() const{
    return this->name;
};

int Workout::getPrice() const{
    return this->price;
};

WorkoutType Workout::getType() const{
    return this->type;
};