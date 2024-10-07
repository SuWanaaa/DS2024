#include <cmath>
#include <ctime>
#include <iostream> 
#include "Vector.cpp"

using namespace std;

class Complex {
public:
    double real, imag;

    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    double module() const 
	{
        return sqrt(real * real + imag * imag);
    }

    friend ostream& operator<<(ostream& os, const Complex& c) 
	{
        os << "(" << c.real << ", " << c.imag << ")";
        return os;
    }
    
    bool operator==(const Complex& other) const 
	{
        return (real == other.real) && (imag == other.imag);
    }

    bool operator!=(const Complex& other) const 
	{
        return !(*this == other);
    }
    
    bool operator<(const Complex& other) const 
	{
        if (module() != other.module()) {
            return module() < other.module();
        }
        return real < other.real;
    }
    
    bool operator<=(const Complex& other) const 
	{
        return (*this < other) || (*this == other);
    }
    
    bool operator>(const Complex& other) const 
	{
        if (module() != other.module()) {
            return module() > other.module();
        }
        return real > other.real;
    }
};

Complex generate() {
    return Complex(rand() % 100, rand() % 100);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Vector<Complex> A;

    for (int i = 0; i < 5000; i++) {
        A.insert(A.size(), generate());
    }

    cout << "Original Vector: \n";
    A.traverse([](Complex& c) { cout << c << " "; });
    cout << endl;

    // 置乱
    A.unsort();
    cout << "\nAfter Unsorted Vector: ";
    A.traverse([](Complex& c) { cout << c << " "; });
    cout << endl;

	//查找 
    Complex target;
	target.real=1;
	target.imag=2; 
    Rank index = A.find(target);
    if (index >= 0) 
	{
        cout << "\nFound " << target << " at index " << index << std::endl;
    }
	else 
	{
        cout << "\n" << target << " not found." << std::endl;
    }
    
    //插入
    int m;
    Complex n;
	m=0; 
	n.real=1;
	n.imag=1; 
	A.insert(m, n);
    std::cout << "\nAfter Insertion: \n";
    A.traverse([](Complex& c) { cout << c << " "; });
    cout << endl; 
    
    //删除
    int p;
	p=0;
	A.remove(p);
    cout << "\nAfter Deletion: \n";
    A.traverse([](Complex& c) { cout << c << " "; });
    cout << endl; 
    
    //唯一化
    A.uniquify();
    cout << "\nAfter Uniquify: \n";
    A.traverse([](Complex& c) { cout << c << " "; });
    cout << endl; 
    cout<<"\n";
    
    //乱序情况下
    cout<<"in unsort insituation:\n";
    Vector<Complex> B(A);

    clock_t start_merge = clock();
    A.mergeSort(0, A.size());
    clock_t end_merge = clock();

    double merge_time = static_cast<double>(end_merge - start_merge) / CLOCKS_PER_SEC;
    cout << "Time taken for Merge Sort: " << merge_time << " seconds" << endl;

    clock_t start_bubble = clock();
    B.bubbleSort(0, B.size());
    clock_t end_bubble = clock();

    double bubble_time = static_cast<double>(end_bubble - start_bubble) / CLOCKS_PER_SEC;
    cout << "Time taken for Bubble Sort: " << bubble_time << " seconds" << endl;

    if (merge_time < bubble_time) 
	{
        cout << "Merge Sort is faster." << endl;
    } 
	else if (merge_time > bubble_time) {
        cout << "Bubble Sort is faster." << endl;
    } 
	else 
	{
        cout << "Both  took the same amount of time." << endl;
    }
    cout<<"\n";
    
    //顺序情况下
    cout<<"in sort insituation:\n";
	Vector<Complex> C(A);
	Vector<Complex> D(A);
	
    clock_t start_merge_1 = clock();
    C.mergeSort(0, C.size());
    clock_t end_merge_1 = clock();

    double merge_time_1 = static_cast<double>(end_merge_1 - start_merge_1) / CLOCKS_PER_SEC;
    cout << "Time taken for Merge Sort: " << merge_time_1 << " seconds" << endl;

    clock_t start_bubble_1 = clock();
    D.bubbleSort(0, D.size());
    clock_t end_bubble_1 = clock();

    double bubble_time_1 = static_cast<double>(end_bubble_1 - start_bubble_1) / CLOCKS_PER_SEC;
    cout << "Time taken for Bubble Sort: " << bubble_time_1 << " seconds" << endl;

    if (merge_time_1 < bubble_time_1) 
	{
        cout << "Merge Sort is faster." << endl;
    } 
	else if (merge_time_1 > bubble_time_1) {
        cout << "Bubble Sort is faster." << endl;
    } 
	else 
	{
        cout << "Both  took the same amount of time." << endl;
    }
    cout<<"\n";
    
    //逆序情况下
    cout<<"in reverse insituation:\n";
    A.reverse();
    Vector<Complex> E(A);
    clock_t start_merge_2 = clock();
    A.mergeSort(0, A.size());
    clock_t end_merge_2 = clock();

    double merge_time_2 = static_cast<double>(end_merge_2 - start_merge_2) / CLOCKS_PER_SEC;
    cout << "Time taken for Merge Sort: " << merge_time_2 << " seconds" << endl;

    clock_t start_bubble_2 = clock();
    E.bubbleSort(0, E.size());
    clock_t end_bubble_2 = clock();

    double bubble_time_2 = static_cast<double>(end_bubble_2 - start_bubble_2) / CLOCKS_PER_SEC;
    cout << "Time taken for Bubble Sort: " << bubble_time_2 << " seconds" << endl;

    if (merge_time_2 < bubble_time_2) 
	{
        cout << "Merge Sort is faster." << endl;
    } 
	else if (merge_time_2 > bubble_time_2) {
        cout << "Bubble Sort is faster." << endl;
    } 
	else 
	{
        cout << "Both  took the same amount of time." << endl;
    }
    cout<<"\n";
    
    //顺序向量区间查找 
    double m1 = 10.0, m2 = 50.0; 
	Vector<Complex> found;

    for (int i = 0; i < A.size(); i++) {
        double mod = A[i].module(); 
        if (mod >= m1 && mod < m2) 
		{
            found.insert(found.size(), A[i]); 
        }
    }

    cout << "Complex numbers with modulus in range [" << m1 << ", " << m2 << "): ";
    found.traverse([](Complex& c) { cout << "(" << c.real << "," << c.imag << ") "; });
    cout << endl;
    
    return 0;    
}

