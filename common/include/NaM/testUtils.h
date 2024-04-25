#pragma once

#include <iostream>
#include <sstream>

namespace NaM
{
    namespace CppScratch
    {
        const std::string& TrueOrFalse(const bool& value)
        {
            static const std::string trueString{"true"};
            static const std::string falseString{"false"};
            return (value ? trueString : falseString);
        }

        std::string LastToken(const std::string& in, const std::string seps = "\\/:")
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

        std::string FirstToken(const std::string& in, const std::string seps = "\\/:")
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
                std::string outString(in.substr(firstpos + 1));
                return outString;
            }
        }

        //------------------------------------------------------------------------
        const std::string DASHES(std::string(70, '-'));
        const std::string EQUALS(std::string(70, '='));
        const std::string HASHES(std::string(70, '#'));
        const std::string nullvalstr(std::string("0x").append(std::string(sizeof(nullptr_t), '0')));

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
