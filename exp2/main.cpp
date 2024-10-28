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
    default: throw std::runtime_error(std::string("δ֪������: ") + op);
    }
}

// ��ȡ����������ȼ���ϵ
char orderBetween(char op1, char op2) {
    int i = optrIndex(op1);
    int j = optrIndex(op2);
    return pri[i][j];
};

// ��ȡ���֣�����������
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

// ��ȡ��������
void readFunction(const char*& S, std::string& funcName) {
    funcName.clear();
    while (isalpha(*S)) {
        funcName += *S;
        S++;
    }
}

// ִ������
float calcu(float a, char op, float b = 0) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) throw std::runtime_error("��������Ϊ0");
        return a / b;
    case '^': return pow(a, b);
    case 's': return sin(a);  // sin
    case 'c': return cos(a);  // cos
    case 't': return tan(a);  // tan
    case 'l':
        if (a <= 0) throw std::runtime_error("�����������������0");
        return log10(a);       // log
    default: throw std::runtime_error("δʶ��Ĳ�����");
    }
}

// ������ʽ
float evaluate(const char* S, std::string& RPN) {
    Stack<float> opnd;  // ������ջ
    Stack<char> optr;   // ������ջ
    optr.push('\0');    // ��ʼ����

    bool expectOperand = true; // ����������
    int parenthesisCount = 0;  // ���ż�����

    while (*S != '\0') {
        if (isspace(*S)) {
            S++; // �����հ��ַ�
            continue;
        }

        if (isdigit(*S) || (*S == '.' && isdigit(*(S + 1)))) {
            readNumber(S, opnd);  // ��ȡ����
            RPN += std::to_string(opnd.top()) + " "; // ��ӵ�RPN
            expectOperand = false;  // ��ȡ��������������״̬
        }
        else if (isalpha(*S)) {
            // ��ȡ��������
            std::string funcName;
            readFunction(S, funcName);
            char funcOp;
            if (funcName == "sin") funcOp = 's';
            else if (funcName == "cos") funcOp = 'c';
            else if (funcName == "log") funcOp = 'l';
            else if (funcName == "tan") funcOp = 't';
            else throw std::runtime_error("δ֪����: " + funcName);

            optr.push(funcOp);
            expectOperand = true; // ���������������Ż������
        }
        else if (*S == '(') {
            optr.push(*S);
            parenthesisCount++;
            S++;
            expectOperand = true; // �������ź�����������
        }
        else if (*S == ')') {
            if (expectOperand) throw std::runtime_error("')'ǰӦ�в�����"); // ������ǰӦ�в�����
            while (optr.top() != '(') {
                if (optr.empty()) throw std::runtime_error("δƥ�䵽������"); // ƥ�䲻������
                char op = optr.top();
                optr.pop();
                RPN += op; // ��ӵ�RPN
                RPN += " ";

                if (op == 's' || op == 'c' || op == 'l' || op == 't') {
                    // һԪ��������
                    if (opnd.empty()) throw std::runtime_error("���������㣬�޷���������");
                    float pOpnd = opnd.top(); opnd.pop();
                    opnd.push(calcu(pOpnd, op));  // Ӧ�ú���
                }
                else {
                    // ��Ԫ����
                    if (opnd.size() < 2) throw std::runtime_error("���������㣬�޷���������");
                    float pOpnd2 = opnd.top(); opnd.pop();
                    float pOpnd1 = opnd.top(); opnd.pop();
                    opnd.push(calcu(pOpnd1, op, pOpnd2));  // ��ͨ��Ԫ����
                }
            }
            optr.pop(); // ����������
            parenthesisCount--;
            // ����Ƿ��к�����������֮ǰ
            if (!optr.empty() && (optr.top() == 's' || optr.top() == 'c' || optr.top() == 'l' || optr.top() == 't')) {
                char op = optr.top();
                optr.pop();
                RPN += op; // ��ӵ�RPN
                RPN += " ";

                if (opnd.empty()) throw std::runtime_error("���������㣬�޷���������");
                float pOpnd = opnd.top(); opnd.pop();
                opnd.push(calcu(pOpnd, op));  // Ӧ�ú���
            }
            S++;
            expectOperand = false; // �����ź���Լ�������
        }
        else {
            if (expectOperand) throw std::runtime_error("�������ȱ�ٲ�����");
            char op = *S;
            while (orderBetween(optr.top(), op) == '>') {
                char topOp = optr.top();
                optr.pop();
                RPN += topOp; // ��ӵ�RPN
                RPN += " ";

                if (topOp == 's' || topOp == 'c' || topOp == 'l' || topOp == 't') {
                    // һԪ��������
                    if (opnd.empty()) throw std::runtime_error("���������㣬�޷���������");
                    float pOpnd = opnd.top(); opnd.pop();
                    opnd.push(calcu(pOpnd, topOp));  // Ӧ�ú���
                }
                else {
                    // ��Ԫ����
                    if (opnd.size() < 2) throw std::runtime_error("���������㣬�޷���������");
                    float pOpnd2 = opnd.top(); opnd.pop();
                    float pOpnd1 = opnd.top(); opnd.pop();
                    opnd.push(calcu(pOpnd1, topOp, pOpnd2));  // ��ͨ��Ԫ����
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
            expectOperand = true; // ������������������
        }
    }

    // ����ʣ�������
    while (!optr.empty() && optr.top() != '\0') {
        char op = optr.top();
        optr.pop();
        if (op == '(') throw std::runtime_error("δƥ�䵽������");
        RPN += op; // ��ӵ�RPN
        RPN += " ";

        if (op == 's' || op == 'c' || op == 'l' || op == 't') {
            // һԪ��������
            if (opnd.empty()) throw std::runtime_error("���������㣬�޷���������");
            float pOpnd = opnd.top(); opnd.pop();
            opnd.push(calcu(pOpnd, op));  // Ӧ�ú���
        }
        else {
            // ��Ԫ����
            if (opnd.size() < 2) throw std::runtime_error("�������ȱ�ٲ�����");
            float pOpnd2 = opnd.top(); opnd.pop();
            float pOpnd1 = opnd.top(); opnd.pop();
            opnd.push(calcu(pOpnd1, op, pOpnd2));  // ��ͨ��Ԫ����
        }
    }

    // ��������ջ�Ƿ�ֻʣ��һ�����
    if (opnd.size() != 1) throw std::runtime_error("���������㣬�޷���������");

    if (parenthesisCount > 0) throw std::runtime_error("δƥ���������");
    if (expectOperand) throw std::runtime_error("���ʽĩβȱ�ٲ�����");

    return opnd.top();  // �������ս��
}

// ������ε�������
int largestArea(const std::vector<int>& heights) {
    Stack<int> indexs; // ջ�洢���ӵ�����
    int maxArea = 0;    // ������
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

// ���������������
std::vector<int> generateHeights(int length, int maxHeight) {
    std::vector<int> heights;
    for (int i = 0; i < length; ++i) {
        heights.push_back(rand() % (maxHeight + 1));
    }
    return heights;
}

int main() {
    srand(static_cast<unsigned>(time(0))); // �������������

    std::cout<<"---------���ʽ����---------"<<"\n";
    std::cout << "\n";
    const char* expressions[] = {
        "(1+1*5/4",     // ʾ��1: ȱ�������� 
        "3+5*",         // ʾ��2: �������ӦΪ������
        "1+1+2>3",      // ʾ��3: ��������Ч
        "3+)",          // ʾ��4: ������ǰ�޲����� 
        "1+5*9/(2*2)",  // ʾ��5: ��Ч���ʽ
        "sin(1)",       // ʾ��6: sin����
        "cos(0)",       // ʾ��7: cos����
        "log(10)",      // ʾ��8: log����
        "sin(1+1)",     // ʾ��9: ����Ƕ�ױ��ʽ
        "1+sin(0)+log(10)+cos(0)", // ʾ��10: ��ϱ��ʽ
        nullptr
    };

    for (const char** expr = expressions; *expr != nullptr; ++expr) 
	{
        std::string RPN;  // ʹ��std::string���洢�沨�����ʽ
        try {
            float result = evaluate(*expr, RPN);  // ����evaluate����
            std::cout << "���ʽ: " << *expr << std::endl;
            std::cout << "�沨�����ʽ: " << RPN << std::endl;
            std::cout << "������: " << result << std::endl << std::endl;
        } catch (const std::exception& e) {
            std::cout << "���ʽ: " << *expr << std::endl;
            std::cout << "ʽ����Ч: " << e.what() << std::endl << std::endl;  // ������������ʾ
        }
    }
	std::cout<<"---------��������������---------"<<"\n";
	std::cout << "\n";
    srand(static_cast<unsigned>(time(0))); // �������������

    // ʾ�� 1
    std::vector<int> heights1 = {2, 1, 5, 6, 2, 3};
    std::cout << "ʾ�� 1 ���������: " << largestArea(heights1) << std::endl;

    // ʾ�� 2
    std::vector<int> heights2 = {2, 4};
    std::cout << "ʾ�� 2 ���������: " << largestArea(heights2) << std::endl;

    // ������� 10 �����ݽ��в���
    std::cout << "\n����������ݣ�" << std::endl;
    for (int i = 1; i <= 10; ++i) 
	{
        int length = 1 + rand() % 105;         // ������� [1, 105]
        int maxHeight = 104;                  // ���߶�����Ϊ 104
        std::vector<int> heights = generateHeights(length, maxHeight);
        
        std::cout << "�������� " << i << " :";
        for (int h : heights) std::cout << h << " ";
        std::cout << "\n";
        std::cout << "=> ���������: " << largestArea(heights) << std::endl;
        std::cout << "\n";
	}
    return 0;
}

