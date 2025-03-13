#ifndef EXPRESSIONBUILDER_H
#define EXPRESSIONBUILDER_H

#include <stack>
#include <iostream>
#include <cmath>
#include <variant>
#include "expression.h"
#include "Node.h"

using CValue = std::variant<std::monostate, double, std::string>;

/** @brief subclass of an external class to handle expressions
 */
class ExpressionBuilder : public CExprBuilder {
public:
    ExpressionBuilder();

    ~ExpressionBuilder();

    void opAdd() override;

    void opSub() override;

    void opMul() override;

    void opDiv() override;

    void opPow() override;

    void opNeg() override;

    void opEq() override;

    void opNe() override;

    void opLt() override;

    void opLe() override;

    void opGt() override;

    void opGe() override;

    void valNumber(double val) override;

    void valString(std::string val) override;

    void valReference(std::string val) override;

    void valRange(std::string val) override;

    void funcCall(std::string fnName, int paramCount) override;
    /**
     * @brief Getter for AST.

     * @return std::shared_ptr<Node> ast.
     */
    std::shared_ptr<Node> getAST();

private:
    std::stack<std::shared_ptr<Node>> stack;
    std::shared_ptr<Node> ast;
};

#endif // EXPRESSIONBUILDER_H
