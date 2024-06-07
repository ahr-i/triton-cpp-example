#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <string>

using namespace std;

class Manager {
    string health_checker_address_;
    string port_;
public:
    Manager(const string& address, const string& port);
    void registerWithHealthChecker();
};

#endif
