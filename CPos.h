#ifndef CPOS_H
#define CPOS_H

#include <string>
#include <cstring>
#include <iostream>

/** @brief The CPos class represents a position in a sheet.
 */
class CPos {
public:
    /**  @brief creates a new position
     * class also validates an input
     * @param str [in] a position
     */
    CPos(std::string_view str);


    /**
     * @brief Gets the row of the position.
     *
     * @return size_t The row of the position.
     */
    size_t getRow() const;

    /**
     * @brief Gets the column of the position.
     *
     * @return size_t The column of the position.
     */
    size_t getColumn() const;

private:
    std::string rawData;
    size_t row;
    size_t column;

    /**
     * @brief Checks if the provided string is a valid position string.
     *
     * @param str [in] The string to validate.
     * @param firstNumberPosition [out] The position of the first number in the string.
     * @return bool True if the string is valid, false otherwise.
     */
    bool isValid(std::string_view str, size_t &firstNumberPosition) const;

    /**
     * @brief Processes the position string to extract row and column information.
     *
     * @param str [in] The position string.
     * @return bool True if processing is successful, false otherwise.
     */
    bool processPos(std::string_view str);

    /**
     * @brief Calculates the column from the position string.
     *
     * @param s [in] The position string.
     * @return size_t The calculated column.
     */
    size_t calculateCol(std::string_view s);
};

#endif // CPOS_H