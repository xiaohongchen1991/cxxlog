#include "cxxlog.hpp"

#include <string>
#include <fstream>
#include <cstdio>

#include "gtest/gtest.h"

using namespace cxxlog;

TEST(TestCXXLog, FileStream)
{
    // create log file tmp.log
    std::string file = "tmp.log";
    RegisterLogFile log_file(file);

    // test verbosity
    FileLog::Verbosity() = verbosity::verbose;
    FILE_LOG(info) << "test cxxlog!" << '\n';
    FileLog::Verbosity() = verbosity::normal;
    FILE_LOG(info) << "test cxxlog!" << '\n';
    FileLog::Verbosity() = verbosity::minimal;
    FILE_LOG(info) << "test cxxlog!" << '\n';
    FileLog::Verbosity() = verbosity::quiet;
    FILE_LOG(info) << "test cxxlog!" << '\n';

    // test severity
    FileLog::Severity() = severity::info;
    FileLog::Verbosity() = verbosity::minimal;
    FILE_LOG(info) << "test cxxlog!" << '\n';
    FILE_LOG(debug) << "test cxxlog!" << '\n';

    // check log file, will ignore the first two lines.
    // file should look like:
    // XXX | XXX | XXX | XXX | test cxxlog!
    // XXX | XXX | XXX | test cxxlog!
    // test cxxlog!
    // test cxxlog!
    std::ifstream stream(file);
    std::string dummyLine;
    getline(stream, dummyLine);
    getline(stream, dummyLine);
    std::string log((std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>());
    // set up baseline
    std::string baseline = "test cxxlog!\ntest cxxlog!\n";
    EXPECT_TRUE(log == baseline);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
