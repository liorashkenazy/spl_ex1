#ifndef WORKOUT_H_
#define WORKOUT_H_

#include <string>

#define ANEROBIC_TYPE_STR "Anaerobic"
#define MIXED_TYPE_STR "Mixed"
#define CARDIO_TYPE_STR "Cardio"

enum WorkoutType{
    ANAEROBIC, MIXED, CARDIO
};

class Workout{
public:
    Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type);
    int getId() const { return id; };
    std::string getName() const { return name; };
    int getPrice() const { return price; };
    WorkoutType getType() const { return type; };
private:
	const int id;
    const std::string name;
    const int price;
    const WorkoutType type;
};


#endif