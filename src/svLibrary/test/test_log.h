#include <iostream>
#include <memory>

#include <sv/Globals.h>
#include <sv/Log.h>

namespace sv {
class BufferLog : public LogObserver {
  public:
    BufferLog(uint32_t observeAreas, uint32_t observeLevels)
        : LogObserver(observeAreas, observeLevels) {}

    virtual void write(const LogMessage &message) { buffer += message.text; }

    std::string buffer;
};
}

// Simple logging example
TEST(Log, LogMessage) {
    sv::LogDistributor distributor;
    std::shared_ptr<sv::BufferLog> observer(new sv::BufferLog(
        sv::LogArea::Enum::COMMON | sv::LogArea::Enum::CONSOLE,
        sv::LogLevel::Enum::ALL));
    std::string logTxt("Hello world");
    sv::LogMessage msg(sv::LogArea::Enum::COMMON, sv::LogLevel::Enum::ERROR,
                       logTxt);

    distributor.registerObserver(observer);
    distributor.logMessage(msg);

    EXPECT_TRUE(observer->buffer == logTxt);
}

// Test that the 'ALL' enum works
TEST(Log, ObserveAll) {
    sv::LogDistributor distributor;

    std::shared_ptr<sv::BufferLog> observer(
        new sv::BufferLog(sv::LogArea::Enum::ALL, sv::LogLevel::Enum::ALL));
    distributor.registerObserver(observer);

    std::string logTxt("1");
    sv::LogMessage msg1(sv::LogArea::Enum::COMMON, sv::LogLevel::Enum::EVENT,
                        logTxt);
    logTxt = std::string("2");
    sv::LogMessage msg2(sv::LogArea::Enum::CONSOLE, sv::LogLevel::Enum::DEBUG,
                        logTxt);
    logTxt = std::string("3");
    sv::LogMessage msg3(sv::LogArea::Enum::CONSOLE, sv::LogLevel::Enum::WARNING,
                        logTxt);

    distributor.logMessage(msg1);
    distributor.logMessage(msg2);
    distributor.logMessage(msg3);

    EXPECT_TRUE(observer->buffer == "123");
}

// Test removing observers works
TEST(Log, RemoveObserver) {
    sv::LogDistributor distributor;
    std::shared_ptr<sv::BufferLog> observer(new sv::BufferLog(
        sv::LogArea::Enum::COMMON | sv::LogArea::Enum::CONSOLE,
        sv::LogLevel::Enum::ALL));
    std::string logTxt("Hello world");
    sv::LogMessage msg(sv::LogArea::Enum::COMMON, sv::LogLevel::Enum::ERROR,
                       logTxt);

    distributor.registerObserver(observer);
    distributor.logMessage(msg);

    EXPECT_TRUE(observer->buffer == logTxt);

    distributor.removeObserver(observer);
    distributor.logMessage(msg);
    EXPECT_TRUE(observer->buffer == logTxt);
}

// Test that log global works
// TEST(Log, GlobalLog) {
//     sv_globals::logger =
//         std::unique_ptr<sv::LogDistributor>(new sv::LogDistributor);

//     std::shared_ptr<sv::BufferLog> observer(
//         new sv::BufferLog(sv::LogArea::Enum::ALL, sv::LogLevel::Enum::ALL));
//     sv_globals::logger->registerObserver(observer);

//     std::string logTxt("Hiya!");
//     sv_globals::log(sv::LogArea::Enum::COMMON, sv::LogLevel::Enum::EVENT,
//                     logTxt);

//     EXPECT_TRUE(observer->buffer == logTxt);
// }
