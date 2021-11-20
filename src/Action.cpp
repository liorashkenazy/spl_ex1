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

void PrintWorkoutOptions::act(Studio& studio) {
    for (const Workout& opt : studio.getWorkoutOptions()) {
        std::cout << opt.toString() << std::endl;
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    return "Print workout options";
}
