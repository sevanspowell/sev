#include <sv/resource/Resource.h>

namespace sv {
bool operator<(const Resource &r1, const Resource &r2) {
    return (r1.name < r2.name);
}
}
