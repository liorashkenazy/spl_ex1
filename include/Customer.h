#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include <typeinfo>
#include "Workout.h"

// String representation of the customer types
#define CUSTOMER_TYPE_CHEAP_STR "chp"
#define CUSTOMER_TYPE_SWEATY_STR "swt"
#define CUSTOMER_TYPE_HEAVY_MUSCLE_STR "mcl"
#define CUSTOMER_TYPE_FULL_BODY_STR "fbd"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options) = 0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual ~Customer();
private:
    const std::string name;
    const int id;
};


class SweatyCustomer : public Customer {
public:
	SweatyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;

    static const std::string type_str;
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;

    static const std::string type_str;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
	HeavyMuscleCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;

    static const std::string type_str;
private:
};


class FullBodyCustomer : public Customer {
public:
	FullBodyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;

    static const std::string type_str;
private:
};

#endif