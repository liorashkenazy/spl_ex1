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
    Studio& operator=(const Studio& other);

    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    int getCurrentCustomerId() const;
    void SetCurrentCustomerId(int increment_by);
    void addActionToLog(BaseAction *action);
    void close();

    virtual ~Studio();
private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int next_customer_id;

    void addWorkoutFromConfig(const std::string &workout_info, int workout_id);
    BaseAction *parseAction(const std::string &action_str);
    void clear();
    void copyActionsLog(const Studio& other);
};

#endif
