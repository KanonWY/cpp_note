#include <iostream>
#include <ostream>
#include <vector>

class STLBasic {
  public:
    STLBasic(int x) : val(x) { std::cout << "STLBasic(int x)" << std::endl; }

    STLBasic(const STLBasic &rhs) {
        val = rhs.val;
        std::cout << "copy" << std::endl;
    }

    STLBasic(STLBasic &&obj) noexcept {
        val = std::move(obj.val);
        std::cout << "move" << std::endl;
    }

    ~STLBasic() { std::cout << "~ST:STLBasic" << std::endl; }

    int get_val() const { return val; }
    friend std::ostream &operator<<(std::ostream &os, const STLBasic &rhs);

  private:
    int val = 0;
};

std::ostream &operator<<(std::ostream &os, const STLBasic &rhs) {
    os << rhs.val;
    return os;
}

int main() {
    std::vector<STLBasic> res;
    res.reserve(10);
    res.push_back(1);
    res.push_back(2);
    for (int i = 0; i < res.size(); ++i) {
        std::cout << res[i] << std::endl;
    }
    return 0;
}
