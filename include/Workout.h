#ifndef WORKOUT_H_
#define WORKOUT_H_

#include <string>

enum WorkoutType{
    ANAEROBIC, MIXED, CARDIO
};

class Workout{
public:
    Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    WorkoutType getType() const;
    std::string toString() const;

    // An array that maps between WorkoutType enum to a string representation of the workout type
    static const std::string typeToTypeStr[];
private:
	const int id;
    const std::string name;
    const int price;
    const WorkoutType type;
};


#endif