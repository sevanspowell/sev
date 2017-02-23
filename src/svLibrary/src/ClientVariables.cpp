#include <algorithm>
#include <sstream>

#include <sv/ClientVariables.h>

namespace sv {
bool ClientVariables::getFloatValue(const std::string &variableName,
                                    float *valueOut) const {
    bool result = false;

    if (valueOut != nullptr) {
        // Find variable
        std::vector<ClientVariable>::const_iterator it =
            std::find_if(variables.begin(), variables.end(),
                         [&](const ClientVariable &varIt) {
                             return varIt.name == variableName;
                         });

        // If found
        if (it != variables.end()) {
            // Check if valid float
            if (isFloat(it->value)) {
                // Set valueOut
                std::istringstream iss(it->value);
                iss >> std::noskipws >> *valueOut;
                result = true;
            }
        }
    }

    return result;
}

bool ClientVariables::getIntValue(const std::string &variableName,
                                  int *valueOut) const {
    bool result = false;

    if (valueOut != nullptr) {
        // Find variable
        std::vector<ClientVariable>::const_iterator it =
            std::find_if(variables.begin(), variables.end(),
                         [&](const ClientVariable &varIt) {
                             return varIt.name == variableName;
                         });

        // If found
        if (it != variables.end()) {
            // Check if valid int
            if (isInt(it->value)) {
                // Set valueOut
                std::istringstream iss(it->value);
                iss >> std::noskipws >> *valueOut;
                result = true;
            }
        }
    }

    return result;
}

bool ClientVariables::getStringValue(const std::string &variableName,
                                     std::string *valueOut) const {
    bool result = false;

    if (valueOut != nullptr) {
        // Find variable
        std::vector<ClientVariable>::const_iterator it =
            std::find_if(variables.begin(), variables.end(),
                         [&](const ClientVariable &varIt) {
                             return varIt.name == variableName;
                         });

        // If found
        if (it != variables.end()) {
            // Set valueOut
            *valueOut = it->value;
            result    = true;
        }
    }

    return result;
}

void ClientVariables::setFloatValue(const std::string &variableName,
                                    float value) {
    // Convert float to string
    std::ostringstream buffer;
    buffer << value;

    // Try to find existing variable with given name
    std::vector<ClientVariable>::iterator it = std::find_if(
        variables.begin(), variables.end(), [&](const ClientVariable &varIt) {
            return varIt.name == variableName;
        });

    // If found, set it's value
    if (it != variables.end()) {
        it->value = buffer.str();
    }
    // Else create new client variable
    else {
        variables.push_back(ClientVariable(variableName, buffer.str()));
    }
}

void ClientVariables::setIntValue(const std::string &variableName, int value) {
    // Convert int to string
    std::ostringstream buffer;
    buffer << value;

    // Try to find existing variable with given name
    std::vector<ClientVariable>::iterator it = std::find_if(
        variables.begin(), variables.end(), [&](const ClientVariable &varIt) {
            return varIt.name == variableName;
        });

    // If found, set it's value
    if (it != variables.end()) {
        it->value = buffer.str();
    }
    // Else create new client variable
    else {
        variables.push_back(ClientVariable(variableName, buffer.str()));
    }
}

void ClientVariables::setStringValue(const std::string &variableName,
                                     const std::string &value) {
    // Try to find existing variable with given name
    std::vector<ClientVariable>::iterator it = std::find_if(
        variables.begin(), variables.end(), [&](const ClientVariable &varIt) {
            return varIt.name == variableName;
        });

    // If found, set it's value
    if (it != variables.end()) {
        it->value = value;
    }
    // Else create new client variable
    else {
        variables.push_back(ClientVariable(variableName, value));
    }
}

void ClientVariables::removeVariable(const std::string &variableName) {
    for (size_t i = 0; i < variables.size(); ++i) {
        if (variables[i].name == variableName) {
            // Copy last element to this element (element to be removed) and pop
            // last element off vector
            variables[i] = variables[variables.size() - 1];
            variables.pop_back();
        }
    }
}

bool isFloat(const std::string &value) {
    // http://stackoverflow.com/questions/447206/c-isfloat-function - Bill the
    // Lizard
    std::istringstream iss(value);
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

bool isInt(const std::string &value) {
    std::istringstream iss(value);
    int i;
    iss >> std::noskipws >> i;
    return iss.eof() && !iss.fail();
}
}
