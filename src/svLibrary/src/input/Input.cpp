#include <sv/input/Input.h>

namespace sv {
// \returns Binding associated with given key name or nullptr if no binding
// exists for that key name.
Binding *Input::findBinding(const std::string &keyName) {
    Binding *binding = nullptr;

    for (size_t i = 0; i < bindings.size(); ++i) {
        Binding *bindingIt = &bindings[i];

        if (bindingIt->keyName == keyName) {
            binding = bindingIt;
            break;
        }
    }

    return binding;
}

// \returns Binding associated with given key name or nullptr if no binding
// exists for that key name.
const Binding *Input::findBinding(const std::string &keyName) const {
    const Binding *binding = nullptr;

    for (size_t i = 0; i < bindings.size(); ++i) {
        const Binding *bindingIt = &bindings[i];

        if (bindingIt->keyName == keyName) {
            binding = bindingIt;
            break;
        }
    }

    return binding;
}

std::string Input::getBinding(const std::string &keyName) const {
    std::string cmdString;

    const Binding *binding = findBinding(keyName);
    if (binding != nullptr) {
        cmdString = binding->cmdString;
    }

    return cmdString;
}

void Input::clearAllBindings() { bindings.clear(); }

void Input::bind(const std::string &keyName, const std::string &commandStr) {
    Binding *binding = findBinding(keyName);

    // if no binding already exists for that key, add one
    if (binding == nullptr) {
        bindings.push_back(Binding(keyName, commandStr));
    }
    // else change existing binding
    else {
        binding->cmdString = commandStr;
    }
}
}
