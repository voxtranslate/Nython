#ifndef __DEFINITIONS__HPP
#define __DEFINITIONS__HPP

#ifndef __EXTENDS__HPP
#define __EXTENDS__HPP
#ifdef extends
#undef extends
#define extends :public
#else
#define extends :public
#endif // extends
#endif // __EXTENDS__HPP

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__)
#define OS_WINDOWS
#elif defined(__WIN64__) || defined(_WIN64) || defined(WIN64)
#define OS_WINDOWS64
#elif defined(unix) || defined(__unix) || defined(__unix__)
#define OS_UNIX
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined (__LINUX__) || defined(__gnu_linux__)
#define OS_LINUX
#elif defined(__APPLE__) || defined(__MACH__)
#define OS_APPLE
#elif defined (__ANDROID__) || defined(ANDROID)//Android
#define OS_ANDROID
#endif // defined

/* Boilerplate feature-test macros: */
#if _WIN32 || _WIN64
#ifndef _WIN32_WINNT
#  define _WIN32_WINNT  0x0A00 // _WIN32_WINNT_WIN10
#endif // _WIN32_WINNT
#ifndef NTDDI_VERSION
#  define NTDDI_VERSION 0x0A000002 // NTDDI_WIN10_RS1
#endif // NTDDI_VERSION
#  include <sdkddkver.h>
#else
#  define _XOPEN_SOURCE     700
#  define _POSIX_C_SOURCE   200809L
#endif

#ifndef MS_STDLIB_BUGS // Allow overriding the auto-detection.
/* The Microsoft C and C++ runtime libraries that ship with Visual Studio, as
 * of 2017, have a bug that neither stdio, iostreams or wide iostreams can
 * handle Unicode input or output.  Windows needs some non-standard magic to
 * work around that.  This includes programs compiled with MinGW and Clang
 * for the win32 and win64 targets.
 *
 * NOTE TO USERS OF TDM-GCC: This code is known to break on tdm-gcc 4.9.2. As
 * a workaround, "-D MS_STDLIB_BUGS=0" will at least get it to compile, but
 * Unicode output will still not work.
 */
#  if ( _MSC_VER || __MINGW32__ || __MSVCRT__ )
    /* This code is being compiled either on MS Visual C++, or MinGW, or
     * clang++ in compatibility mode for either, or is being linked to the
     * msvcrt (Microsoft Visual C RunTime) library.
     */
#    define MS_STDLIB_BUGS 1
#  else
#    define MS_STDLIB_BUGS 0
#  endif
#endif

#if MS_STDLIB_BUGS
#  include <io.h>
#  include <conio.h>
#  include <fcntl.h>
#endif

#if defined(OS_WINDOWS) || defined(OS_WINDOWS64)
#   include <windows.h>
#   include <wincon.h>
#endif // defined

#ifdef __clang__
#    pragma clang system_header
#elif defined __GNUC__
#    pragma GCC system_header
#endif

// #included from: internal/catch_suppress_warnings.h

#ifdef __clang__
#   ifdef __ICC // icpc defines the __clang__ macro
#       pragma warning(push)
#       pragma warning(disable: 161 1682)
#   else // __ICC
#       pragma clang diagnostic ignored "-Wglobal-constructors"
#       pragma clang diagnostic ignored "-Wvariadic-macros"
#       pragma clang diagnostic ignored "-Wc99-extensions"
#       pragma clang diagnostic ignored "-Wunused-variable"
#       pragma clang diagnostic push
#       pragma clang diagnostic ignored "-Wpadded"
#       pragma clang diagnostic ignored "-Wc++98-compat"
#       pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#       pragma clang diagnostic ignored "-Wswitch-enum"
#       pragma clang diagnostic ignored "-Wcovered-switch-default"
#    endif
#elif defined __GNUC__
#    pragma GCC diagnostic ignored "-Wvariadic-macros"
#    pragma GCC diagnostic ignored "-Wunused-variable"
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wpadded"
#endif



#ifndef __ldouble__
#define __ldouble__
typedef long double ldouble;
#endif // __ldouble__

#ifndef _O_U8TEXT
#define _O_U8TEXT 0x00040000
#endif // _O_U8TEXT

#ifndef _O_U16TEXT
#define _O_U16TEXT 0x00020000
#endif // _O_U16TEXT

#ifndef _O_WTEXT
#define _O_WTEXT 0x00010000
#endif // _O_WTEXT


#ifndef __DISALLOW_COPY_AND_ASSIGN__HPP
#define __DISALLOW_COPY_AND_ASSIGN__HPP
#define DISALLOW_COPY_AND_ASSIGN(Cls) private: Cls(const Cls&) = delete; Cls& operator=(const Cls&) = delete;
#endif // __DISALLOW_COPY_AND_ASSIGN__HPP

#ifndef DEBUG
#define DEBUG 1  // set debug mode
#endif

#define LOG(x)                     \
  do {                             \
    if (DEBUG) {                   \
      std::cerr << x << std::endl; \
    }                              \
  } while (0)

// Defines for versions
#ifndef NYTHON_VERSION
  #define NYTHON_VERSION "0.1.0"
#endif

#define _GNU_SOURCE

// Defines for exit codes
#define NYTHON_EXIT_OK 0
#define NYTHON_EXIT_USAGE 64
#define NYTHON_EXIT_DATAERR 65
#define NYTHON_EXIT_SOFTWAREERR 70
#define NYTHON_EXIT_OSERR 71
#define NYTHON_EXIT_IOERR 74
#define NYTHON_EXIT_CONFIG 78


#include <map>
#include <regex>
#include <memory>
#include <locale>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <cfloat>
#include <codecvt>
#include <cstdlib>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>
#include <iostream>
#include <strstream>
#include <algorithm>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <cmath>
#include <cstdint>
#include <initializer_list>

#include <vector>
#include <thread>
#include <chrono>
#include <cwchar>
#include <locale>
#include <cerrno>
#include <cctype>
#include <iomanip>
#include <utility>
#include <unistd.h>
#include <unordered_map>

///3rd parties libraries
#include <utf8/utf8.h>
#include <replxx/util.h>
#include <replxx/replxx.hxx>
#include <cxxopts/cxxopts.hpp>

#include <fmt/os.h>
#include <fmt/args.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>
#include <fmt/printf.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/compile.h>
#include <fmt/ostream.h>

/*


#include <filesystem.hpp>

*/

#define CXXOPTS_USE_UNICODE 1


//namespace fs = ghc::filesystem;



namespace nython::exception {

  // Class that defines a base for all exceptions and errors
  class Exception {
    private:
      // The message of the exception
      std::string message_;

      // The previous exception that caused the exception to happen
      Exception* previous_;


    public:
      // Constructor
      inline Exception(std::string message, Exception* previous) : message_(message), previous_(previous) {}
      inline Exception(std::string message) : Exception(message, nullptr) {}

      // Return the message of the exception
      inline std::string message() { return message_; }

      virtual inline std::string what() noexcept {
            return message_;
      }

      // Return the previous exception that caused the exception to happen
      inline Exception* previous() { return previous_; }
  };
}


namespace nython::io {
    using std::vector;
    using std::string;

    extern string file_name;
    extern vector<string> lines; // stores the lines of the input file
}

namespace nython::utils {

inline std::vector<std::string> splitString(std::string str, char sep = ',') {
    std::vector<std::string> vecString;
    std::string item;

    std::stringstream stringStream(str);

    while (std::getline(stringStream, item, sep)) {
        vecString.push_back(item);
    }
    return vecString;
}

}


#define STRING_REMOVE_CHAR(str, ch) str.erase(std::remove(str.begin(), str.end(), ch), str.end())


#define DECLARE_ENUM_WITH_TYPE(E, T, ...)                                                                     \
    enum class E : T                                                                                          \
    {                                                                                                         \
        __VA_ARGS__                                                                                           \
    };                                                                                                        \
    static std::map<T, std::string> E##MapName(generateEnumMap<T>(#__VA_ARGS__));                             \
    inline std::ostream &operator<<(std::ostream &os, E enumTmp)                                              \
    {                                                                                                         \
        os << E##MapName[static_cast<T>(enumTmp)];                                                            \
        return os;                                                                                            \
    }                                                                                                         \
    inline size_t operator*(E enumTmp) { (void) enumTmp; return E##MapName.size(); }                                 \
    inline std::string operator~(E enumTmp) { return E##MapName[static_cast<T>(enumTmp)]; }                          \
    inline std::string operator+(std::string &&str, E enumTmp) { return str + E##MapName[static_cast<T>(enumTmp)]; } \
    inline std::string operator+(E enumTmp, std::string &&str) { return E##MapName[static_cast<T>(enumTmp)] + str; } \
    inline std::string &operator+=(std::string &str, E enumTmp)                                                      \
    {                                                                                                         \
        str += E##MapName[static_cast<T>(enumTmp)];                                                           \
        return str;                                                                                           \
    }                                                                                                         \
    inline E operator++(E &enumTmp)                                                                           \
    {                                                                                                         \
        auto iter = E##MapName.find(static_cast<T>(enumTmp));                                                 \
        if (iter == E##MapName.end() || std::next(iter) == E##MapName.end())                                  \
            iter = E##MapName.begin();                                                                        \
        else                                                                                                  \
        {                                                                                                     \
            ++iter;                                                                                           \
        }                                                                                                     \
        enumTmp = static_cast<E>(iter->first);                                                                \
        return enumTmp;                                                                                       \
    }                                                                                                         \
    inline bool valid##E(T value) { return (E##MapName.find(value) != E##MapName.end()); }

#define DECLARE_ENUM(E, ...) DECLARE_ENUM_WITH_TYPE(E, int32_t, __VA_ARGS__)


template <typename T>
inline std::map<T, std::string> generateEnumMap(std::string strMap) {
    STRING_REMOVE_CHAR(strMap, ' ');
    STRING_REMOVE_CHAR(strMap, '(');
    std::vector<std::string> enumTokens(nython::utils::splitString(strMap));
    std::map<T, std::string> retMap;
    T inxMap;
    inxMap = 0;
    for (auto iter = enumTokens.begin(); iter != enumTokens.end(); ++iter) {
        // Token: [EnumName | EnumName=EnumValue]
        std::string enumName;
        /// T enumValue;
        if (iter->find('=') == std::string::npos) {
            enumName = *iter;
        } else {
            std::vector<std::string> enumNameValue(nython::utils::splitString(*iter, '='));
            enumName = enumNameValue[0];
            //inxMap = static_cast<T>(enumNameValue[1]);
            if (std::is_unsigned<T>::value) {
                inxMap = static_cast<T>(std::stoull(enumNameValue[1], 0, 0));
            }else {
                inxMap = static_cast<T>(std::stoll(enumNameValue[1], 0, 0));
            }
        }
        retMap[inxMap++] = enumName;
    }

    return retMap;
}

#endif // __DEFINITIONS__HPP
