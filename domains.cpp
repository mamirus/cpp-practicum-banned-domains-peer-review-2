#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>

using namespace std;

class Domain {
public:
    explicit Domain(const string_view str) : domain_(str) {
        reverse(domain_.begin(), domain_.end());
        domain_.push_back('.');
    }

    bool IsSubdomain(const Domain& other) const {
        if (domain_.size() < other.domain_.size()) {
            return false;
        }
        auto res = mismatch(other.domain_.begin(), other.domain_.end(), domain_.begin());

        return res.first == other.domain_.end();
    }

    string_view GetDomain() const {
        return domain_;
    }

    friend bool operator==(const Domain& left, const Domain& right);

private:
    string domain_;
};

bool operator==(const Domain& left, const Domain& right) {
    return left.domain_ == right.domain_;
}

class DomainChecker {
public:
    explicit DomainChecker(const vector<Domain>& forbidden_domains) {
        forbidden_domains_ = forbidden_domains;
        sort(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetDomain() < rhs.GetDomain();
            });
        auto last = unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return rhs.IsSubdomain(lhs);
            });
        forbidden_domains_.erase(last, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) const {
        const auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetDomain() < rhs.GetDomain();
            });

        if (it == forbidden_domains_.begin()) {
            return false;
        }

        return domain.IsSubdomain(*prev(it));
    }

private:
    vector<Domain> forbidden_domains_;
};

vector<Domain> ReadDomains(istream& in, const size_t number_of_lines) {
    vector<Domain> result;
    result.reserve(number_of_lines);
    for (size_t i = 0; i < number_of_lines; ++i) {
        string line;
        getline(in, line);
        result.emplace_back(line);
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    const DomainChecker checker(forbidden_domains);

    const vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const auto& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }

    return 0;
}
