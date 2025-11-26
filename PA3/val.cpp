#include "val.h"
#include <type_traits>

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
        return Value();;
    }
}

Value Value::operator-(const Value& op) const {
    if(realNum(*this) && realNum(op)) {
        return Value(getNumber(*this) - getNumber(op));
    }
    else {
        return Value();;
    }
}

Value Value::operator+(const Value& op) const {
    if(realNum(*this) && realNum(op)) {
        return Value(getNumber(*this) + getNumber(op));
    }
    else {
        return Value();;
    }
}

Value Value::operator/(const Value& op) const {
    if(realNum(*this) && realNum(op)) {
        return Value(getNumber(*this) / getNumber(op));
    }
    else {
        return Value();;
    }
}

Value Value::operator%(const Value& op) const {
    if(realNum(*this) && realNum(op)) {
        return Value(std::fmod(getNumber(*this), getNumber(op)));
    }
    else {
        return Value();
    }
}

Value Value::Expon(const Value& oper) const {
    if(this->IsNum() && oper.IsNum()) {
        return Value(std::pow(this->GetNum(), oper.GetNum()));
    }
    else {
        return Value();;
    }
}

Value Value::operator<(const Value& op) const {
    if(this->IsNum() && op.IsNum()) {
        return Value(this->GetNum() < op.GetNum());
    }
    else {
        return Value();;
    }
}

Value Value::operator>=(const Value& op) const {
    if(realNum(*this) && realNum(op)) {
        return Value(getNumber(*this) >= getNumber(op));
    }
    else {
        return Value();;
    }
}

Value Value::operator==(const Value& op) const {
    if(realNum(*this) && realNum(op)) {
        return Value(getNumber(*this) == getNumber(op));
    }
    else {
        return Value();;
    }
}

Value Value::SLE(const Value& oper) const {
    std::string val1, val2;
    if(!this->IsString()) {
        if(this->IsNum()) {
            val1 = std::to_string(this->GetNum());
        }
        else {
            return Value();;
        }
    } else {
        val1 = this->GetString();
    }

    if(!oper.IsString()) {
        if(oper.IsNum()) {
            val2 = std::to_string(oper.GetNum());  
        }
        else {
            return Value();;
        }
    } else {
        val2 = oper.GetString();
    }

    return Value(val1.compare(val2) < 0);
}

Value Value::SGT(const Value& oper) const {
    std::string val1, val2;
    if(!this->IsString()) {
        if(this->IsNum()) {
            val1 = std::to_string(this->GetNum());
        }
        else {
            return Value();;
        }
    } else {
        val1 = this->GetString();
    }

    if(!oper.IsString()) {
        if(oper.IsNum()) {
            val2 = std::to_string(oper.GetNum());  
        }
        else {
            return Value();;
        }
    } else {
        val2 = oper.GetString();
    }

    return Value(val1.compare(val2) > 0);
}

Value Value::Catenate(const Value& oper) const {
    std::string val1, val2;
    if(!this->IsString()) {
        if(this->IsNum()) {
            val1 = std::to_string(this->GetNum());
        }
        else {
            return Value();;
        }
    } else {
        val1 = this->GetString();
    }

    if(!oper.IsString()) {
        if(oper.IsNum()) {
            val2 = std::to_string(oper.GetNum());  
        }
        else {
            return Value();;
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
            return Value();;
        }
    } else {
        val1 = this->GetString();
    }
    val1 = stripZeros(val1);
    
    int val2;
    if(realNum(oper)) {
        val2 = (int)getNumber(oper);
    } else {
        return Value();;
    }

    std::string result = "";
    for(int i = 0; i< val2;i++) {
        result += val1;
    }
    return result;
}

Value Value::SEQ(const Value& oper) const {
    if(this->IsErr() || oper.IsErr()) {
        return Value();;
    }
    std::string val1, val2;

    if(!this->IsString()) {
        if(this->IsNum()) {
            val1 = std::to_string(this->GetNum());
        }
        else {
            return Value();;
        }
    } else {
        val1 = this->GetString();
    }

    if(!oper.IsString()) {
        if(oper.IsNum()) {
            val2 = std::to_string(oper.GetNum());  
        }
        else {
            return Value();;
        }
    } else {
        val2 = oper.GetString();
    }
    return val1 == val2;
}

Value Value::operator&&(const Value& op) const {
    bool val1, val2;

    if(this->IsNum()) {
        if(this->GetNum() == 0) {
            val1 = false;
        } else {
            val1 = true;
        }
    } else if(this->IsString()) {
        if(this->GetString() == "") {
            val1 = false;
        } else {
            val1 = true;
        }
    } else if(this->IsBool()) {
        val1 = this->GetBool();
    } else {
        return Value();;
    }

    if(op.IsNum()) {
        if(op.GetNum() == 0) {
            val2 = false;
        } else {
            val2 = true;
        }
    } else if(op.IsString()) {
        if(op.GetString() == "") {
            val2 = false;
        } else {
            val2 = true;
        }
    } else if(op.IsBool()) {
        val2 = op.GetBool();
    } else {
        return Value();;
    }

    return Value(val1 && val2);
}

Value Value::operator||(const Value& op) const {
    bool val1, val2;

    if(this->IsNum()) {
        if(this->GetNum() == 0) {
            val1 = false;
        } else {
            val1 = true;
        }
    } else if(this->IsString()) {
        if(this->GetString() == "") {
            val1 = false;
        } else {
            val1 = true;
        }
    } else if(this->IsBool()) {
        val1 = this->GetBool();
    } else {
        return Value();;
    }

    if(op.IsNum()) {
        if(op.GetNum() == 0) {
            val2 = false;
        } else {
            val2 = true;
        }
    } else if(op.IsString()) {
        if(op.GetString() == "") {
            val2 = false;
        } else {
            val2 = true;
        }
    } else if(op.IsBool()) {
        val2 = op.GetBool();
    } else {
        return Value();;
    }

    return Value(val1 || val2);
}

Value Value::operator!() const {
    bool val1;

    if(this->IsNum()) {
        if(this->GetNum() == 0) {
            val1 = false;
        } else {
            val1 = true;
        }
    } else if(this->IsString()) {
        if(this->GetString() == "") {
            val1 = false;
        } else {
            val1 = true;
        }
    } else if(this->IsBool()) {
        val1 = this->GetBool();
    } else {
        return Value();;
    }

    return Value( !val1 );
}