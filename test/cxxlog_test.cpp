#include "cxxlog.hpp"

#include <string>
#include <fstream>
#include <sstream>
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

TEST(TestCXXLog, STDCStream)
{
    // use std::ostringstream
    std::ostringstream oss;
    STDCStream::SetStream(oss);
    // test verbosity
    TrivialLog::Verbosity() = verbosity::verbose;
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    TrivialLog::Verbosity() = verbosity::normal;
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    TrivialLog::Verbosity() = verbosity::minimal;
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    TrivialLog::Verbosity() = verbosity::quiet;
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';

    // test severity
    TrivialLog::Severity() = severity::info;
    TrivialLog::Verbosity() = verbosity::minimal;
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    TRIVIAL_LOG(debug) << "test cxxlog!" << '\n';

    // check log oss, will ignore the first two lines.
    // file should look like:
    // XXX | XXX | XXX | XXX | test cxxlog!
    // XXX | XXX | XXX | test cxxlog!
    // test cxxlog!
    // test cxxlog!

    // set up baseline
    std::string baseline = "test cxxlog!\ntest cxxlog!\n";
    // remove first two lines
    std::istringstream iss(oss.str());
    std::string dummyLine;
    getline(iss, dummyLine);
    getline(iss, dummyLine);
    std::string log((std::istreambuf_iterator<char>(iss)),
        std::istreambuf_iterator<char>());

    EXPECT_TRUE(log == baseline);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
