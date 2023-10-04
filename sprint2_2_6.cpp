#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Document {
    int id;
    int rating;
};

bool HasDocumentGreaterRating(Document a, Document b){
    return a.rating < b.rating;
}

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish) {
    // отсортировать документы и вывести не все
    //sort(documents.begin(), documents.end(), HasDocumentGreaterRating);
    int size = static_cast<int>(documents.size());
    int start = static_cast<int>(skip_start);
    int finish = static_cast<int>(skip_finish);
    //cout << "size = "s << size << endl;
    for(int i = start; i < size - finish; ++i){
        cout << "{ id = "s << documents[i].id << ", rating = "s << documents[i].rating << " }" << endl;
    }
}

int main() {
    PrintDocuments({{100, 5}, {101, 7}, {102, -4}, {103, 9}, {104, 1}}, 1, 2);
}
