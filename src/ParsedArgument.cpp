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

#include "ParsedArgument.h"
#include "InvalidArgumentException.h"

using namespace std;

namespace cppargparser {

ParsedArgument::~ParsedArgument() {}

void ParsedArgument::putArgument(const string& arg, const string& value) {
    if (arg.size() == 0) {
        return;
    }
    map<string, vector<string> >::iterator i = args.find(arg);
    if (i == args.end()) {
        vector<string> v;
        v.push_back(value);
        args[arg] = v;
    } else {
        i->second.push_back(value);
    }
}

string ParsedArgument::getValue(const string& arg) const {
    map<string, vector<string> >::const_iterator i = args.find(arg);
    if (i == args.end()) {
        throw InvalidArgumentException(arg + " is an invalid argument");
    }
    // always return the first index
    return i->second[0];
}

vector<string> ParsedArgument::getValues(const string& arg) const {
    map<string, vector<string> >::const_iterator i = args.find(arg);
    if (i == args.end()) {
        throw InvalidArgumentException(arg + " is an invalid argument");
    }

    return i->second;
}

bool ParsedArgument::hasArgument(const string& arg) const {
    map<string, vector<string> >::const_iterator i = args.find(arg);
    return (i == args.end()) ? false : true;
}

} /* namespace cppargparser */
