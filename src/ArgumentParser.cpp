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

#include <iostream>
#include "ArgumentParser.h"
#include "InvalidArgumentException.h"
#include "ArgumentParserUtils.h"

using namespace std;

namespace cppargparser {

ArgumentParser::~ArgumentParser() {}

void ArgumentParser::addArgument(const Argument& arg) {
    vargs.push_back(arg);
    if (arg.getShortArg().size() > 0) {
        args.insert(pair<string, Argument>(arg.getShortArg(), arg));
    }

    if (arg.getLongArg().size() > 0) {
        args.insert(pair<string, Argument>(arg.getLongArg(), arg));
    }
}

ParsedArgument ArgumentParser::parse(int argc, char** argv) {
    // put all the arguments into vector for easy manipulation
    vector<string> v;
    for (int i = 1; i < argc; i++) {
        v.push_back(string(argv[i]));
    }
    ParsedArgument pa;
    // ignore the first argument since the first argument is a program name
    for (size_t i = 0; i < v.size(); ++i) {
        string s = v[i];
        bool shortArg = cppargparser::isShortArg(s);
        bool longArg = cppargparser::isLongArg(s);
        if (!shortArg && !longArg) {
            throw InvalidArgumentException(s + " is an invalid argument");
        }
        string arg = s;
        if (longArg) {
            // if the argument has =, e.g. ---ccc=123 split it by = and then
            // modify the vector by modifying --ccc=123 with --ccc and adding
            // a new element 123
            size_t index = arg.find("=");
            if (index != string::npos) {
                string key = arg.substr(0, index);
                string value = arg.substr(index+1);
                arg = key;
                v[i] = key;
                v.insert(v.begin()+i+1, value);
            }
        }
        map<string, Argument>::iterator it = args.find(arg);
        if (it == args.end()) {
            throw InvalidArgumentException(arg + " is an invalid argument");
        }
        Argument argument = it->second;
        if (argument.getNumArgs() == Argument::INFINITY) {
            string value = "";
            do {
                value = v[++i];
                if (!cppargparser::isShortArg(value)) {
                    pa.putArgument(argument.getShortArg(), value);
                }
                if (!cppargparser::isLongArg(value)) {
                    pa.putArgument(argument.getLongArg(), value);
                }
            } while (!isShortArg(value) && !isLongArg(value));
        } else {
            i = i + 1;
            size_t n = i + argument.getNumArgs();
            // this condition means there's the argument doesn't need any value,
            // i.e. the numArgs is 0, thus there's no need to iterate each
            // argument value
            if (i == n) {
                pa.putArgument(argument.getShortArg(), "");
                pa.putArgument(argument.getLongArg(), "");
            }
            else {
                for (; i < n; ++i) {
                    if (i >= v.size()) {
                        throw InvalidArgumentException(
                            argument.getDescription() + " requires " +
                            cppargparser::toString(argument.getNumArgs()) +
                            " argument(s)");
                    }
                    string value = v[i];
                    pa.putArgument(argument.getShortArg(), value);
                    pa.putArgument(argument.getLongArg(), value);
                }
            }
        }
        Validator* validator = argument.getValidator();
        if (validator != NULL) {
            if (argument.isShortArg()) {
                vector<string> values = pa.getValues(argument.getShortArg());
                if (!validator->validate(values)) {
                    throw InvalidArgumentException(cppargparser::toString(values) +
                        " is an invalid argument value");
                }
            } else {
                vector<string> values = pa.getValues(argument.getLongArg());
                if (!validator->validate(values)) {
                    throw InvalidArgumentException(cppargparser::toString(values) +
                        " is an invalid argument value");
                }
            }
        }
        args.erase(argument.getShortArg());
        args.erase(argument.getLongArg());
        // need to decrement i here because both inner and outer loops
        // increment i by 1
        --i;
    }
    // check if the there are still mandatory arguments in the args map
    // if there are, throw an InvalidArgumentException
    for (map<string, Argument>::const_iterator i = args.begin(); i != args.end(); ++i) {
        if (i->second.isMandatory()) {
            throw InvalidArgumentException(i->second.getDescription() +
                " is a mandatory argument");
        }
    }

    return pa;
}

void ArgumentParser::showHelp(const string& programName) const {
    cout << "Usage: " << programName << endl;
    cout << "Options:" << endl;
    for (vector<Argument>::const_iterator i = vargs.begin(); i != vargs.end(); ++i) {
        string options = "";
        if (i->isShortArg()) {
            options += i->getShortArg();
            if (i->isLongArg()) {
                options += ", ";
            }
        }
        if (i->isLongArg()) {
            options += i->getLongArg();
        }
        cout << string(4, ' ') << options << string(40-options.size(), ' ') <<
            i->getDescription() << endl;
    }
}

} /* namespace cppargparser */
