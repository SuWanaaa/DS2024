#include <vector>

class Bitmap {
private:
    std::vector<unsigned char> M;
    int N;

public:
    Bitmap(int n = 8) : M((n + 7) / 8, 0), N(n) {}

    void set(int k) { M[k >> 3] |= (0x80 >> (k & 0x07)); }
    void clear(int k) { M[k >> 3] &= ~(0x80 >> (k & 0x07)); }
    bool test(int k) const { return M[k >> 3] & (0x80 >> (k & 0x07)); }
    std::string bits2string(int n) const {
        std::string s;
        for (int i = 0; i < n; i++)
            s += test(i) ? '1' : '0';
        return s;
    }
};


