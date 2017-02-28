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
        sv::LogArea::Enum::Common | sv::LogArea::Enum::Console,
        sv::LogLevel::Enum::All));
    std::string logTxt("Hello world");
    sv::LogMessage msg(sv::LogArea::Enum::Common, sv::LogLevel::Enum::Error,
                       logTxt);

    distributor.registerObserver(observer);
    distributor.logMessage(msg);

    EXPECT_TRUE(observer->buffer == logTxt);
}

// Test that the 'All' enum works
TEST(Log, ObserveAll) {
    sv::LogDistributor distributor;

    std::shared_ptr<sv::BufferLog> observer(
        new sv::BufferLog(sv::LogArea::Enum::All, sv::LogLevel::Enum::All));
    distributor.registerObserver(observer);

    std::string logTxt("1");
    sv::LogMessage msg1(sv::LogArea::Enum::Common, sv::LogLevel::Enum::Event,
                        logTxt);
    logTxt = std::string("2");
    sv::LogMessage msg2(sv::LogArea::Enum::Console, sv::LogLevel::Enum::Debug,
                        logTxt);
    logTxt = std::string("3");
    sv::LogMessage msg3(sv::LogArea::Enum::Console, sv::LogLevel::Enum::Warning,
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
        sv::LogArea::Enum::Common | sv::LogArea::Enum::Console,
        sv::LogLevel::Enum::All));
    std::string logTxt("Hello world");
    sv::LogMessage msg(sv::LogArea::Enum::Common, sv::LogLevel::Enum::Error,
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
//     globals::logger =
//         std::unique_ptr<sv::LogDistributor>(new sv::LogDistributor);

//     std::shared_ptr<sv::BufferLog> observer(
//         new sv::BufferLog(sv::LogArea::Enum::All, sv::LogLevel::Enum::All));
//     globals::logger->registerObserver(observer);

//     std::string logTxt("Hiya!");
//     globals::log(sv::LogArea::Enum::Common, sv::LogLevel::Enum::Event,
//                     logTxt);

//     EXPECT_TRUE(observer->buffer == logTxt);
// }
