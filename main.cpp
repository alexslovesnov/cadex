// НЕрабочая версия программы, выдается ошибка
//Ошибка компиляции:
///exposed/submission/main.cpp: In constructor ‘StreamUntier::StreamUntier(std::istream&)’:
///exposed/submission/main.cpp:26:14: error: ‘StreamUntier::main_stream_’ will be initialized after [-Werror=reorder]
//   26 |     istream* main_stream_;
//      |              ^~~~~~~~~~~~
//compilation terminated due to -Wfatal-errors.
//cc1plus: all warnings being treated as errors
 

#include "log_duration.h"

#include <iostream>

using namespace std;

class StreamUntier {
public:
    // добавьте конструктор, деструктор
    // и дополнительные поля класса при необходимости
    StreamUntier() = default;
    
    StreamUntier(istream& stream_name):
        main_stream_(&stream_name),
        tied_before_(stream_name.tie(nullptr))
    {
       
    }
    
    ~StreamUntier(){
        (*main_stream_).tie(tied_before_);
    }

private:
    ostream* tied_before_;
    istream* main_stream_;
};

int main() {
    LOG_DURATION("\\n with tie"s);

    StreamUntier guard(cin);
    int i;
    while (cin >> i) {
        cout << i * i << "\n"s;
    }

    return 0;
}

// Рабочая версия

#include "log_duration.h"

#include <iostream>

using namespace std;

class StreamUntier {
public:
    // добавьте конструктор, деструктор
    // и дополнительные поля класса при необходимости
    StreamUntier() = default;
    
    StreamUntier(istream& stream_name):
        tied_before_(stream_name.tie(nullptr))
    {
        main_stream_ = &stream_name;
    }
    
    ~StreamUntier(){
        (*main_stream_).tie(tied_before_);
    }

private:
    ostream* tied_before_;
    istream* main_stream_;
};

int main() {
    LOG_DURATION("\\n with tie"s);

    StreamUntier guard(cin);
    int i;
    while (cin >> i) {
        cout << i * i << "\n"s;
    }

    return 0;
}
