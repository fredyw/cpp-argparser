cpp-argparser
=============

A small C++ argument parser library

How to build
------------

### Building a static library ###
    make -f Makefile.static

### Building a shared library ###
    make -f Makefile.shared

Examples
--------
```c++
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

ArgumentParser argParser;
argParser.addArgument(Argument("-a", "-a arg", Argument::SHORT, 1, true));
argParser.addArgument(Argument("-b", "-b arg1 arg2", Argument::SHORT, 2, true));
argParser.addArgument(Argument("-c", "-c", Argument::SHORT, 0, true));
argParser.addArgument(Argument("-d", "-d arg1 arg2", Argument::SHORT, 2, false));
argParser.addArgument(Argument("-e", "-e", Argument::SHORT, 0, false));
argParser.addArgument(Argument("-f", "-f arg", Argument::SHORT, 1, false));
argParser.addArgument(Argument("-h", "-h arg1...", Argument::SHORT, Argument::INFINITY, true));
argParser.addArgument(Argument("-i", "-i arg", Argument::SHORT, 1, true));
PortNumberValidator validator;
argParser.addArgument(Argument("-p", "--port", "Port Number", 1, true, &validator)
// show the help menu
argParser.showHelp(string(argv[0]);

const char* cargv[] = {
    "test_program", "-a", "1", "-b", "2", "3", "-c", "-d", "foo", "bar",
    "-f", "abc", "-h", "1", "2", "3", "4", "5", "-i", "blah", "--port=8888"
};
char** argv = const_cast<char**>(cargv);
ParsedArgument pa = argParser.parse(21, argv);

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
EXPECT_EQ("1", args[0]);
EXPECT_EQ("2", args[1]);
EXPECT_EQ("3", args[2]);
EXPECT_EQ("4", args[3]);
EXPECT_EQ("5", args[4]);
EXPECT_EQ("blah", pa.getValue("-i"));
EXPECT_EQ("8888", pa.getValue("-p"));
EXPECT_EQ("8888", pa.getValue("--port"));
```
