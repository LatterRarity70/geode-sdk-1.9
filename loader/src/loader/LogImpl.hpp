#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Types.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

namespace geode::log {
    class Logger {
    private:
        bool m_initialized = false;
        std::vector<Log> m_logs;
        std::ofstream m_logStream;
        std::filesystem::path m_logPath;

        Logger() = default;
    public:
        static Logger* get();

        void setup();

        void push(Severity sev, std::string&& thread, std::string&& source, int32_t nestCount,
            std::string&& content);

        std::vector<Log> const& list();
        Severity getConsoleLogLevel();
        Severity getFileLogLevel();
        void clear();

        std::filesystem::path const& getLogPath() const;

        void deleteOldLogs(size_t maxAgeHours);

        template <typename Rep, typename Period>
        void deleteOldLogs(std::chrono::duration<Rep, Period> const& maxAge) {
            this->deleteOldLogs(std::chrono::duration_cast<std::chrono::hours>(maxAge).count());
        }
    };

    class Nest::Impl {
    public:
        int32_t m_nestLevel;
        int32_t m_nestCountOffset;
        Impl(int32_t nestLevel, int32_t nestCountOffset);
    };
}
