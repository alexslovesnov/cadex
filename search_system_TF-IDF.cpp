#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}
// изменения урока 8 здесь
struct Document {
    int id;
    double relevance;
};

class SearchServer {
public:
    // автоматически добавляем стоп-слова с минусом
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
            stop_words_.insert("-"s + word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        int N = words.size();
        // изменения урока 8 здесь
        ++document_count_;
        for(auto word : words){
            doc_index_[word][document_id] += 1.0/N;
        }
    }        

    vector<Document> FindTopDocuments(const string& raw_query) const{
        const set<string> query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }
    
    void PrintWordDoc(const string& word) const{
        cout << "List of documents for " << word << ":"s << endl;
        for(const auto& [key, value] : doc_index_.at(word)){
            cout << "docId = "s << key << ", TF = "s << value << endl;
        }
    }

    /*
    double Test(const string& raw_query) const{
        const set<string> query_words = ParseQuery(raw_query);
        return DocRelevance(1, query_words);
    } */
  
private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };
    // изменения урока 8 здесь
    int document_count_ = 0;

    vector<DocumentContent> documents_;
    
    // изменения урока 8 здесь
    map<string, map<int, double>> doc_index_;
    //map<string, set<int>> doc_index_;

    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    set<string> ParseQuery(const string& text) const {
        set<string> query_words;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            query_words.insert(word);
        }
        return query_words;
    }

    vector<Document> FindAllDocuments(const set<string>& query_words) const{
        vector<Document> matched_documents;
        map<int, double> res;

        // изменения урока 8 здесь
        set<string> minus_words;
        set<string> plus_words;
        map<string, double> words_idf;
        
        //Разделяем запрос на плюс-слова и минус-слова
        // Слова, отсутствующие в индексе игнорируем
        // Для плюс-слов определяем idf
        for(const string& word : query_words){
            if(word[0] == '-'){
                string n_word = word.substr(1, word.size()-1);
                if(word.size() > 1 && doc_index_.count(n_word) > 0){                    
                    minus_words.insert(n_word);
                }
            }else if(doc_index_.count(word) > 0){
                plus_words.insert(word);
                words_idf[word] = log((document_count_ + 0.0)/doc_index_.at(word).size());
            } 
        }
        
        //Определяем релевантность документов по плюс словам
        for(const string& word : plus_words){
            for(const auto [key, value] : doc_index_.at(word)){
                if(res.count(key) == 0){
                    res[key] = words_idf.at(word)*doc_index_.at(word).at(key);
                } else{
                    res[key] += words_idf.at(word)*doc_index_.at(word).at(key);
                }
            }
        }
        //Убираем документы по минус словам
        for(const string& word : minus_words){
            if(doc_index_.count(word) > 0){
                for(const auto [key, value] : doc_index_.at(word)){
                    res.erase(key);
                }
            }
        }
        
        // Записываем результат map в вектор Document
        for(const auto [key, value] : res){
            matched_documents.push_back({key, value});
        }
        
        return matched_documents;
    }

};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}


int main() {

    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    
    for (auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
    
    
    //search_server.Test();
    //search_server.PrintWordDoc(t_word);
    //cout << "relevance = " << search_server.Test(query) << endl;
    
}