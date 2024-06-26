#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cmath>

namespace NaM
{
    namespace CppScratch
    {
        const std::string DASHES(std::string(70, '-'));
        const std::string EQUALS(std::string(70, '='));
        const std::string HASHES(std::string(70, '#'));
        const std::string calcNullStr()
        {
            std::stringstream ss;
            ss << "0x" << (void*)(nullptr);
            return ss.str();
        }
        const std::string nullvalstr(calcNullStr());
        const std::string pathSeparators("\\/:");
        const std::string wsSeparators("\t \v");
        const std::string lineEndSeparators("\r\n");

        //------------------------------------------------------------------------
        const std::string& TrueOrFalse(const bool& value)
        {
            static const std::string trueString{"true"};
            static const std::string falseString{"false"};
            return (value ? trueString : falseString);
        }

        std::string LastToken(const std::string& in, const std::string &seps = wsSeparators)
        {
            std::size_t lastpos(in.find_last_of(seps));
            if (in.empty())
            {
                return std::string{""};
            }
            else if (lastpos == std::string::npos)
            {
                return in;
            }
            else
            {
                std::string outString(in.substr(lastpos + 1));
                return outString;
            }
        }

        std::string FirstToken(const std::string& in, const std::string& seps = wsSeparators)
        {
            std::size_t firstpos(in.find_first_of(seps));
            if (in.empty())
            {
                return std::string{""};
            }
            else if (firstpos == std::string::npos)
            {
                return in;
            }
            else
            {
                std::string outString(in.substr(0, firstpos));
                return outString;
            }
        }

        std::string SplitLastToken(const std::string& in, std::string& remainder, const std::string& seps = wsSeparators)
        {
            std::size_t lastpos(in.find_last_of(seps));
            if (in.empty())
            {
                return std::string{""};
            }
            else if (lastpos == std::string::npos)
            {
                remainder.clear();
                return in;
            }
            else
            {
                std::string outString(in.substr(lastpos + 1));
                remainder.assign(in.substr(0, lastpos));
                return outString;
            }
        }

        std::string SplitFirstToken(const std::string& in, std::string& remainder, const std::string& seps = wsSeparators)
        {
            std::size_t firstpos(in.find_first_of(seps));
            if (in.empty())
            {
                return std::string{""};
            }
            else if (firstpos == std::string::npos)
            {
                remainder.clear();
                return in;
            }
            else
            {
                std::string outString(in.substr(0, firstpos));
                remainder.assign(in.substr(firstpos+1));
                return outString;
            }
        }

        /**
         * Calculate the number of chars needed to display a number
         */
        template<typename T> // how do I limit this to a integer type?
        const size_t TextNumLength(const T& num)
        {
            return (num == 0 ?
                    1 : 
                    static_cast<size_t>(std::floor(std::log10(std::abs(static_cast<double>(num)))) + (num < 0 ? 2 : 1)));
        }

        constexpr const char* const _defaultXofYSep{ "/" };

        template <typename T>
        std::string XofYStr(const T& x, const T& y, const std::string& sep = _defaultXofYSep)
        {
            // first, don't be stupid
            if (x > y)
            {
                return XofYStr(y, x);
            }
            else
            {
                std::stringstream ss;
                size_t w{ TextNumLength(y) };
                ss << std::setw(w) << x << sep << std::setw(w) << y;
                return ss.str();
            }
        }

        template <typename T>
        inline std::string XofYStr(const T& x, const T& y, const char& sep)
        {
            return XofYStr<T>(x, y, std::string(sep));
        }

        template<typename T>
        const std::string PtrString(const T* ptr)
        {
            std::stringstream ss;
            if (ptr)
            {
                ss << "0x"
                    << std::hex << std::setfill('0') << std::setw(sizeof(ptr))
                    << (void*)ptr
                    << std::dec;
            }
            else
            {
                ss << NaM::CppScratch::nullvalstr;
            }
            return ss.str();
        }

        //------------------------------------------------------------------------
        namespace Path
        {
            /**
             * Get the file part of a passed in path
             */
            std::string BaseName(const std::string& path, const std::string& sep = pathSeparators)
            {
                size_t pos;
                pos = path.find_last_of(sep);
                return std::string { pos == std::string::npos ? path : path.substr(pos+1) };
            }

            /**
             * Get the folder part of a passed in path
             */
            std::string DirName(const std::string& path, const std::string& sep = pathSeparators)
            {
                size_t pos;
                pos = path.find_last_of(sep);
                return std::string { pos == std::string::npos ? path : path.substr(0, pos) };
            }

            /**
             * Get the name of a file without the extension
             *
             * NOTE: For now this is lazy and assumes the first '.' is the beginning of the extension
             */
            std::string FileShortName(const std::string& path, const std::string& sep = pathSeparators)
            {
                return FirstToken(BaseName(path, sep), ".");
            }

            constexpr static const char defaultWinSep = '\\';
            constexpr static const char defaultMacSep = ':';
            constexpr static const char defaultPosixSep = '/';
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            constexpr static const char defaultPathSep = defaultWinSep;
#elif __APPLE__
            constexpr static const char defaultPathSep = defaultMacSep;
#elif __linux__
            constexpr static const char defaultPathSep = defaultPosixSep;
#elif __unix__ // all unices not caught above
            constexpr static const char defaultPathSep = defaultPosixSep;
#elif defined(_POSIX_VERSION)
            constexpr static const char defaultPathSep = defaultPosixSep;
#else
            // nfc what they have
            constexpr static const char defaultPathSep = defaultPosixSep;
#endif

            /**
             * Conditionally append a delimiter to the end of a string
             */
            std::string AppendDelimiter(const std::string& path, const char delimiter = defaultPathSep)
            {
                std::string s{ path };
                if (delimiter && !path.empty())
                {
                    if (s.back() != delimiter)
                    {
                        s.push_back(delimiter);
                    }
                }
                return s;
            }

            /**
             * Conditionally remove a delimiter from the end of a string
             */
            std::string StripDelimiter(const std::string& path, const char delimiter = defaultPathSep)
            {
                std::string s{ path };
                if (delimiter && !path.empty())
                {
                    if (s.back() == delimiter)
                    {
                        s.pop_back();
                    }
                }
                return s;
            }
        };

        //------------------------------------------------------------------------
        class _CounterVal
        {
        private:
            static std::uint64_t ms_count;
        public:
            [[nodiscard]] inline const std::uint64_t GetCount() const { return ms_count; }
            [[nodiscard]] inline const std::uint64_t Count() const { return ++ms_count; }
        };
        std::uint64_t _CounterVal::ms_count = 0;
        std::ostream& operator<<(std::ostream& oss, const _CounterVal& counter)
        {
            oss << std::dec << std::setfill(' ') << std::setw(3) << counter.Count() << ") ";
            return oss;
        }

        //------------------------------------------------------------------------
        class TestRunCerr
        {
        private:
            static std::uint64_t ms_runId;
            static size_t ms_depth;
            std::uint64_t m_runId;
            std::string m_name;
            const std::string& m_decorator;

        public:
            TestRunCerr() = delete;
            TestRunCerr(const TestRunCerr&) = delete;
            TestRunCerr(TestRunCerr&&) = delete;

            TestRunCerr(const std::string& name)
                : m_runId(++ms_runId), m_name(name), m_decorator(ms_depth == 0 ? EQUALS : DASHES)
            {
                ++ms_depth;
                std::cerr << m_decorator << std::endl;
                std::cerr << ">>> Start run " << RunId() << ": " << Name() << " >>>" << std::endl;
                std::cerr << m_decorator << std::endl;
            }
            ~TestRunCerr()
            {
                std::cerr << m_decorator << std::endl;
                std::cerr << "<<< End run " << RunId() << ": " << Name() << " <<<" << std::endl;
                std::cerr << m_decorator << std::endl;
                --ms_depth;
                if (ms_depth == 0)
                    std::cerr << std::endl;
                m_name.clear();
                m_runId = std::uint64_t(0);
            }

            [[nodiscard]] inline const std::uint64_t RunId() const { return m_runId; }
            [[nodiscard]] inline const std::string& Name() const { return m_name; }

            TestRunCerr& operator=(const TestRunCerr&) = delete;
            TestRunCerr& operator=(TestRunCerr&) = delete;
        };
        std::uint64_t TestRunCerr::ms_runId = 0;
        size_t TestRunCerr::ms_depth = 0;

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
