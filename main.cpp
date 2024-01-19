#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    // место для вашей реализации
    int res = 0;
    unordered_map<size_t, unordered_set<string>> hash_results;
    string word;
    while(text >> word){        
        size_t res = hasher(word);
        bool is_hash = (hash_results.count(res) > 0);
        if(!is_hash){ 
            hash_results[res].insert(move(word));
            continue;
        }
        auto hash_size = hash_results.at(res).size();
        hash_results.at(res).insert(move(word));
        res += hash_results.at(res).size() - hash_size;
    }
    return res;
}

// Это плохой хешер. Его можно использовать для тестирования.
// Подумайте, в чём его недостаток
struct HasherDummy {
    size_t operator() (const string& str) const {
        size_t res = 0;
        for (char c : str) {
            res += static_cast<size_t>(c);
        }
        return res;
    }
};

int main() {
    hash<string> str_hasher;
    int collisions = FindCollisions(str_hasher, cin);
    cout << "Found collisions: "s << collisions << endl;
}
