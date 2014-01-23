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

#include <gtest/gtest.h>
#include "ArgumentParser.h"
#include "InvalidArgumentException.h"
#include <sstream>

using namespace std;
using namespace testing;
using namespace cppargparser;

class PortNumberValidator : public Validator {
public:
    bool validate(const vector<string>& values) {
        for (vector<string>::const_iterator i = values.begin(); i != values.end(); ++i) {
            istringstream iss(*i);
            int port;
            try {
                iss >> port;
                if (port < 0 || port > 65535) {
                    return false;
                }
            } catch (...) {
                return false;
            }
        }
        return true;
    }

    ~PortNumberValidator() {}
};

TEST(ArgumentParserTest, ParseAllShortArguments) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "-a arg", Argument::SHORT, 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("-c", "-c", Argument::SHORT, 0, true));
    argParser.addArgument(Argument("-d", "-d arg1 arg2", Argument::SHORT, 2, false));
    argParser.addArgument(Argument("-e", "-e", Argument::SHORT, 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));
    argParser.addArgument(Argument("-h", "-h arg1...", Argument::SHORT,
        Argument::INFINITY, true));
    argParser.addArgument(Argument("-i", "-i arg", Argument::SHORT, 1, true));

    const char* cargv[] = {
        "test_program", "-a", "1", "-b", "2", "3", "-c", "-d", "foo", "bar",
        "-f", "abc", "-h", "1", "2", "3", "4", "5", "-i", "blah"
    };
    char** argv = const_cast<char**>(cargv);
    ParsedArgument pa = argParser.parse(20, argv);

    EXPECT_EQ("1", pa.getValue("-a"));
    vector<string> args = pa.getValues("-b");
    EXPECT_EQ("2", args[0]);
    EXPECT_EQ("3", args[1]);
    EXPECT_TRUE(pa.hasArgument("-c"));
    EXPECT_EQ("", pa.getValue("-c"));
    args = pa.getValues("-d");
    EXPECT_EQ("foo", args[0]);
    EXPECT_EQ("bar", args[1]);
    EXPECT_FALSE(pa.hasArgument("-e"));
    EXPECT_EQ("abc", pa.getValue("-f"));
    args = pa.getValues("-h");
    EXPECT_EQ(5u, args.size());
    EXPECT_EQ("1", args[0]);
    EXPECT_EQ("2", args[1]);
    EXPECT_EQ("3", args[2]);
    EXPECT_EQ("4", args[3]);
    EXPECT_EQ("5", args[4]);
    EXPECT_EQ("blah", pa.getValue("-i"));
}

TEST(ArgumentParserTest, ParseAllShortArgumentsHasAdditionalArgument) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "-a arg", Argument::SHORT, 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("-c", "-c", Argument::SHORT, 0, true));
    argParser.addArgument(Argument("-d", "-d arg1 arg2", Argument::SHORT, 2, false));
    argParser.addArgument(Argument("-e", "-e", Argument::SHORT, 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));

    const char* cargv[] = {
        "test_program", "-a", "1", "-b", "2", "3", "-c", "-d", "foo", "bar",
        "-f", "abc", "h", "whatever"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(14, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseAllShortArgumentsMissingMandatoryArgument) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "-a arg", Argument::SHORT, 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("-c", "-c", Argument::SHORT, 0, true));
    argParser.addArgument(Argument("-d", "-d arg1 arg2", Argument::SHORT, 2, false));
    argParser.addArgument(Argument("-e", "-e", Argument::SHORT, 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));

    const char* cargv[] = {
        "test_program", "-b", "2", "3", "-c", "-d", "foo", "bar",
        "-f", "abc"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(10, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseAllShortArgumentsInvalidArgumentValue1) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "-a arg", Argument::SHORT, 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("-c", "-c", Argument::SHORT, 0, true));
    argParser.addArgument(Argument("-d", "-d arg1 arg2", Argument::SHORT, 2, false));
    argParser.addArgument(Argument("-e", "-e", Argument::SHORT, 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));

    const char* cargv[] = {
        "test_program", "-a", "1", "-b", "2", "3", "xxx", "-c", "-d", "foo", "bar",
        "-f", "abc"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(13, argv), InvalidArgumentException);
}


TEST(ArgumentParserTest, ParseAllShortArgumentsInvalidArgumentValue2) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "-a arg", Argument::SHORT, 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("-c", "-c", Argument::SHORT, 0, true));
    argParser.addArgument(Argument("-d", "-d arg1 arg2", Argument::SHORT, 2, false));
    argParser.addArgument(Argument("-e", "-e", Argument::SHORT, 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));

    const char* cargv[] = {
        "test_program", "-a", "1", "-b", "2", "-c", "-d", "foo", "bar",
        "-f", "abc"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(11, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseAllLongArguments) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("--aaa", "-aaa arg", Argument::LONG, 1, true));
    argParser.addArgument(Argument("--bbb", "-bbb arg1 arg2", Argument::LONG, 2, true));
    argParser.addArgument(Argument("--ccc", "-ccc", Argument::LONG, 0, true));
    argParser.addArgument(Argument("--ddd", "-ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("--eee", "-eee", Argument::LONG, 0, false));
    argParser.addArgument(Argument("--fff", "-fff arg", Argument::LONG, 1, false));
    argParser.addArgument(Argument("--hhh", "-hhh arg1...", Argument::LONG,
        Argument::INFINITY, true));
    argParser.addArgument(Argument("--iii", "-iii arg", Argument::LONG, 1, true));

    const char* cargv[] = {
        "test_program", "--aaa", "1", "--bbb", "2", "3", "--ccc", "--ddd", "foo", "bar",
        "--fff=abc", "--hhh", "1", "2", "3", "4", "5", "--iii", "blah"
    };
    char** argv = const_cast<char**>(cargv);
    ParsedArgument pa = argParser.parse(19, argv);

    EXPECT_EQ("1", pa.getValue("--aaa"));
    vector<string> args = pa.getValues("--bbb");
    EXPECT_EQ("2", args[0]);
    EXPECT_EQ("3", args[1]);
    EXPECT_TRUE(pa.hasArgument("--ccc"));
    EXPECT_EQ("", pa.getValue("--ccc"));
    args = pa.getValues("--ddd");
    EXPECT_EQ("foo", args[0]);
    EXPECT_EQ("bar", args[1]);
    EXPECT_FALSE(pa.hasArgument("--eee"));
    EXPECT_EQ("abc", pa.getValue("--fff"));
    args = pa.getValues("--hhh");
    EXPECT_EQ(5u, args.size());
    EXPECT_EQ("1", args[0]);
    EXPECT_EQ("2", args[1]);
    EXPECT_EQ("3", args[2]);
    EXPECT_EQ("4", args[3]);
    EXPECT_EQ("5", args[4]);
    EXPECT_EQ("blah", pa.getValue("--iii"));
}

TEST(ArgumentParserTest, ParseAllLongArgumentsHasAdditionalArgument) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("--aaa", "-aaa arg", Argument::LONG, 1, true));
    argParser.addArgument(Argument("--bbb", "-bbb arg1 arg2", Argument::LONG, 2, true));
    argParser.addArgument(Argument("--ccc", "-ccc", Argument::LONG, 0, true));
    argParser.addArgument(Argument("--ddd", "-ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("--eee", "-eee", Argument::LONG, 0, false));
    argParser.addArgument(Argument("--fff", "-fff arg", Argument::LONG, 1, false));

    const char* cargv[] = {
        "test_program", "--aaa", "1", "--bbb", "2", "3", "--ccc", "--ddd", "foo", "bar",
        "--fff=abc", "--hhh=whatever"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(12, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseAllLongArgumentsMissingMandatoryArgument) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("--aaa", "-aaa arg", Argument::LONG, 1, true));
    argParser.addArgument(Argument("--bbb", "-bbb arg1 arg2", Argument::LONG, 2, true));
    argParser.addArgument(Argument("--ccc", "-ccc", Argument::LONG, 0, true));
    argParser.addArgument(Argument("--ddd", "-ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("--eee", "-eee", Argument::LONG, 0, false));
    argParser.addArgument(Argument("--fff", "-fff arg", Argument::LONG, 1, false));

    const char* cargv[] = {
        "test_program", "--bbb", "2", "3", "--ccc", "--ddd", "foo", "bar",
        "--fff=abc"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(9, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseAllLongArgumentsInvalidArgumentValue1) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("--aaa", "-aaa arg", Argument::LONG, 1, true));
    argParser.addArgument(Argument("--bbb", "-bbb arg1 arg2", Argument::LONG, 2, true));
    argParser.addArgument(Argument("--ccc", "-ccc", Argument::LONG, 0, true));
    argParser.addArgument(Argument("--ddd", "-ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("--eee", "-eee", Argument::LONG, 0, false));
    argParser.addArgument(Argument("--fff", "-fff arg", Argument::LONG, 1, false));

    const char* cargv[] = {
        "test_program", "--aaa", "1", "--bbb", "2", "3", "xxx", "--ccc", "--ddd",
        "foo", "bar", "--fff=abc"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(12, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseAllLongArgumentsInvalidArgumentValue2) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("--aaa", "-aaa arg", Argument::LONG, 1, true));
    argParser.addArgument(Argument("--bbb", "-bbb arg1 arg2", Argument::LONG, 2, true));
    argParser.addArgument(Argument("--ccc", "-ccc", Argument::LONG, 0, true));
    argParser.addArgument(Argument("--ddd", "-ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("--eee", "-eee", Argument::LONG, 0, false));
    argParser.addArgument(Argument("--fff", "-fff arg", Argument::LONG, 1, false));

    const char* cargv[] = {
        "test_program", "--aaa", "1", "--bbb", "2", "--ccc", "--ddd",
        "foo", "bar", "--fff=abc"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(10, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseMixedArguments) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "--aaa", "-a arg", 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("--ccc", "-c", "-c", 0, true));
    argParser.addArgument(Argument("--ddd", "--ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("-e", "--eee", "--eee", 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));
    argParser.addArgument(Argument("-h", "--hhh", "-h arg1...", Argument::INFINITY, true));
    argParser.addArgument(Argument("--iii", "-iii arg", Argument::LONG, 1, true));


    const char* cargv[] = {
        "test_program", "--aaa=1", "-b", "2", "3", "--ccc", "--ddd", "foo", "bar",
        "-f", "abc", "-h", "1", "2", "3", "4", "5", "--iii", "blah"
    };
    char** argv = const_cast<char**>(cargv);
    ParsedArgument pa = argParser.parse(19, argv);

    EXPECT_EQ("1", pa.getValue("-a"));
    EXPECT_EQ("1", pa.getValue("--aaa"));
    vector<string> args = pa.getValues("-b");
    EXPECT_EQ("2", args[0]);
    EXPECT_EQ("3", args[1]);
    EXPECT_TRUE(pa.hasArgument("-c"));
    EXPECT_TRUE(pa.hasArgument("--ccc"));
    EXPECT_EQ("", pa.getValue("-c"));
    EXPECT_EQ("", pa.getValue("--ccc"));
    args = pa.getValues("--ddd");
    EXPECT_EQ("foo", args[0]);
    EXPECT_EQ("bar", args[1]);
    EXPECT_FALSE(pa.hasArgument("-e"));
    EXPECT_FALSE(pa.hasArgument("--eee"));
    EXPECT_EQ("abc", pa.getValue("-f"));
    args = pa.getValues("-h");
    EXPECT_EQ(5u, args.size());
    EXPECT_EQ("1", args[0]);
    EXPECT_EQ("2", args[1]);
    EXPECT_EQ("3", args[2]);
    EXPECT_EQ("4", args[3]);
    EXPECT_EQ("5", args[4]);
    args = pa.getValues("--hhh");
    EXPECT_EQ(5u, args.size());
    EXPECT_EQ("1", args[0]);
    EXPECT_EQ("2", args[1]);
    EXPECT_EQ("3", args[2]);
    EXPECT_EQ("4", args[3]);
    EXPECT_EQ("5", args[4]);
    EXPECT_EQ("blah", pa.getValue("--iii"));

}

TEST(ArgumentParserTest, ParseMixedArgumentsHasAdditionalArgument) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "--aaa", "-a arg", 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("--ccc", "-c", "-c", 0, true));
    argParser.addArgument(Argument("--ddd", "--ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("-e", "--eee", "--eee", 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));

    const char* cargv[] = {
        "test_program", "--aaa=1", "-b", "2", "3", "--ccc", "--ddd", "foo", "bar",
        "-f", "abc", "-h", "whatever"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(13, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseMixedArgumentsMissingMandatoryArgument) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "--aaa", "-a arg", 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("--ccc", "-c", "-c", 0, true));
    argParser.addArgument(Argument("--ddd", "--ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("-e", "--eee", "--eee", 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));

    const char* cargv[] = {
        "test_program", "-b", "2", "3", "--ccc", "--ddd", "foo", "bar",
        "-f", "abc"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(10, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseMixedArgumentsInvalidArgumentValue1) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "--aaa", "-a arg", 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("--ccc", "-c", "-c", 0, true));
    argParser.addArgument(Argument("--ddd", "--ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("-e", "--eee", "--eee", 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));

    const char* cargv[] = {
        "test_program", "--aaa=1", "-b", "2", "3", "xxx", "--ccc", "--ddd", "foo", "bar",
        "-f", "abc"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(12, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseMixedArgumentsInvalidArgumentValue2) {
    ArgumentParser argParser;
    argParser.addArgument(Argument("-a", "--aaa", "-a arg", 1, true));
    argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
    argParser.addArgument(Argument("--ccc", "-c", "-c", 0, true));
    argParser.addArgument(Argument("--ddd", "--ddd arg1 arg2", Argument::LONG, 2, false));
    argParser.addArgument(Argument("-e", "--eee", "--eee", 0, false));
    argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));

    const char* cargv[] = {
        "test_program", "--aaa=1", "-b", "2", "--ccc", "--ddd", "foo", "bar",
        "-f", "abc"
    };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(10, argv), InvalidArgumentException);
}

TEST(ArgumentParserTest, ParseValidArgumentWithValidator) {
    ArgumentParser argParser;
    PortNumberValidator validator;
    argParser.addArgument(Argument("-p", "--port", "Port Number", 1, true, &validator));

    const char* cargv[] = { "test_program", "--port=8888" };
    char** argv = const_cast<char**>(cargv);
    ParsedArgument pa = argParser.parse(2, argv);

    EXPECT_EQ("8888", pa.getValue("-p"));
    EXPECT_EQ("8888", pa.getValue("--port"));
}

TEST(ArgumentParserTest, ParseInvalidArgumentWithValidator) {
    ArgumentParser argParser;
    PortNumberValidator validator;
    argParser.addArgument(Argument("-p", "--port", "Port Number", 1, true, &validator));

    const char* cargv[] = { "test_program", "--port=99999" };
    char** argv = const_cast<char**>(cargv);
    EXPECT_THROW(argParser.parse(2, argv), InvalidArgumentException);
}
