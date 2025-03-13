#include "Node.h"

CValue OperatorNode::evaluate(CSpreadsheet &sheet, std::set<std::string> &positions) {
    std::set<std::string> positionsRight = positions;
    const CValue &leftVal = left->evaluate(sheet, positions);
    const CValue &rightVal = right->evaluate(sheet, positionsRight);
    switch (op) {
        case Operator::ADD:

            if (std::holds_alternative<double>(leftVal) && std::holds_alternative<double>(rightVal)) {
                return std::get<double>(leftVal) + std::get<double>(rightVal);
            } else if (std::holds_alternative<std::string>(leftVal) || std::holds_alternative<std::string>(rightVal)) {
                std::string str_b = std::holds_alternative<std::string>(leftVal) ? std::get<std::string>(leftVal)
                                                                                 : std::to_string(
                                std::get<double>(leftVal));
                std::string str_a = std::holds_alternative<std::string>(rightVal) ? std::get<std::string>(rightVal)
                                                                                  : std::to_string(
                                std::get<double>(rightVal));
                std::string result = str_b + str_a;
                return result;
            } else {
                return CValue();
            }
            break;
        case Operator::SUBTRACT:
            if (std::holds_alternative<double>(leftVal) && std::holds_alternative<double>(rightVal)) {
                return std::get<double>(leftVal) - std::get<double>(rightVal);
            } else {
                return CValue();
            }
            break;
        case Operator::MULTIPLY:
            if (std::holds_alternative<double>(leftVal) && std::holds_alternative<double>(rightVal)) {
                return std::get<double>(leftVal) * std::get<double>(rightVal);
            } else {
                return CValue();
            }
            break;
        case Operator::DIVIDE:
            if (std::holds_alternative<double>(leftVal) && std::holds_alternative<double>(rightVal)) {
                if (std::get<double>(rightVal) == 0) {
                    return CValue();
                }
                return std::get<double>(leftVal) / std::get<double>(rightVal);
            }
            break;
        case Operator::POWER:
            if (std::holds_alternative<double>(leftVal) && std::holds_alternative<double>(rightVal)) {
                return std::pow(std::get<double>(leftVal), std::get<double>(rightVal));
            } else {
                return CValue();
            }
            break;
        case Operator::NEGATE:
            if (std::holds_alternative<double>(leftVal)) {
                return -std::get<double>(leftVal);
            } else {
                return CValue();
            }
            break;
        case Operator::EQUAL:
            return (leftVal == rightVal) ? 1.0 : 0.0;
            break;
        case Operator::NOT_EQUAL:
            return (leftVal != rightVal) ? 1.0 : 0.0;
            break;
        case Operator::LESS_THAN:
            return (leftVal < rightVal) ? 1.0 : 0.0;
            break;
        case Operator::LESS_THAN_OR_EQUAL:
            return (leftVal <= rightVal) ? 1.0 : 0.0;
            break;
        case Operator::GREATER_THAN:
            return (leftVal > rightVal) ? 1.0 : 0.0;
            break;
        case Operator::GREATER_THAN_OR_EQUAL:
            return (leftVal >= rightVal) ? 1.0 : 0.0;
            break;
        default:
            return CValue();
            break;
    }
    return CValue();
}

CValue ValueNode::evaluate(CSpreadsheet &sheet, std::set<std::string> &positions) {
    (void) sheet;
    return value;
}

CValue RefNode::evaluate(CSpreadsheet &sheet, std::set<std::string> &positions) {
    if (positions.contains(value)) {
        return CValue();
    } else {
        positions.insert(value);
        return sheet.getValueRec(CPos(value), positions);
    }
}