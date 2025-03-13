#include "ExpressionBuilder.h"

ExpressionBuilder::ExpressionBuilder() {}
ExpressionBuilder::~ExpressionBuilder() = default;

void ExpressionBuilder::opAdd()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::ADD);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opSub()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::SUBTRACT);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opMul()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::MULTIPLY);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opDiv()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::DIVIDE);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opPow()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::POWER);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opNeg()
{
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::NEGATE);
    node->setLeft(left);
    node->setRight(std::make_shared<ValueNode>(CValue()));
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opEq()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::EQUAL);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opNe()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::NOT_EQUAL);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opLt()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::LESS_THAN);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opLe()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::LESS_THAN_OR_EQUAL);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opGt()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::GREATER_THAN);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::opGe()
{
    auto right = stack.top();
    stack.pop();
    auto left = stack.top();
    stack.pop();
    auto node = std::make_shared<OperatorNode>(Operator::GREATER_THAN_OR_EQUAL);
    node->setLeft(left);
    node->setRight(right);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::valNumber(double val)
{
    auto node = std::make_shared<ValueNode>(CValue(val));
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::valString(std::string val)
{
    auto node = std::make_shared<ValueNode>(CValue(val));
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::valReference(std::string val)
{
    std::string pos;
    for (auto c : val)
    {
        if (c != '$')
        {
            pos += std::tolower(c);
        }
    }
    auto node = std::make_shared<RefNode>(pos);
    stack.push(node);
    ast = node;
}

void ExpressionBuilder::valRange(std::string val)
{
    stack.push(std::make_shared<ValueNode>(CValue(val)));
}

void ExpressionBuilder::funcCall(std::string fnName, int paramCount)
{
    std::string name = fnName;
    int size = stack.size();
    if (size < paramCount)
    {
        stack.push(std::make_shared<ValueNode>(CValue()));
    }
}

std::shared_ptr<Node> ExpressionBuilder::getAST()
{
    return ast;
}