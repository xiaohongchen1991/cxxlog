# Release versions

version 1.0.0:

A simple fast and thread-safe C++ logging library.

# Quick start

This is a simple C++ library for logging. By default, two logging modes are supported, log to standard C output and log to a given file. The library also supports customization for different output stream and attributes.

## TrivailLog Mode

TrivailLog mode sets default output stream to std::cout and has two attributes: ThreadID and TimeStamp. See its definition.

    using TrivialLog = Log<STDCStream, ThreadID, TimeStamp>;
    
A convinient helper MACRO TRIVIAL_LOG is provided to use this mode. For example:

    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    
In this library, six different severity levels are provided, namely, trace, debug, info, warning, error and fatal. The severity level is set in the helper MACRO argument. For each Log type, the default report severity level is info. To change it, do

    TrivialLog::Severity() = severity::warning;
    // level info is below report severity level warning and hence is filtered out
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    // OK! This will be printed out
    TRIVIAL_LOG(warning) << "test cxxlog!" << '\n';
    // restore default severity level
    TrivialLog::Severity() = severity::info;

One can also set the verbosity level for the logger. Four different levels are supported in this library, namely, quiet, miniaml, normal, verbose. By default level normal is selected. To change the verbosity level do:

    TrivialLog::Verbosity() = verbosity::verbose;
    // output:
    // ThreadID | TimeStamp | Severity | test cxxlog!
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    TrivialLog::Verbosity() = verbosity::normal;
    // ThreadID | TimeStamp | test cxxlog!
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    TrivialLog::Verbosity() = verbosity::minimal;
    // test cxxlog!
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    TrivialLog::Verbosity() = verbosity::quiet;
    // silent this output.
    TRIVIAL_LOG(info) << "test cxxlog!" << '\n';
    
By default, the output stream for TrivailLog type is std::cout. One can also change it to any std::ostream type. For example:

    std::ostringstream oss;
    STDCStream::SetStream(oss);
    
sets output stream to std::ostringstream.

## FileLog Mode

FileLog mode sets default output stream to std::ofstream and has three attributes: Counter, ThreadID and TimeStamp. See its definition.

    using FileLog = Log<FileStream, Counter, ThreadID, TimeStamp>;
    
A convinient helper MACRO FILE_LOG is provided to use this mode. For example:

    FILE_LOG(info) << "test cxxlog!" << '\n';
    
The usage of FileLog mode is similar to that of TrivialLog mode, except that there is no default output stream. User has to set it up with an opened std::ofstream before using the logging utility. A helper RAII class RegisterLogFile is provided for settin up log file. To use it, do

    std::string file = "tmp.log";
    RegisterLogFile log_file(file);
    FILE_LOG(info) << "test cxxlog!" << '\n';

## Attributes

The libarary provides three different attributes to add into Log class.

1. Counter
   LineID, used to tell how many information has been logged.
   Format: No.#
   
2. ThreadID
   ThreadID, used to tell which thread logs this information.
   Format: T.#
   
3. TimeStamp
   TimeStamp, used to tell when the information is logged
   Format(ex): Sun Aug 11 11:48:08 2019
   
## Customization

One can customize what attributes to use and which type of output ostream to set by doing:

    // Use STDCStream as output and select all three attributes
    using CustomLog = Log<STDCStream, Counter, ThreadID, TimeStamp>;
    // Use FileStream as output and select attributes Counter and TimeStamp
    using CustomLog = Log<FileStream, Counter, TimeStamp>;
    
    #define CUSTOM_LOG(level) \
    if (level < CustomLog::Severity()); \
    else CustomLog().Get(level)
    
    CUSTOM_LOG(info) << "test cxxlog!" << '\n';

## Linking

This is a header only library.

## Requirements

The library only requires a C++ compiler that supports C++14.

## TODO list

* Add Format policy
