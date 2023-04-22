#include "Book.h"


class VectorStruct {
    struct Node {
        Book book;
        float key;
        std::string searchMode;

        bool operator<(const Node &n) const { // The comparator for the min heap priority queue
            if (searchMode == "Rating")
                return fabs(this->book.rating - key) < fabs(n.book.rating - key);
            else
                return fabs((float) this->book.pageCount - key) < fabs((float) n.book.pageCount - key);
        }

        Node(Book book_) : book(book_) {}

        ~Node() {}
    };

public:
    std::vector<Node *> vec = {}; // The central data structure is a C++ vector
    void insert(Book book_);

    std::vector<Book *>
    searchKeywords(std::vector<std::string> keywords, const std::string searchMode, const std::string matchMode);

    std::vector<Book *> searchApproximate(const float &key, const std::string searchMode, const unsigned int capacity);

    Book *searchExact(const std::string &key, const std::string searchMode);

    ~VectorStruct();

    struct nodeComparator { // The wrapper for the Node comparator
        bool operator()(Node *n1, Node *n2) {
            return *n1 < *n2;
        }
    };
};


// De-allocate and delete all elements
VectorStruct::~VectorStruct() {
    while (!vec.empty()) {
        delete vec.back();
        vec.pop_back();
    }
}


// Insert a new element
void VectorStruct::insert(Book book_) {
    vec.push_back(new Node(book_));
}


// Search for title or author keywords
std::vector<Book *> VectorStruct::searchKeywords(std::vector<std::string> keywords, const std::string searchMode,
                                                 const std::string matchMode) {
    std::vector<Book *> results = {};
    for (unsigned int i = 0; i < vec.size(); i++) {
        for (unsigned int j = 0; j < keywords.size(); j++) {
            if (matchMode == "exact") {
                Book *tempBook = &vec[i]->book;
                if (Book::lowercase(searchMode == "title" ? tempBook->title : tempBook->author).find(keywords[j]) !=
                    std::string::npos) {
                    results.push_back(tempBook);
                    break;
                }
            } else if (matchMode == "tokenized") {
                Book *tempBook = &vec[i]->book;
                std::vector<std::string> book = tempBook->splitText(
                        searchMode == "title" ? tempBook->title : tempBook->author);
                if (std::find(book.begin(), book.end(), keywords[j]) != book.end()) {
                    results.push_back(tempBook);
                    break;
                }
            }
        }
    }
    return results;
}


// Search an approximate value (rating or page count)
std::vector<Book *>
VectorStruct::searchApproximate(const float &key, const std::string searchMode, const unsigned int capacity) {
    std::vector<Book *> results = {};
    std::priority_queue<Node *, std::vector<Node *>, nodeComparator> pq;
    for (unsigned int i = 0; i < vec.size(); i++) {
        vec[i]->searchMode = searchMode;
        vec[i]->key = key;
        pq.push(vec[i]);
        if (pq.size() > capacity)
            pq.pop();
    }
    while (!pq.empty()) {
        results.push_back(&pq.top()->book);
        pq.pop();
    }
    return results;
}


// Search an exact value (ID, ISBN, or ISBN13)
Book *VectorStruct::searchExact(const std::string &key, const std::string searchMode) {
    for (unsigned int i = 0; i < vec.size(); i++) {
        if ((searchMode == "ID" && std::to_string(vec[i]->book.id) == key) ||
            (searchMode == "ISBN" && Book::lowercase(vec[i]->book.isbn) == Book::lowercase(key)) ||
            (searchMode == "ISBN13" && std::to_string(vec[i]->book.isbn13) == key))
            return &vec[i]->book;
    }
    return nullptr;
}
