//
// Created by spl211 on 09/11/2021.
//

#include <vector>
#include "Trainer.h"
#include "Customer.h"
#include "Workout.h"

Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false) {}

int Trainer::getCapacity() const{
    return this->capacity;
}
