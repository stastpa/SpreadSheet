#include "CPos.h"

CPos::CPos(std::string_view str) {
    try {
        if (!processPos(str)) {
            throw std::invalid_argument("Not a valid position.");
        }
    }
    catch (const std::runtime_error &e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }
}

bool CPos::isValid(std::string_view str, size_t &firstNumberPosition) const {
    if (str.size() < 2) {
        return false;
    }

    bool hasAlphabet = false;
    bool hasNumber = false;

    for (size_t i = 0; i < str.size(); ++i) {
        char c = str[i];
        if (std::isalpha(c)) {
            if (!hasAlphabet && !hasNumber) {
                hasAlphabet = true;
            } else if (hasNumber) {
                return false;
            }
        } else if (std::isdigit(c)) {
            if (!hasAlphabet) {
                return false;
            } else {
                hasNumber = true;
                if (!firstNumberPosition) {
                    firstNumberPosition = i;
                }
            }
        } else {
            return false;
        }
    }

    return hasAlphabet && hasNumber;
}

bool CPos::processPos(std::string_view str) {
    size_t separatorPos = 0;
    if (!isValid(str, separatorPos)) {
        return false;
    }
    for (auto c: str) {
        if (c != '$') {
            rawData += std::tolower(c);
        }
    }
    rawData = std::string(str);

    std::string_view colStr = str.substr(0, separatorPos);
    std::string_view rowStr = str.substr(separatorPos);

    column = calculateCol(colStr);
    row = std::stoul(std::string(rowStr));

    return true;
}

size_t CPos::calculateCol(std::string_view str) {
    size_t value = 0;
    for (const char &x: str) {
        char c = std::tolower(x);
        value = value * 26 + (c - 'a' + 1);
    }
    return value;
}

size_t CPos::getRow() const {
    return this->row;
}

size_t CPos::getColumn() const {
    return this->column;
}
