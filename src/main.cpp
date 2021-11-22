#include "Studio.h"
#include "Action.h"
#include <iostream>

using namespace std;

//Studio* backup = nullptr;

BaseAction *parseAction(const std::string &action_str, Studio& studio)
{
    std::string action_type = action_str.substr(0, action_str.find(' '));

    std::string data = action_str.find(' ') == string::npos ? "": action_str.substr(action_str.find(' ') + 1);
    if (action_type == OpenTrainer::name) {
        return OpenTrainer::createOpenTrainerAction(data,
                                                    studio.getCurrentCustomerId());
    }
    else if (action_type == Order::name) {
        return Order::createOrder(data);
    }
    else if (action_type == MoveCustomer::name) {
        return MoveCustomer::createMoveCustomer(data);
    }
    else if (action_type == Close::name) {
        return Close::createClose(data);
    }
    else if (action_type == PrintWorkoutOptions::name) {
        return new PrintWorkoutOptions();
    }
    else if (action_type == PrintTrainerStatus::name) {
        return PrintTrainerStatus::createPrintTrainerStatus(data);
    }
    else if (action_type == PrintActionsLog::name) {
        return new PrintActionsLog();
    }
    return nullptr;
}

int main(int argc, char** argv){

    if(argc!=2){
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Studio studio("config.txt");

    studio.getAll();
    string current_input;
    BaseAction *next_action;
    while (next_action != nullptr) {
        std::cout << "Please enter the next input" << std::endl;
        getline(cin, current_input);
        next_action = parseAction(current_input, studio);
        next_action->act(studio);
        studio.addActionToLog(next_action);
//        next_action->act(studio);
    }

//    studio.start();
//    if(backup!=nullptr){
//    	delete backup;
//    	backup = nullptr;
//    }
    return 0;
}
