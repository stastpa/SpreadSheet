#ifndef NODE_H
#define NODE_H

#include <stack>
#include <iostream>
#include <cmath>
#include <variant>
#include "CSpreadsheet.h"

using CValue = std::variant<std::monostate, double, std::string>;

class CSpreadsheet;

/** @brief Node for AST
 */
class Node {
public:
    /**  @brief default destructor
      */
    virtual ~Node() = default;
    /**  @brief evalueates an expression
     * @param sheet [in] an sheet needed to evaluate node
     * @param positions [in] visited positions
     * @return Value depending on type of node
     */
    virtual CValue evaluate(CSpreadsheet &sheet, std::set<std::string> &positions) = 0;
};

/** @brief Enum class representing all possible operations
 */
enum class Operator {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER,
    NEGATE,
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL
};

/** @brief Node representing Operator
 */
class OperatorNode : public Node {
public:
    /**  @brief creates a new operator node
     * @param op [in] an operator
     */
    OperatorNode(Operator op) : op(op), left(nullptr), right(nullptr) {}
    /**  @brief default destructor
     */
    ~OperatorNode() override = default;
    /**  @brief evalueates an expression
     * @param sheet [in] an sheet needed to evaluate node
     * @param positions [in] visited positions
     * @return Value depending on type of operation.
     */
    CValue evaluate(CSpreadsheet &sheet, std::set<std::string> &positions) override;
    /**
     * @brief Setter for left node.
     */
    void setLeft(std::shared_ptr<Node> node) { left = node; }
    /**
     * @brief Setter for right node.
     */
    void setRight(std::shared_ptr<Node> node) { right = node; }
private:
    Operator op;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

/** @brief Node representing Value
 */
class ValueNode : public Node {
public:
    ValueNode(CValue value) : value(value) {}
    /**  @brief default destructor
     */
    ~ValueNode() override = default;
    /**  @brief evalueates an expression
     * @param sheet [in] an sheet needed to evaluate node
     * @param positions [in] visited positions
     * @return Value.
     */
    CValue evaluate(CSpreadsheet &sheet, std::set<std::string> &positions) override;
private:
    CValue value;
};

/** @brief Node representing Reference
 */
class RefNode : public Node {
public:
    RefNode(std::string value) : value(value) {}
    /**  @brief default destructor
     */
    ~RefNode() override = default;
    /**  @brief evalueates an expressi
     * @param sheet [in] an sheet needed to evaluate node
     * @param positions [in] visited positions
     * @return Value depending on referenced position
     */
    CValue evaluate(CSpreadsheet &sheet, std::set<std::string> &positions) override;
private:
    std::string value;
};

#endif // NODE_H