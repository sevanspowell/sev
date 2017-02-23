//===-- sv/ClientVariables.h - Client var system ----------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief A client variable is a simple name-value association. This system
/// manages those variables.
///
/// A client variable is always stored as a string, but may be interpreted as a
/// float, int or string.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <string>
#include <vector>

namespace sv {

struct ClientVariable {
    ClientVariable() : name(""), value("") {}
    ClientVariable(const std::string &varName, const std::string &varValue)
        : name(varName), value(varValue) {}

    std::string name;
    std::string value;
};

/// Class for managing a number of client variables and their values.
class ClientVariables {
  public:
    ///-------------------------------------------------------------------------
    /// Get the client variable with the given name as a float if possible.
    ///
    /// @post Value of given valueOut parameter is not changed if this method
    /// returns false.
    ///
    /// \param     variableName     Name of client variable to get value of.
    /// \param     valueOut [out]   Float value out.
    /// \returns                    True if could get value as float
    /// successfuly, false otherwise.
    ///-------------------------------------------------------------------------
    bool getFloatValue(const std::string &variableName, float *valueOut) const;

    ///-------------------------------------------------------------------------
    /// Get the client variable with the given name as a int if possible.
    ///
    /// @post Value of given valueOut parameter is not changed if this method
    /// returns false.
    ///
    /// \param     variableName     Name of client variable to get value of.
    /// \param     valueOut [out]   Int value out.
    /// \returns                    True if could get value as int successfuly,
    /// false otherwise.
    ///-------------------------------------------------------------------------
    bool getIntValue(const std::string &variableName, int *valueOut) const;

    ///-------------------------------------------------------------------------
    /// Get the client variable with the given name as a string if possible.
    ///
    /// @post Value of given valueOut parameter is not changed if this method
    /// returns false.
    ///
    /// \param     variableName     Name of client variable to get value of.
    /// \param     valueOut [out]   String value out.
    /// \returns                    True if could get value as string
    /// successfuly, false otherwise.
    ///-------------------------------------------------------------------------
    bool getStringValue(const std::string &variableName,
                        std::string *valueOut) const;

    ///-------------------------------------------------------------------------
    /// Set the value of the client variable with the given name (float).
    ///-------------------------------------------------------------------------
    void setFloatValue(const std::string &variableName, float value);

    ///-------------------------------------------------------------------------
    /// Set the value of the client variable with the given name (int).
    ///-------------------------------------------------------------------------
    void setIntValue(const std::string &variableName, int value);

    ///-------------------------------------------------------------------------
    /// Set the value of the client variable with the given name (string).
    ///-------------------------------------------------------------------------
    void setStringValue(const std::string &variableName,
                        const std::string &value);

    ///-------------------------------------------------------------------------
    /// Remove the variable with the given name (if one exists).
    ///-------------------------------------------------------------------------
    void removeVariable(const std::string &variableName);

  private:
    std::vector<ClientVariable> variables;
};

///-----------------------------------------------------------------------------
/// \returns True if given string can be parsed as a float and false otherwise.
///-----------------------------------------------------------------------------
bool isFloat(const std::string &value);

///-----------------------------------------------------------------------------
/// \returns True if given string can be parsed as an int and false otherwise.
///-----------------------------------------------------------------------------
bool isInt(const std::string &value);
}
