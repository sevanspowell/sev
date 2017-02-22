 #include <sv/input/Input.h>

namespace sv {
// \returns Binding associated with given key name or nullptr if no binding
// exists for that key name.
static Binding *findBinding(const std::shared_ptr<Bindings> &bindings,
                            const std::string &keyName) {
    Binding *binding = nullptr;

    for (size_t i = 0; i < bindings->size(); ++i) {
        Binding *bindingIt = &(*bindings)[i];

        if (bindingIt->keyName == keyName) {
            binding = bindingIt;
            break;
        }
    }

    return binding;
}

// \returns Binding associated with given key name or nullptr if no binding
// exists for that key name.
static const Binding *
findBinding(const std::shared_ptr<const Bindings> &bindings,
            const std::string &keyName) {
    const Binding *binding = nullptr;

    for (size_t i = 0; i < bindings->size(); ++i) {
        const Binding *bindingIt = &(*bindings)[i];

        if (bindingIt->keyName == keyName) {
            binding = bindingIt;
            break;
        }
    }

    return binding;
}

bool BindCommand::execute(Console *const console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 3) {
        console->appendToOutputBuffer(
            "Usage: bind \"KEY_NAME\" \"COMMAND_STRING\"\n");
    } else {
        Binding *binding = findBinding(bindings, argv[1]);
        if (binding == nullptr) {
            Binding createdBinding(argv[1], argv[2]);
            bindings->push_back(createdBinding);
        } else {
            binding->cmdString = std::string(argv[2]);
        }

        result = true;
    }

    return result;
}

bool UnbindAllCommand::execute(Console *const console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 1) {
        console->appendToOutputBuffer("Usage: unbindall\n");
    } else {
        bindings->clear();
        result = true;
    }

    return result;
}

std::string Input::getBinding(const std::string &keyName) const {
    std::string cmdString;

    const Binding *binding = findBinding(bindings, keyName);
    if (binding != nullptr) {
        cmdString = binding->cmdString;
    }

    return cmdString;
}
}
