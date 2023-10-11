// g++ -o sparse_matrix_test --std=c++20 -DMATRIX_TEST sparse_matrix.cpp

#include <unordered_map>
#if defined(MATRIX_TEST)
#include <assert.h>
#include <iostream>
#endif

using namespace std;

template<>
struct std::hash<pair<size_t, size_t>> {
    std::size_t operator()(const pair<size_t, size_t>& p) const noexcept {
        std::size_t h1 = hash<size_t>{}(p.first);
        std::size_t h2 = hash<size_t>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

template<typename ValueType = int>
class matrix {
    unordered_map<pair<size_t, size_t>, ValueType> sparse_values;
    const size_t num_rows, num_cols;

    constexpr bool is_out_of_range(size_t row, size_t col) const {
        return row >= num_rows || col >= num_cols;
    }

public:
    class out_of_range {};
    class size_mismatch {};

    matrix(size_t rows, size_t cols) : num_rows(rows), num_cols(cols) {}

    auto get(const size_t row, const size_t col) const -> ValueType {
        if (is_out_of_range(row,col)) {
            throw out_of_range();
        }
        
        if (sparse_values.contains({row,col})) {
            return sparse_values.at({row,col});
        }
        
        return 0;
    }

    void set(size_t row, size_t col, ValueType value) {
        if (is_out_of_range(row,col)) {
            throw out_of_range();
        }

        if (value == 0) {
            sparse_values.erase({row,col});
        } else {
            sparse_values[{row,col}] = value;
        }
    }

    void add(const matrix<ValueType>& right) {
        if (num_rows != right.num_rows || num_cols != right.num_cols) {
            throw size_mismatch();
        }

        for(auto &&node : right.sparse_values) {
            auto [row,col] = node.first;
            set(row, col, get(row,col) + node.second);
        }
    }
    
    #if defined(MATRIX_TEST)
    bool contains_zeros() const {
        for(auto &&node : sparse_values) {
            if(node.second == 0){
                return true;
            }
        }

        return false;
    }

    size_t number_of_values() {
        return sparse_values.size();
    }
    #endif

};

#if defined(MATRIX_TEST)
int main(int argc, char** argv) {
    cout << "STARTING TESTS" << endl;
    matrix m1{3,3};
    matrix m2{3,3};
    matrix m3{4,2};

    try {
        m1.get(5,2);
        assert(false);
    } catch (const matrix<int>::out_of_range& e) {}

    try {
        m1.set(11,5,-1);
        assert(false);
    } catch (const matrix<int>::out_of_range& e) {}

    try {
        m1.add(m3);
        assert(false);
    } catch (const matrix<int>::size_mismatch& e) {}

    for(size_t row = 0; row < 3; ++row) {
        for (size_t col = 0; col < 3; ++col) {
            assert(m1.get(row,col) == 0);
        }
    }

    m1.set(0,0,0);
    assert(!m1.contains_zeros());

    m1.set(0,1,5);
    assert(m1.get(0,1) == 5);
    m1.set(1,1,-1);
    assert(m1.get(1,1) == -1);
    m1.set(2,1,3);
    assert(m1.get(2,1) == 3);
    m1.set(1,0,2);
    assert(m1.get(1,0) == 2);
    m1.set(1,2,4);
    assert(m1.get(1,2) == 4);
    assert(!m1.contains_zeros());
    assert(m1.number_of_values() == 5);

    m2.set(0,1,1);
    m2.set(1,1,1);
    m2.set(2,1,-4);
    assert(m2.number_of_values() == 3);


    m1.add(m2);
    assert(m1.get(0,0) == 0);
    assert(m1.get(0,1) == 6);
    assert(m1.get(0,2) == 0);
    assert(m1.get(1,0) == 2);
    assert(m1.get(1,1) == 0);
    assert(m1.get(1,2) == 4);
    assert(m1.get(2,0) == 0);
    assert(m1.get(2,1) == -1);
    assert(m1.get(2,2) == 0);
    assert(!m1.contains_zeros());
    assert(m1.number_of_values() == 4);


    cout << "TESTS DONE" << endl;

    return 0;
}
#endif