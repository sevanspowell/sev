#pragma once

#include <sv/ClientVariables.h>
#include <sv/ProgramOptions.h>
#include <sv/console/Console.h>
#include <sv/input/Input.h>
#include <sv/resource/ResourceCache.h>

namespace sv {
class Client {
  public:
    Client() : resourceCache(10) {}

    bool initialize(const ProgramOptions &options);

  private:
    ClientVariables cvars;
    Console console;
    Input input;
    ResourceCache resourceCache;
};
}
