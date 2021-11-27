#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include <typeinfo>
#include "Customer.h"

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
    virtual ~BaseAction() = default;
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
    // Implemented the destructor as a precautionary action, the destructor should never be called whilst the customers
    // vector is not empty.
    virtual ~OpenTrainer();

    /* *
     * @brief Creates a OpenTrainer action based on the user input
     * @param[in]  data     The user input, expected input is: "trainer_id customer_1_name,customer_1_type
     *                      customer_2_name,customer_2_type ... customer_n_name,customer_n_type"
     * @return The created OpenTrainer action.
     * */
    static OpenTrainer *createOpenTrainerAction(const std::string &data, int next_customer_id);
    // String representation of this action's user input command
    static const std::string name;
private:
	const int trainerId;
	std::vector<Customer *> customers;
    // String representation of the successfully added customers, to display in the logs
    std::string added_customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio &studio);
    std::string toString() const;

    /* *
     * @brief Creates an Order action based on the user input
     * @param[in]  data     The user input, expected input is: "trainer_id"
     * @return The created Order action.
     * */
    static Order *createOrder(const std::string& data);
    // String representation of this action's user input command
    static const std::string name;
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio &studio);
    std::string toString() const;

    /* *
     * @brief Creates a CreateMoveCustomer action based on the user input
     * @param[in]  data     The user input, expected input is: "source_trainer_id dest_trainer_id customer_id"
     * @return The created MoveCustomer action.
     * */
    static MoveCustomer *createMoveCustomer(const std::string &data);
    // String representation of this action's user input command
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

    /* *
     * @brief Creates a Close action based on the user input
     * @param[in]  data     The user input, expected input is: "trainer_id"
     * @return The created Close action.
     * */
    static Close *createClose(const std::string& data);
    // String representation of this action's user input command
    static const std::string name;
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;

    // String representation of this action's user input command
    static const std::string name;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;

    // String representation of this action's user input command
    static const std::string name;
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio &studio);
    std::string toString() const;

    /* *
     * @brief Creates a Close action based on the user input
     * @param[in]  data     The user input, expected input is: "trainer_id"
     * @return The created Close action.
     * */
    static PrintTrainerStatus *createPrintTrainerStatus(const std::string& data);
    // String representation of this action's user input command
    static const std::string name;
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;

    // String representation of this action's user input command
    static const std::string name;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;

    // String representation of this action's user input command
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