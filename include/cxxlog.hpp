#ifndef CXXLOG_HPP
#define CXXLOG_HPP

#include "cxxlog_config.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>
#include <mutex>

// a simple logging class
namespace cxxlog {

enum severity {trace, debug, info, warning, error, fatal};

enum verbosity {quiet, minimal, normal, verbose};

namespace detail{
const std::string severity_string[6] = {"trace",
                                        "debug",
                                        "info",
                                        "warning",
                                        "error",
                                        "fatal"};
} // end namespace detail

template <typename OStream, typename... Attributes>
class Log
{
  public:
    Log() = default;
    Log(const Log&) = delete;
    Log& operator =(const Log&) = delete;
    virtual ~Log() {
        if (Verbosity() != verbosity::quiet) {
            OStream::Output(msg_.str());
        }
    }
    std::ostringstream& Get(severity level = severity::info) {
        switch (Verbosity()) {
        case verbosity::quiet:
            break;
        case verbosity::minimal:
            break;
        case verbosity::normal:
            OutputAttributes<Attributes...>(msg_);
            break;
        case verbosity::verbose:
            OutputAttributes<Attributes...>(msg_);
            msg_ << std::setw(7) <<  detail::severity_string[level] << " | ";
            break;
        }
        return msg_;
    }

    static severity& Severity() {
        static severity level = severity::info;
        return level;
    }

    static verbosity& Verbosity() {
        static verbosity level = verbosity::normal;
        return level;
    }

  protected:
    template <typename Attrib>
    std::ostream& OutputAttributes(std::ostream& os) {
        Attrib::Output(os);
        os << " | ";
        return os;
    }

    template <typename Attrib1, typename... Attribs,
              typename = std::enable_if_t<(sizeof...(Attribs) > 0)> >
    std::ostream& OutputAttributes(std::ostream& os) {
        Attrib1::Output(os);
        os << " | ";
        return OutputAttributes<Attribs...>(os);
    }

    template <typename... Attribs,
              typename = std::enable_if_t<sizeof...(Attribs) == 0> >
    std::ostream& OutputAttributes(std::ostream& os) {
        return os;
    }

    std::ostringstream msg_;
};

// attributes
class Counter {
  public:
    static std::ostream& Output(std::ostream& os) {
        return os << "No. " << ++Count();
    }
  private:
    static int& Count() {
        static int n = 0;
        return n;
    }
};

class ThreadID {
  public:
    static std::ostream& Output(std::ostream& os) {
        return os << "T." << std::this_thread::get_id();
    }
};

class TimeStamp {
  public:
    static std::ostream& Output(std::ostream& os) {
        std::time_t current_time = std::time(nullptr);
        std::string current_time_string = std::ctime(&current_time);
        return os << current_time_string.substr(0, current_time_string.length() - 1);
    }
};

// ostreams
class FileStream {
  public:
    static void SetStream(std::ofstream& stream) {
        std::lock_guard<std::mutex> guard(Mutex());
        GetStream() = &stream;
    }
    static void Output(const std::string& msg) {
        std::lock_guard<std::mutex> guard(Mutex());
        std::ofstream* stream = GetStream();
        if (!stream || !stream->is_open())
            return;

        int tmp = msg.length();
        stream->write(msg.c_str(), tmp);
        stream->flush();
    }
  private:
    static std::ofstream*& GetStream() {
        static std::ofstream* stream = nullptr;
        return stream;
    }
    static std::mutex& Mutex() {
        static std::mutex m;
        return m;
    }
};

class STDCStream {
  public:
    static void SetStream(std::ostream& stream) {
        std::lock_guard<std::mutex> guard(Mutex());
        GetStream() = &stream;
    }
    static void Output(const std::string& msg) {
        std::lock_guard<std::mutex> guard(Mutex());
        std::ostream* stream = GetStream();

        stream->write(msg.c_str(), msg.length());
        stream->flush();
    }
  private:
    static std::ostream*& GetStream() {
        static std::ostream* stream = &std::cout;
        return stream;
    }
    static std::mutex& Mutex() {
        static std::mutex m;
        return m;
    }
};

// helper class to set output file
class RegisterLogFile {
  public:
    RegisterLogFile(const std::string& name)
        : ofs_(name)
    {
        FileStream::SetStream(ofs_);
    }
    ~RegisterLogFile() {
        Close();
    }
    void Close() {
        if (ofs_.is_open()) {
            ofs_.close();
        }
    }
  private:
    std::ofstream ofs_;
};


using FileLog = Log<FileStream, Counter, ThreadID, TimeStamp>;
using TrivialLog = Log<STDCStream, ThreadID, TimeStamp>;

#define FILE_LOG(level) \
    if (level < FileLog::Severity()); \
    else FileLog().Get(level)

#define TRIVIAL_LOG(level) \
    if (level < TrivialLog::Severity()); \
    else TrivialLog().Get(level)

} // end of namespace cxxlog
#endif
