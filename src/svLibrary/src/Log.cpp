#include <algorithm>

#include <sv/Log.h>

namespace sv {
uint32_t LogObserver::getAreas() { return areas; }

uint32_t LogObserver::getLevels() { return levels; }

void LogDistributor::registerObserver(
    const std::shared_ptr<LogObserver> &observer) {
    if (observer.get() != nullptr) {
        observerList.push_back(observer);
    }
}

void LogDistributor::removeObserver(
    const std::shared_ptr<LogObserver> &observer) {
    // Try to find element to remove, swap with last element and remove last
    // element from vector
    for (size_t i = 0; i < observerList.size(); ++i) {
        if (observerList[i] == observer) {
            observerList[i] = observerList[observerList.size() - 1];
            observerList.pop_back();
        }
    }
}

void LogDistributor::logMessage(const LogMessage &message) {
    for (size_t i = 0; i < observerList.size(); ++i) {
        // If observer observing this area and level of log message
        if ((observerList[i]->getAreas() &
             static_cast<uint32_t>(message.area)) &&
            (observerList[i]->getLevels() &
             static_cast<uint32_t>(message.level))) {
            // Then write log message
            observerList[i]->write(message);
        }
    }
}
}
