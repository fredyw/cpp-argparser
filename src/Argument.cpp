// Copyright 2012, Fredy Wijaya
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the Lesser GNU General Public License as published by
// the Free Software Foundation, either version 3.0 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Lesser GNU General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>

#include "Argument.h"

using namespace std;

namespace cppargparser {

Argument::Argument(const string& _shortArg, const string& _longArg,
    const string& _desc, int _numArgs, bool _mandatory) :
    shortArg(_shortArg),
    longArg(_longArg),
    description(_desc),
    numArgs(_numArgs),
    mandatory(_mandatory),
    validator(NULL) {
}

Argument::Argument(const string& _shortArg, const string& _longArg,
    const string& _desc, int _numArgs, bool _mandatory, Validator* _validator) :
    shortArg(_shortArg),
    longArg(_longArg),
    description(_desc),
    numArgs(_numArgs),
    mandatory(_mandatory) {
    validator = _validator;
}

Argument::Argument(const string& _arg, const string& _desc, Type _type,
    int _numArgs, bool _mandatory) :
    shortArg(""),
    longArg(""),
    description(_desc),
    numArgs(_numArgs),
    mandatory(_mandatory),
    validator(NULL) {
    switch (_type) {
    case SHORT:
        shortArg = _arg;
        break;
    case LONG:
        longArg = _arg;
        break;
    }
}

Argument::Argument(const string& _arg, const string& _desc, Type _type,
    int _numArgs, bool _mandatory, Validator* _validator) :
    shortArg(""),
    longArg(""),
    description(_desc),
    numArgs(_numArgs),
    mandatory(_mandatory) {
    switch (_type) {
    case SHORT:
        shortArg = _arg;
        break;
    case LONG:
        longArg = _arg;
        break;
    }
    validator = _validator;
}

Argument::~Argument() {}

string Argument::getArg() const {
    if (!shortArg.empty()) {
        return shortArg;
    }
    if (!longArg.empty()) {
        return longArg;
    }
    return "";
}

string Argument::getShortArg() const {
    return shortArg;
}

string Argument::getLongArg() const {
    return longArg;
}

bool Argument::isShortArg() const {
    return (shortArg.size() > 0) ? true : false;
}

bool Argument::isLongArg() const {
    return (longArg.size() > 0) ? true : false;
}

string Argument::getDescription() const {
    return description;
}

int Argument::getNumArgs() const {
    return numArgs;
}

bool Argument::isMandatory() const {
    return mandatory;
}

Validator* Argument::getValidator() {
    return validator;
}

}
