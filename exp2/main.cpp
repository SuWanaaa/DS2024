#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <stdexcept>
#include <cctype>
#include "Stack.cpp"
#include <ctime>

#define N_OPTR 9

typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;

const char pri[N_OPTR][N_OPTR] =
{
    /* + */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /* - */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /* * */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /* / */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /* ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>',
    /* ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>',
    /* ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ',
    /* ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '='
};

// ��ȡ����������ȼ���ϵ
char orderBetween(char op1, char op2) {
    int i = (op1 == '+' ? ADD : op1 == '-' ? SUB : op1 == '*' ? MUL :
             op1 == '/' ? DIV : op1 == '^' ? POW : op1 == '!' ? FAC :
             op1 == '(' ? L_P : op1 == ')' ? R_P : EOE);
    int j = (op2 == '+' ? ADD : op2 == '-' ? SUB : op2 == '*' ? MUL :
             op2 == '/' ? DIV : op2 == '^' ? POW : op2 == '!' ? FAC :
             op2 == '(' ? L_P : op2 == ')' ? R_P : EOE);
    return pri[i][j];
};

// ��ȡ����
void readNumber(const char*& S, Stack<float>& opnd) {
    float number = 0;
    while (isdigit(*S)) {
        number = number * 10 + (*S - '0');
        S++;
    }
    opnd.push(number);
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
    default: throw std::runtime_error("δʶ��Ĳ�����");
    }
}

// ������ʽ
float evaluate(const char* S, std::string& RPN) {
    Stack<float> opnd;  // ������ջ
    Stack<char> optr;   // ������ջ
    optr.push('\0');          // ��ʼ����

    bool expectOperand = true; // ����������
    int parenthesisCount = 0; // ���ż�����

    while (*S != '\0') {
        if (isspace(*S)) {
            S++; // �����հ��ַ�
            continue;
        }

        if (isdigit(*S)) {
            readNumber(S, opnd);  // ��ȡ����
            RPN += std::to_string(opnd.top()) + " "; // ��ӵ�RPN
            expectOperand = false;  // ��ȡ��������������״̬
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
                
                // ȷ��ջ��Ϊ���ٵ���������
                if (opnd.size() < 2) throw std::runtime_error("���������㣬�޷���������");
                float pOpnd2 = opnd.top(); opnd.pop();
                float pOpnd1 = opnd.top(); opnd.pop();
                opnd.push(calcu(pOpnd1, op, pOpnd2));  // ��ͨ��Ԫ����
            }
            optr.pop(); // ����������
            parenthesisCount--;
            S++;
            expectOperand = false; // �����ź���Լ�������
        } 
        else {
            if (expectOperand) throw std::runtime_error("�������ȱ�ٲ�����");
            char op = *S;
            if (orderBetween(optr.top(), op) == '<') {
                optr.push(op);
                S++;
                expectOperand = true; // ������������������
            } 
            else {
                while (orderBetween(optr.top(), op) == '>') {
                    char topOp = optr.top();
                    optr.pop();
                    RPN += topOp; // ��ӵ�RPN
                    RPN += " ";
                    
                    // ȷ��ջ��Ϊ���ٵ���������
                    if (opnd.size() < 2) throw std::runtime_error("���������㣬�޷���������");
                    float pOpnd2 = opnd.top(); opnd.pop();
                    float pOpnd1 = opnd.top(); opnd.pop();
                    opnd.push(calcu(pOpnd1, topOp, pOpnd2));  // ��ͨ��Ԫ����
                }
                optr.push(op);
                S++;
                expectOperand = true; // ������������������
            }
        }
    }

    // ����ʣ�������
    while (!optr.empty() && optr.top() != '\0') {
        char op = optr.top();
        optr.pop();
        if (op == '(') throw std::runtime_error("δƥ�䵽������");
        RPN += op; // ��ӵ�RPN
        RPN += " ";

        // ȷ��ջ��Ϊ���ٵ���������
        if (opnd.size() < 2) throw std::runtime_error("�������ȱ�ٲ�����");
        float pOpnd2 = opnd.top(); opnd.pop();
        float pOpnd1 = opnd.top(); opnd.pop();
        opnd.push(calcu(pOpnd1, op, pOpnd2));  // ��ͨ��Ԫ����
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

