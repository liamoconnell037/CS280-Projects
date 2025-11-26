#include "val.h"
#include <type_traits>

static std::string error = "ERROR";

bool realNum(Value val) {
    if(val.IsNum()) {
        return true;
    } else if(val.IsString()) {
        try {
            std::stod(val.GetString());
            return true;
        } catch(const std::exception& e) {
            return false;
        }
    } else {
        return false;
    }
}

std::string stripZeros(std::string s) {
    while (s.find('.') != std::string::npos && s.back() == '0')
        s.pop_back();

    if (!s.empty() && s.back() == '.')
        s.pop_back();

    return s;
}

double getNumber(Value val) {
    if(realNum(val)) {
        if(val.IsNum()) {
            return val.GetNum();
        } else {
            return std::stod(val.GetString());
        }
    } else {
        throw "RUNTIME ERROR: Value is not a real number.";
    }
}

Value Value::operator*(const Value& op) const {
    if(realNum(*this) && realNum(op)) {
        return Value(getNumber(*this) * getNumber(op));
    }
    else {
        return Value(error);
    }
}

Value Value::operator<(const Value& op) const {
    if(realNum(*this) && realNum(op)) {
        return Value(getNumber(*this) < getNumber(op));
    }
    else {
        return Value(error);
    }
}

Value Value::Catenate(const Value& oper) const {
    std::string val1, val2;
    if(!this->IsString()) {
        if(this->IsNum()) {
            val1 = std::to_string(this->GetNum());
        }
        else {
            return Value(error);
        }
    } else {
        val1 = this->GetString();
    }

    if(!oper.IsString()) {
        if(oper.IsNum()) {
            val2 = std::to_string(oper.GetNum());  
        }
        else {
            return Value(error);
        }
    } else {
        val2 = oper.GetString();
    }
    std::string cat = stripZeros(val1) + stripZeros(val2);
    return Value(cat);
}

Value Value::Repeat(const Value& oper) const {
    
    std::string val1;
    if(!this->IsString()) {
        if(this->IsNum()) {
            val1 = std::to_string(this->GetNum());
        }
        else {
            return Value(error);
        }
    } else {
        val1 = this->GetString();
    }
    val1 = stripZeros(val1);
    
    int val2;
    if(realNum(oper)) {
        val2 = (int)getNumber(oper);
    } else {
        return Value(error);
    }

    std::string result = "";
    for(int i = 0; i< val2;i++) {
        result += val1;
    }
    return result;
}

Value Value::SEQ(const Value& oper) const {
    if(this->IsErr() || oper.IsErr()) {
        return Value(error);
    }
    std::string val1, val2;

    if(!this->IsString()) {
        if(this->IsNum()) {
            val1 = std::to_string(this->GetNum());
        }
        else {
            return Value(error);
        }
    } else {
        val1 = this->GetString();
    }

    if(!oper.IsString()) {
        if(oper.IsNum()) {
            val2 = std::to_string(oper.GetNum());  
        }
        else {
            return Value(error);
        }
    } else {
        val2 = oper.GetString();
    }
    return val1 == val2;
}