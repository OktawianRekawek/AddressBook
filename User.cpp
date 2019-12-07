#include <iostream>
#include "User.h"

using namespace std;

User::User(int i, string log, string pass) {
    id = i;
    login = log;
    password = pass;
}

int User::getId() {
    return id;
}

void User::setId(int i){
    id = i;
}

string User::getLogin() {
    return login;
}

void User::setLogin(string log){
    login = log;
}

string User::getPassword() {
    return password;
}

void User::setPassword(string pass){
    password = pass;
}
