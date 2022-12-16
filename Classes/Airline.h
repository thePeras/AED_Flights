#ifndef AED2_AIRLINE_H
#define AED2_AIRLINE_H

#include <string>
using namespace std;

class Airline {
    private:
    string code;
    string name;
    string callSign;
    string country;

    public:
    Airline(){
        code = "";
        name = "";
        callSign = "";
        country = "";
    }

    Airline(string code, string name, string callSign, string country){
        this->code = code;
        this->name = name;
        this->callSign = callSign;
        this->country = country;
    }

    const string &getCode() const {
        return code;
    }

    void setCode(const string &code) {
        this->code = code;
    }

    const string &getName() const {
        return name;
    }

    void setName(const string &name) {
        this->name = name;
    }

    const string &getCallSign() const {
        return callSign;
    }

    void setCallSign(const string &callSign) {
        this->callSign = callSign;
    }

    const string &getCountry() const {
        return country;
    }

    void setCountry(const string &country) {
        this->country = country;
    }
};


#endif //AED2_AIRLINE_H
