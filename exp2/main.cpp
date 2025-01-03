#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <stdexcept>
#include <cctype>
#include "Stack.cpp"
#include <ctime>


#define N_OPTR 13  // Updated number of operators

typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE, SIN, COS, LOG, TAN } Operator;

const char pri[N_OPTR][N_OPTR] =
{
    /*          +    -    *    /    ^    !    (    )    \0   sin  cos  log  tan */
    /* + */    '>', '>', '<', '<', '<', '<', '<', '>', '>', '<', '<', '<', '<',
    /* - */    '>', '>', '<', '<', '<', '<', '<', '>', '>', '<', '<', '<', '<',
    /* * */    '>', '>', '>', '>', '<', '<', '<', '>', '>', '<', '<', '<', '<',
    /* / */    '>', '>', '>', '>', '<', '<', '<', '>', '>', '<', '<', '<', '<',
    /* ^ */    '>', '>', '>', '>', '>', '<', '<', '>', '>', '<', '<', '<', '<',
    /* ! */    '>', '>', '>', '>', '>', '>', ' ', '>', '>', '<', '<', '<', '<',
    /* ( */    '<', '<', '<', '<', '<', '<', '<', '=', ' ', '<', '<', '<', '<',
    /* ) */    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* \0*/    '<', '<', '<', '<', '<', '<', '<', ' ', '=', '<', '<', '<', '<',
    /* sin*/   '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>',
    /* cos*/   '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>',
    /* log*/   '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>',
    /* tan*/   '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>'
};

// Map operator character to its index
int optrIndex(char op) {
    switch (op) {
    case '+': return ADD;
    case '-': return SUB;
    case '*': return MUL;
    case '/': return DIV;
    case '^': return POW;
    case '!': return FAC;
    case '(': return L_P;
    case ')': return R_P;
    case '\0': return EOE;
    case 's': return SIN;
    case 'c': return COS;
    case 'l': return LOG;
    case 't': return TAN;
    default: throw std::runtime_error(std::string("未知操作符: ") + op);
    }
}

// 获取运算符的优先级关系
char orderBetween(char op1, char op2) {
    int i = optrIndex(op1);
    int j = optrIndex(op2);
    return pri[i][j];
};

// 读取数字，包括浮点数
void readNumber(const char*& S, Stack<float>& opnd) {
    float number = 0;
    while (isdigit(*S)) {
        number = number * 10 + (*S - '0');
        S++;
    }
    if (*S == '.') {
        S++;
        float fraction = 0;
        float base = 0.1;
        while (isdigit(*S)) {
            fraction += (*S - '0') * base;
            base *= 0.1;
            S++;
        }
        number += fraction;
    }
    opnd.push(number);
}

// 读取函数名称
void readFunction(const char*& S, std::string& funcName) {
    funcName.clear();
    while (isalpha(*S)) {
        funcName += *S;
        S++;
    }
}

// 执行运算
float calcu(float a, char op, float b = 0) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) throw std::runtime_error("除数不能为0");
        return a / b;
    case '^': return pow(a, b);
    case 's': return sin(a);  // sin
    case 'c': return cos(a);  // cos
    case 't': return tan(a);  // tan
    case 'l':
        if (a <= 0) throw std::runtime_error("对数的真数必须大于0");
        return log10(a);       // log
    default: throw std::runtime_error("未识别的操作符");
    }
}

// 计算表达式
float evaluate(const char* S, std::string& RPN) {
    Stack<float> opnd;  // 操作数栈
    Stack<char> optr;   // 操作符栈
    optr.push('\0');    // 起始符号

    bool expectOperand = true; // 期望操作数
    int parenthesisCount = 0;  // 括号计数器

    while (*S != '\0') {
        if (isspace(*S)) {
            S++; // 跳过空白字符
            continue;
        }

        if (isdigit(*S) || (*S == '.' && isdigit(*(S + 1)))) {
            readNumber(S, opnd);  // 读取数字
            RPN += std::to_string(opnd.top()) + " "; // 添加到RPN
            expectOperand = false;  // 读取到操作数，更新状态
        }
        else if (isalpha(*S)) {
            // 读取函数名称
            std::string funcName;
            readFunction(S, funcName);
            char funcOp;
            if (funcName == "sin") funcOp = 's';
            else if (funcName == "cos") funcOp = 'c';
            else if (funcName == "log") funcOp = 'l';
            else if (funcName == "tan") funcOp = 't';
            else throw std::runtime_error("未知函数: " + funcName);

            optr.push(funcOp);
            expectOperand = true; // 函数后期望左括号或操作数
        }
        else if (*S == '(') {
            optr.push(*S);
            parenthesisCount++;
            S++;
            expectOperand = true; // 在左括号后期望操作数
        }
        else if (*S == ')') {
            if (expectOperand) throw std::runtime_error("')'前应有操作数"); // 右括号前应有操作数
            while (optr.top() != '(') {
                if (optr.empty()) throw std::runtime_error("未匹配到左括号"); // 匹配不到括号
                char op = optr.top();
                optr.pop();
                RPN += op; // 添加到RPN
                RPN += " ";

                if (op == 's' || op == 'c' || op == 'l' || op == 't') {
                    // 一元函数运算
                    if (opnd.empty()) throw std::runtime_error("操作数不足，无法进行运算");
                    float pOpnd = opnd.top(); opnd.pop();
                    opnd.push(calcu(pOpnd, op));  // 应用函数
                }
                else {
                    // 二元运算
                    if (opnd.size() < 2) throw std::runtime_error("操作数不足，无法进行运算");
                    float pOpnd2 = opnd.top(); opnd.pop();
                    float pOpnd1 = opnd.top(); opnd.pop();
                    opnd.push(calcu(pOpnd1, op, pOpnd2));  // 普通二元运算
                }
            }
            optr.pop(); // 弹出左括号
            parenthesisCount--;
            // 检查是否有函数在左括号之前
            if (!optr.empty() && (optr.top() == 's' || optr.top() == 'c' || optr.top() == 'l' || optr.top() == 't')) {
                char op = optr.top();
                optr.pop();
                RPN += op; // 添加到RPN
                RPN += " ";

                if (opnd.empty()) throw std::runtime_error("操作数不足，无法进行运算");
                float pOpnd = opnd.top(); opnd.pop();
                opnd.push(calcu(pOpnd, op));  // 应用函数
            }
            S++;
            expectOperand = false; // 右括号后可以继续操作
        }
        else {
            if (expectOperand) throw std::runtime_error("运算符后缺少操作数");
            char op = *S;
            while (orderBetween(optr.top(), op) == '>') {
                char topOp = optr.top();
                optr.pop();
                RPN += topOp; // 添加到RPN
                RPN += " ";

                if (topOp == 's' || topOp == 'c' || topOp == 'l' || topOp == 't') {
                    // 一元函数运算
                    if (opnd.empty()) throw std::runtime_error("操作数不足，无法进行运算");
                    float pOpnd = opnd.top(); opnd.pop();
                    opnd.push(calcu(pOpnd, topOp));  // 应用函数
                }
                else {
                    // 二元运算
                    if (opnd.size() < 2) throw std::runtime_error("操作数不足，无法进行运算");
                    float pOpnd2 = opnd.top(); opnd.pop();
                    float pOpnd1 = opnd.top(); opnd.pop();
                    opnd.push(calcu(pOpnd1, topOp, pOpnd2));  // 普通二元运算
                }
            }
            if (orderBetween(optr.top(), op) == '=') {
                optr.pop();
                S++;
            }
            else {
                optr.push(op);
                S++;
            }
            expectOperand = true; // 操作符后期望操作数
        }
    }

    // 处理剩余操作符
    while (!optr.empty() && optr.top() != '\0') {
        char op = optr.top();
        optr.pop();
        if (op == '(') throw std::runtime_error("未匹配到右括号");
        RPN += op; // 添加到RPN
        RPN += " ";

        if (op == 's' || op == 'c' || op == 'l' || op == 't') {
            // 一元函数运算
            if (opnd.empty()) throw std::runtime_error("操作数不足，无法进行运算");
            float pOpnd = opnd.top(); opnd.pop();
            opnd.push(calcu(pOpnd, op));  // 应用函数
        }
        else {
            // 二元运算
            if (opnd.size() < 2) throw std::runtime_error("运算符后缺少操作数");
            float pOpnd2 = opnd.top(); opnd.pop();
            float pOpnd1 = opnd.top(); opnd.pop();
            opnd.push(calcu(pOpnd1, op, pOpnd2));  // 普通二元运算
        }
    }

    // 检查操作数栈是否只剩下一个结果
    if (opnd.size() != 1) throw std::runtime_error("操作数不足，无法进行运算");

    if (parenthesisCount > 0) throw std::runtime_error("未匹配的右括号");
    if (expectOperand) throw std::runtime_error("表达式末尾缺少操作符");

    return opnd.top();  // 返回最终结果
}

// 计算矩形的最大面积
int largestArea(const std::vector<int>& heights) {
    Stack<int> indexs; // 栈存储柱子的索引
    int maxArea = 0;    // 最大面积
    int n = heights.size();

    for (int i = 0; i <= n; ++i) {
        int h = (i == n) ? 0 : heights[i];
        while (!indexs.empty() && heights[indexs.top()] > h) {
            int height = heights[indexs.top()];
            indexs.pop();
            int width = indexs.empty() ? i : (i - indexs.top() - 1);
            maxArea = std::max(maxArea, height * width);
        }
        indexs.push(i);
    }

    return maxArea;
}

// 生成随机测试数据
std::vector<int> generateHeights(int length, int maxHeight) {
    std::vector<int> heights;
    for (int i = 0; i < length; ++i) {
        heights.push_back(rand() % (maxHeight + 1));
    }
    return heights;
}

int main() {
    srand(static_cast<unsigned>(time(0))); // 设置随机数种子

    std::cout<<"---------表达式测试---------"<<"\n";
    std::cout << "\n";
    const char* expressions[] = {
        "(1+1*5/4",     // 示例1: 缺少右括号 
        "3+5*",         // 示例2: 运算符后应为操作数
        "1+1+2>3",      // 示例3: 操作符无效
        "3+)",          // 示例4: 右括号前无操作数 
        "1+5*9/(2*2)",  // 示例5: 有效表达式
        "sin(1)",       // 示例6: sin函数
        "cos(0)",       // 示例7: cos函数
        "log(10)",      // 示例8: log函数
        "sin(1+1)",     // 示例9: 函数嵌套表达式
        "1+sin(0)+log(10)+cos(0)", // 示例10: 组合表达式
        nullptr
    };

    for (const char** expr = expressions; *expr != nullptr; ++expr) 
	{
        std::string RPN;  // 使用std::string来存储逆波兰表达式
        try {
            float result = evaluate(*expr, RPN);  // 调用evaluate函数
            std::cout << "表达式: " << *expr << std::endl;
            std::cout << "逆波兰表达式: " << RPN << std::endl;
            std::cout << "计算结果: " << result << std::endl << std::endl;
        } catch (const std::exception& e) {
            std::cout << "表达式: " << *expr << std::endl;
            std::cout << "式子无效: " << e.what() << std::endl << std::endl;  // 捕获错误并输出提示
        }
    }
	std::cout<<"---------矩形最大面积测试---------"<<"\n";
	std::cout << "\n";
    srand(static_cast<unsigned>(time(0))); // 设置随机数种子

    // 示例 1
    std::vector<int> heights1 = {2, 1, 5, 6, 2, 3};
    std::cout << "示例 1 最大矩形面积: " << largestArea(heights1) << std::endl;

    // 示例 2
    std::vector<int> heights2 = {2, 4};
    std::cout << "示例 2 最大矩形面积: " << largestArea(heights2) << std::endl;

    // 随机生成 10 组数据进行测试
    std::cout << "\n随机测试数据：" << std::endl;
    for (int i = 1; i <= 10; ++i) 
	{
        int length = 1 + rand() % 105;         // 随机长度 [1, 105]
        int maxHeight = 104;                  // 最大高度限制为 104
        std::vector<int> heights = generateHeights(length, maxHeight);
        
        std::cout << "测试数据 " << i << " :";
        for (int h : heights) std::cout << h << " ";
        std::cout << "\n";
        std::cout << "=> 最大矩形面积: " << largestArea(heights) << std::endl;
        std::cout << "\n";
	}
    return 0;
}

