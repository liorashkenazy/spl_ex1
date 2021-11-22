#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

#define ACTION_TYPE_OPEN_TRAINER_STR "open"
#define ACTION_TYPE_ORDER_STR "order"
#define ACTION_TYPE_MOVE_CUSTOMER_STR "move"
#define ACTION_TYPE_CLOSE_STR "close"
#define ACTION_TYPE_CLOSE_ALL_STR "closeall"
#define ACTION_TYPE_PRINT_OPTIONS_STR "workout_options"
#define ACTION_TYPE_PRINT_T_STATUS_STR "status"
#define ACTION_TYPE_PRINT_LOG_STR "log"
#define ACTION_TYPE_BACKUP_STR "backup"
#define ACTION_TYPE_RESTORE_STR "restore"

enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    void act(Studio &studio);
    std::string toString() const;
    static OpenTrainer *createOpenTrainerAction(const std::string &data, int next_customer_id);
    static const std::string name;
private:
	const int trainerId;
	std::vector<Customer *> customers;
    std::string action_args;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio &studio);
    std::string toString() const;
    static Order *createOrder(const std::string& data);
    static const std::string name;
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio &studio);
    std::string toString() const;
    static MoveCustomer *createMoveCustomer(const std::string &data);
    static const std::string name;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio &studio);
    std::string toString() const;
    static Close *createClose(const std::string& data);
    static const std::string name;
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    static const std::string name;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    static const std::string name;
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio &studio);
    std::string toString() const;
    static PrintTrainerStatus *createPrintTrainerStatus(const std::string& data);
    static const std::string name;
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    static const std::string name;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    static const std::string name;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    static const std::string name;

};


#endif