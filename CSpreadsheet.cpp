#include "CSpreadsheet.h"
#include "ExpressionBuilder.h"

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