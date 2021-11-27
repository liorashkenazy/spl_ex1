//
// Created by spl211 on 09/11/2021.
//
#include <string>
#include <utility>
#include "Workout.h"

// An array of strings that has to keep the same order as the WorkoutType enums, is used to convert the enum to an
// appropriate string.
const std::string Workout::typeToTypeStr[] = {"Anaerobic", "Mixed", "Cardio"};

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):
        id(w_id), name(w_name), price(w_price), type(w_type) {}

int Workout::getId() const {
    return id;
}

std::string Workout::getName() const {
    return name;
}

int Workout::getPrice() const {
    return price;
}

WorkoutType Workout::getType() const {
    return type;
}

std::string Workout::toString() const {
    return getName() +
            ", " +
            typeToTypeStr[getType() - ANAEROBIC] +
            ", " +
            std::to_string(getPrice());
}