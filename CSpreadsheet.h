#ifndef CSPREADSHEET_H
#define CSPREADSHEET_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include "CPos.h"
#include "Node.h"

using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;

class Node;

/** @brief The CSpreadsheet class represents a spreadsheet.
 */
class CSpreadsheet {
public:
    static unsigned capabilities() {
        return 0;
    }
    /**  @brief creates a new spreadsheet
     * default constructor
     */
    CSpreadsheet();
    /**  @brief creates a new spreadsheet
     * copy-constructor
     * @param other [in] other spreadsheet
     */
    CSpreadsheet(const CSpreadsheet &other);
    /** @brief overloads operator =
     * @param other [in] other spreadsheet
     * @return spreadsheet
     */
    CSpreadsheet &operator=(const CSpreadsheet &other);

    /**
     * @brief loads a spreadsheet from stream
     *
     * @param is [in] stream to load spreadsheet from.
     * @return bool True if loading is successful, false otherwise.
     */
    bool load(std::istream &is);

    /**
     * @brief saves a spreadsheet into stream
     *
     * @param os [in] stream to save spreadsheet into.
     * @return bool True if saving is successful, false otherwise.
     */
    bool save(std::ostream &os) const;

    /**
     * @brief sets value of a cell in given position
     *
     * @param pos [in] position in the sheet.
     * @param contents [in] value to be saved in given position.
     * @return bool True if setting is successful, false otherwise.
     */
    bool setCell(CPos pos,
                 std::string contents);

    /**
     * @brief returns a value on given position
     *
     * @param pos [in] position in the sheet.
     * @return CValue, Value of a given position.
     */
    CValue getValue(CPos pos);

    /**
     * @brief returns a value on given position, needed for cyclic dependencies.
     *
     * @param pos [in] position in the sheet.
     * @param positions [in] already visited positions.
     * @return CValue, Value of a given position.
     */
    CValue getValueRec(CPos pos, std::set<std::string> &positions);

    /**
     * @brief copies a rectangle of values into a different place in sheet
     *
     * @param dst [in] position to copy rectangle into
     * @param src [in] position to be copied
     * @param w [in] weight of a rectangle (default 1)
     * @param h [in] height of a rectangle (default 1)
     */
    void copyRect(CPos dst,
                  CPos src,
                  int w = 1,
                  int h = 1);

private:
    std::map<std::pair<size_t, size_t>, std::pair<CValue, std::shared_ptr<Node>>> sheet;

    /**
     * @brief converts number into a column position
     *
     * @param number [in] number to be converted
     * @return string column value of a given number.
     */
    std::string numberToString(size_t number) const;

    /**
     * @brief converts string into a column number
     *
     * @param str [in] string to be converted
     * @return size_t column value of a given string.
     */
    size_t calculateCol(const std::string &str) const;


    /**
     * @brief converts string into a column number
     *
     * @param str [in] string to be converted
     * @return size_t column value of a given string.
     */
    std::string processExpression(const std::string &str, int w, int h) const;
};

#endif // CSPREADSHEET_H