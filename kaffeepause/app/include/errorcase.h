#pragma once

#include <string>
using namespace std;
class ErrorCase {

private :
    int errorcode;
    string errormessage;

public :
    ErrorCase();
    ~ErrorCase();
    int getErrorcode();
    void setErrorcode(int errorcode);
    string getErrormessage();
    void setErrormessage(string errormessage);
    bool manageError();

};

