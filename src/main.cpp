#include "Studio.h"
#include "Action.h"
#include <iostream>

using namespace std;

//Studio* backup = nullptr;

BaseAction *parseAction(const std::string &action_str, const Studio& studio)
{
    std::string action_type = action_str.substr(0, action_str.find(' '));

    if (action_type == ACTION_TYPE_OPEN_TRAINER_STR) {
        return OpenTrainer::createOpenTrainerAction(action_str.substr(action_str.find(' ') + 1),
                                                    studio.getCurrentCustomerId());
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
    string current_input;
    BaseAction *next_action;

//    while (typeid(next_action) != typeid(CloseAll)) {
    std::cout << "Please enter the next input" << std::endl;
    getline(cin, current_input);
    next_action = parseAction(current_input, studio);
//        next_action->act(studio);
//    }

//    studio.getAll();
//    studio.start();
//    if(backup!=nullptr){
//    	delete backup;
//    	backup = nullptr;
//    }
    return 0;
}