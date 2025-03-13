
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

/** @brief The CSpreadsheet class represents a spreadsheet.
 */
class CSpreadsheet {
public:
    static unsigned capabilities() {
        return SPREADSHEET_CYCLIC_DEPS;
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

CSpreadsheet::CSpreadsheet() {
}

CSpreadsheet::CSpreadsheet(const CSpreadsheet &other) : sheet(other.sheet) {}

CSpreadsheet &CSpreadsheet::operator=(const CSpreadsheet &other) {
    if (this != &other) {
        sheet = other.sheet;
    }
    return *this;
}

bool CSpreadsheet::setCell(CPos pos,
                           std::string contents) {

    if (!contents.empty() && contents[0] == '=') {
        if (!(CValue(contents).index() == 0)) {
            ExpressionBuilder builder;
            try {
                parseExpression(contents, builder);
            }
            catch (const std::exception &e) {
                return false;
            }
            sheet[{pos.getRow(), pos.getColumn()}] = std::make_pair(contents, builder.getAST());
            return true;
        } else {
            return false;
        }
    } else {
        std::istringstream iss(contents);
        double number;
        if (iss >> number) {
            sheet[{pos.getRow(), pos.getColumn()}] = std::make_pair(number, nullptr);
            return true;
        } else {
            sheet[{pos.getRow(), pos.getColumn()}] = std::make_pair(contents, nullptr);
            return true;
        }
    }
}

CValue CSpreadsheet::getValue(CPos pos) {
    auto it = sheet.find({pos.getRow(), pos.getColumn()});
    if (it != sheet.end()) {
        switch (it->second.first.index()) {
            case 1:
                return CValue(std::get<double>(it->second.first));
            case 2: {
                const std::string &value = std::get<std::string>(it->second.first);
                if (!value.empty() && value[0] == '=' && it->second.second) {
                    std::set<std::string> positions;
                    return it->second.second->evaluate(*this, positions);
                } else if (value.empty()) {
                    return CValue();
                } else {
                    return it->second.first;
                }
            }
            default:
                return CValue();
        }
    } else {
        return CValue();
    }
}

CValue CSpreadsheet::getValueRec(CPos pos, std::set<std::string> &positions) {
    auto it = sheet.find({pos.getRow(), pos.getColumn()});
    if (it != sheet.end()) {
        switch (it->second.first.index()) {
            case 1:
                return CValue(std::get<double>(it->second.first));
            case 2: {
                const std::string &value = std::get<std::string>(it->second.first);
                if (!value.empty() && value[0] == '=' && it->second.second) {
                    return it->second.second->evaluate(*this, positions);
                } else if (value.empty()) {
                    return CValue();
                } else {
                    return it->second.first;
                }
            }
            default:
                return CValue();
        }
    } else {
        return CValue();
    }
}

bool CSpreadsheet::save(std::ostream &os) const {
    for (const auto &pos: sheet) {
        switch (pos.second.first.index()) {
            case 1:
                os << pos.first.first << ' ' << pos.first.second << ' ';
                os << pos.second.first.index() << ' ';
                os << 1 << ' ';
                os << std::get<double>(pos.second.first);
                break;
            case 2:
                os << pos.first.first << ' ' << pos.first.second << ' ';
                os << pos.second.first.index() << ' ';
                os << std::get<std::string>(pos.second.first).length() << ' ';
                os << std::get<std::string>(pos.second.first);
                break;
            default:
                break;
        }
        os << '\n';
    }
    return os.good();
}

bool CSpreadsheet::load(std::istream &is) {
    std::map<std::pair<size_t, size_t>, std::pair<CValue, std::shared_ptr<Node>>> tmp;
    while (!is.eof()) {
        size_t row, col;
        if (!(is >> row >> col)) {
            return false;
        }
        size_t index;
        if (!(is >> index)) {
            return false;
        }
        size_t len;
        if (!(is >> len)) {
            return false;
        }
        std::string res;
        ExpressionBuilder builder;
        switch (index) {
            case 1:
                double number;
                if (!(is >> number)) {
                    return false;
                }
                tmp[{row, col}] = std::make_pair(CValue(number), nullptr);
                break;
            case 2:
                res.reserve(len + 2);
                is.ignore(1);
                is.get(res.data(), len + 1, EOF);
                try {
                    parseExpression(res.data(), builder);
                }
                catch (const std::exception &e) {
                    return false;
                }
                tmp[{row, col}] = std::make_pair(CValue(res.data()), builder.getAST());
                break;
            default:
                return false;
        }
        is >> std::ws;
    }
    sheet.clear();
    sheet = tmp;
    return !is.fail();
}

void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {
    std::map<std::pair<size_t, size_t>, std::pair<CValue, std::shared_ptr<Node>>> tmp;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            ExpressionBuilder builder;
            auto it = sheet.find({src.getRow() + i, src.getColumn() + j});
            if (it != sheet.end()) {
                if (std::holds_alternative<std::string>(it->second.first)) {
                    if (!std::get<std::string>(it->second.first).empty() &&
                        std::get<std::string>(it->second.first)[0] == '=') {
                        std::string res = processExpression(std::get<std::string>(it->second.first),
                                                            dst.getColumn() - src.getColumn(),
                                                            dst.getRow() - src.getRow());
                        parseExpression(res, builder);
                        tmp[{dst.getRow() + i, dst.getColumn() + j}] = std::make_pair(res, builder.getAST());
                    } else {
                        tmp[{dst.getRow() + i, dst.getColumn() + j}] = std::make_pair(it->second.first, nullptr);
                    }
                } else if (std::holds_alternative<double>(it->second.first)) {
                    tmp[{dst.getRow() + i, dst.getColumn() + j}] = std::make_pair(it->second.first, nullptr);
                } else {
                    tmp[{dst.getRow() + i, dst.getColumn() + j}] = std::make_pair(CValue(), nullptr);
                }
            } else {
                tmp[{dst.getRow() + i, dst.getColumn() + j}] = std::make_pair(CValue(), nullptr);
            }
        }
    }
    for (const auto &pos: tmp) {
        sheet[pos.first] = pos.second;
    }
}

std::string CSpreadsheet::numberToString(size_t number) const {
    std::string result;

    while (number > 0) {
        int remainder = (number - 1) % 26;
        char digit = 'A' + remainder;
        result = digit + result;
        number = (number - 1) / 26;
    }

    return result;
}

size_t CSpreadsheet::calculateCol(const std::string &str) const {
    size_t value = 0;
    for (const char &x: str) {
        char c = std::tolower(x);
        value = value * 26 + (c - 'a' + 1);
    }
    return value;
}

std::string CSpreadsheet::processExpression(const std::string &str, int w, int h) const {
    std::string res = "=";
    res.reserve(str.length() + 1);
    try {
        for (size_t i = 1; i < str.length(); ++i) {
            if (str.at(i) == '\"') {
                do {
                    res += str.at(i);
                    ++i;
                } while (i < str.length() && str.at(i) != '\"');
                --i;
                continue;
            } else if (str.at(i) == '\'') {
                do {
                    res += str.at(i);
                    ++i;
                } while (i < str.length() && str.at(i) != '\'');
                --i;
                continue;
            } else if (isdigit(str.at(i))) {
                do {
                    res += str.at(i);
                    ++i;
                } while (i < str.length() && (isdigit(str.at(i)) || str.at(i) == 'e' || str.at(i) == 'E'));
                --i;
                continue;
            } else if (str.at(i) == '$' || isalpha(str.at(i))) {
                if (str.at(i) == '$') {
                    do {
                        res += str.at(i);
                        ++i;
                    } while (i < str.length() && isalpha(str.at(i)));
                } else {
                    std::string buffer;
                    do {
                        buffer += str.at(i);
                        ++i;
                    } while (i < str.length() && isalpha(str.at(i)));
                    res += numberToString(calculateCol(buffer) + w);
                }
                if (str.at(i) == '$') {
                    do {
                        res += str.at(i);
                        ++i;
                    } while (i < str.length() && isdigit(str.at(i)));
                } else {
                    std::string buffer;
                    do {
                        buffer += str.at(i);
                        ++i;
                    } while (i < str.length() && isdigit(str.at(i)));
                    res += std::to_string(std::stoi(buffer) + h);
                }
                --i;
                continue;
            } else {
                res += str.at(i);
            }
        }
    }
    catch (const std::exception &e) {
    }
    return res;
}

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
