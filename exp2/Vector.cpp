typedef int Rank;
#include <cstdlib> 
#include <algorithm> 
#define DEFAULT_CAPACITY 3

template <typename T> 
class Vector {
protected:
    Rank _size;
    int _capacity;
    T* _elem;

    void copyFrom(T const* A, Rank lo, Rank hi);
    void expand();
    void shrink();

    Rank max(Rank lo, Rank hi);
    void selectionSort(Rank lo, Rank hi);

    Rank partition(Rank lo, Rank hi);
    void quickSort(Rank lo, Rank hi);
    void heapSort(Rank lo, Rank hi);
    void heapify(Rank n, Rank i); // ������������heapSort

public:
	void reverse(); 
    bool bubble(Rank lo, Rank hi);
    void bubbleSort(Rank lo, Rank hi);
	void merge(Rank lo, Rank mi, Rank hi);
    void mergeSort(Rank lo, Rank hi);
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }

    Vector(T const* A, Rank n) { copyFrom(A, 0, n); }
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

    ~Vector() { delete[] _elem; }

    Rank size() const { return _size; }
    bool empty() const { return !_size; }
    int disordered() const;
    Rank find(T const& e) const { return find(e, 0, _size); }
    Rank find(T const& e, Rank lo, Rank hi) const;
    Rank search(T const& e) const { return (_size <= 0) ? -1 : search(e, 0, _size); }
    Rank search(T const& e, Rank lo, Rank hi) const;

    T& operator[](Rank r) const;
    Vector<T>& operator=(Vector<T> const&);
    T remove(Rank r);
    int remove(Rank lo, Rank hi);
    Rank insert(Rank r, T const& e);
    Rank insert(T const& e) { return insert(_size, e); }
    void sort(Rank lo, Rank hi);
    void sort() { sort(0, _size); }
    void unsort(Rank lo, Rank hi);
    void unsort() { unsort(0, _size); }
    int deduplicate();
    int uniquify();

    void traverse(void(*)(T&));
    template <typename VST> void traverse(VST&);
};

//�������� 
template <typename T>
void Vector<T>::reverse() {
    for (Rank i = 0; i < _size / 2; ++i) {
        std::swap(_elem[i], _elem[_size - 1 - i]);
    }
}

// ��������
template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi) {
        _elem[_size++] = A[lo++];
    }
}

// ����
template <typename T>
void Vector<T>::expand() {
    if (_size < _capacity) return;
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
    T* oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];
    delete[] oldElem;
}

// ����
template <typename T>
void Vector<T>::shrink() {
    if (_capacity < (DEFAULT_CAPACITY << 1)) return;
    if (_size << 2 > _capacity) return;
    T* oldElem = _elem;
    _elem = new T[_capacity >>= 1];
    for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];
    delete[] oldElem;
}

// ð�������е�һ��ð��
template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {
    bool sorted = true;
    while (++lo < hi) {
        if (_elem[lo - 1] > _elem[lo]) {
            sorted = false;
			std::swap(_elem[lo - 1], _elem[lo]);      
        }
    }
    return sorted;
}

// ð������
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
    while (!bubble(lo, hi--));
}

//�������Ԫ�� 
template <typename T>
Rank Vector<T>::max(Rank lo, Rank hi) 
{
    Rank maxIndex = lo;
    while (++lo < hi) {
        if (_elem[maxIndex] < _elem[lo]) {
            maxIndex = lo;
        }
    }
    return maxIndex;
}

// ѡ������
template <typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi) {
    for (Rank i = lo; i < hi - 1; ++i) {
        Rank minIndex = i;
        for (Rank j = i + 1; j < hi; ++j) {
            if (_elem[j] < _elem[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(_elem[i], _elem[minIndex]);
        }
    }
}

// �鲢����
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    T* A = _elem + lo;
    int lb = mi - lo;
    T* B = new T[lb];
    for (Rank i = 0; i < lb; B[i] = A[i++]);

    int lc = hi - mi;
    T* C = _elem + mi;

    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
        if ((k < lc) && (!(j < lb) || (C[k] <= B[j]))) A[i++] = C[k++];
    }
    delete[] B;
}

// �鲢����
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;
    Rank mi = (lo + hi) / 2;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}

// ��������Ļ��ֺ���
template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) {
    T pivot = _elem[lo]; // �Ե�һ��Ԫ��Ϊ��׼
    while (lo < hi) {
        while (lo < hi && _elem[hi] >= pivot) hi--; // ���������ҵ���һ��С�� pivot ��Ԫ��
        _elem[lo] = _elem[hi]; // ����
        while (lo < hi && _elem[lo] <= pivot) lo++; // ���������ҵ���һ������ pivot ��Ԫ��
        _elem[hi] = _elem[lo]; // ����
    }
    _elem[lo] = pivot; // ��׼Ԫ�ع�λ
    return lo; // ���ػ�׼Ԫ�ص�λ��
}

// ��������
template <typename T>
void Vector<T>::quickSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return;
    Rank pivotPos = partition(lo, hi - 1);
    quickSort(lo, pivotPos);
    quickSort(pivotPos + 1, hi);
}

// ���������������ѵ���
template <typename T>
void Vector<T>::heapify(Rank n, Rank i) {
    Rank largest = i;
    Rank left = 2 * i + 1;
    Rank right = 2 * i + 2;

    if (left < n && _elem[left] > _elem[largest]) largest = left;
    if (right < n && _elem[right] > _elem[largest]) largest = right;

    if (largest != i) {
        swap(_elem[i], _elem[largest]);
        heapify(n, largest);
    }
}

// ������
template <typename T>
void Vector<T>::heapSort(Rank lo, Rank hi) {
    Rank n = hi - lo;
    for (Rank i = n / 2 - 1; i >= 0; i--) {
        heapify(n, i);
    }
    for (Rank i = n - 1; i > 0; i--) {
        swap(_elem[lo], _elem[lo + i]);
        heapify(i, 0);
    }
}

// �������������
template <typename T>
int Vector<T>::disordered() const {
    int n = 0;
    for (int i = 1; i < _size; i++) {
        if (_elem[i - 1] > _elem[i]) n++;
    }
    return n;
}

// ����Ԫ��
template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
    while ((lo < hi--) && (e != _elem[hi]));
    return hi;
}

// ���ֲ���
template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
    while ((lo < hi--) && (_elem[hi] != e));
    return hi;
}

// �����±������
template <typename T>
T& Vector<T>::operator[](Rank r) const { return _elem[r]; }

// ���ظ�ֵ�����
template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
    if (_elem) delete[] _elem;
    copyFrom(V._elem, 0, V.size());
    return *this;
}

// ɾ��ָ��λ�õ�Ԫ��
template <typename T>
T Vector<T>::remove(Rank r) {
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}

// ɾ�������ڵ�Ԫ��
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
    if (lo == hi) return 0;
    while (hi < _size) _elem[lo++] = _elem[hi++];
    _size = lo;
    shrink();
    return hi - lo;
}

// ����Ԫ��
template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
    expand();
    for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1];
    _elem[r] = e;
    _size++;
    return r;
}

// ������
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
    switch (rand() % 5) {
        case 1: bubbleSort(lo, hi); break;
        case 2: selectionSort(lo, hi); break;
        case 3: mergeSort(lo, hi); break;
        case 4: heapSort(lo, hi); break;
        default: quickSort(lo, hi); break;
    }
}

// �����������
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {
    T* V = _elem + lo;
    for (Rank i = hi - lo; i > 0; i--) {
        std::swap(V[i - 1], V[rand() % i]);
    }
}

// ɾ���ظ�Ԫ��
template <typename T>
int Vector<T>::deduplicate() {
    int oldSize = _size;
    Rank i = 1;
    while (i < _size) {
        (find(_elem[i], 0, i) < 0) ? i++ : remove(i);
    }
    return oldSize - _size;
}

// ȥ�أ�����������
template <typename T>
int Vector<T>::uniquify() {
    Rank i = 0, j = 0;
    while (++j < _size) {
        if (_elem[i] != _elem[j]) _elem[++i] = _elem[j];
    }
    _size = ++i;
    shrink();
    return j - i;
}

// ��������������ָ�룩
template <typename T>
void Vector<T>::traverse(void(*visit)(T&)) {
    for (int i = 0; i < _size; i++) visit(_elem[i]);
}

// ������������������
template <typename T>
template <typename VST>
void Vector<T>::traverse(VST& visit) {
    for (int i = 0; i < _size; i++) visit(_elem[i]);
}



