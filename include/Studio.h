#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    Studio(const Studio& other);
    Studio(Studio&& other);
    Studio& operator=(const Studio& other);
    Studio& operator=(Studio&& other);

    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    // Returns the next available customer id
    int getNextCustomerId() const;
    // Sets the next available customer id
    void SetNextCustomerId(int next_id);
    void close();

    virtual ~Studio();
private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int next_customer_id;

    /* *
     * @brief Parses a single WorkoutOption from the config, and inserts it to the workout_options vector
     * @param[in]  workout_info   The configuration string that represents the workout, expected: "name, type, price"
     * @param[in]  workout_id     The id to be associated with the Workout
     * */
    void addWorkoutFromConfig(const std::string &workout_info, int workout_id);
    /* *
     * @brief Parses a single action from the user input
     * @param[in]  action_str    The user input, expected: "action_name param_1 param_2 ... param_n"
     * @return     BaseAction*   A pointer to the newly created Action based on the action_str
     * */
    BaseAction *parseAction(const std::string &action_str);
    // Deletes every instance in the trainers and actionsLog vectors, and empties them.
    void clear();
    // Set the actionsLog to be a deep copy of the actionsLog of the other Studio.
    void copyActionsLog(const Studio& other);
};

#endif
