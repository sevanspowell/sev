#include <cassert>

#include <sv/platform/Platform.h>

namespace sv {
void Platform::queueEvent(const PlatformEvent &event) { queue.push(event); }

// Platform &Platform::getSingleton() {
//     assert(singleton);
//     return (*singleton);
// }

// Platform *Platform::getSingletonPtr() { return singleton; }
}
