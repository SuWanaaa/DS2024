template <typename T>
class BinTreeNode {
public:
    T data;
    int freq;
    BinTreeNode *left, *right;

    BinTreeNode(T data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};


