#include <iostream>

using namespace std;

class User {

private:
    int id;
    string login,
           password;
public:
    User(int=0, string="user", string="password");
    int getId();
    void setId(int);
    string getLogin();
    void setLogin(string);
    string getPassword();
    void setPassword(string);
};
