#include <list>
#include "Book.h"


class ListStruct {
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
    std::list<Node *> list = {}; // The central data structure is a C++ list
    void insert(Book book_);

    std::vector<Book *>
    searchKeywords(std::vector<std::string> keywords, const std::string searchMode, const std::string matchMode);

    std::vector<Book *> searchApproximate(const float &key, const std::string searchMode, const unsigned int capacity);

    Book *searchExact(const std::string &key, const std::string searchMode);

    ~ListStruct();

    struct nodeComparator { // The wrapper for the Node comparator
        bool operator()(Node *n1, Node *n2) {
            return *n1 < *n2;
        }
    };
};


// De-allocate and delete all elements
ListStruct::~ListStruct() {
    while (!list.empty()) {
        delete list.back();
        list.pop_back();
    }
}


// Insert a new element
void ListStruct::insert(Book book_) {
    list.push_back(new Node(book_));
}


// Search for title or author keywords
std::vector<Book *> ListStruct::searchKeywords(std::vector<std::string> keywords, const std::string searchMode,
                                               const std::string matchMode) {
    std::vector<Book *> results = {};
    for (Node *bookNode: list) {
        for (unsigned int j = 0; j < keywords.size(); j++) {
            if (matchMode == "exact") {
                Book *tempBook = &bookNode->book;
                if (Book::lowercase(searchMode == "title" ? tempBook->title : tempBook->author).find(keywords[j]) !=
                    std::string::npos) {
                    results.push_back(tempBook);
                    break;
                }
            } else if (matchMode == "tokenized") {
                Book *tempBook = &bookNode->book;
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
ListStruct::searchApproximate(const float &key, const std::string searchMode, const unsigned int capacity) {
    std::vector<Book *> results = {};
    std::priority_queue<Node *, std::vector<Node *>, nodeComparator> pq;
    for (Node *bookNode: list) {
        bookNode->searchMode = searchMode;
        bookNode->key = key;
        pq.push(bookNode);
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
Book *ListStruct::searchExact(const std::string &key, const std::string searchMode) {
    for (Node *bookNode: list) {
        if ((searchMode == "ID" && std::to_string(bookNode->book.id) == key) ||
            (searchMode == "ISBN" && Book::lowercase(bookNode->book.isbn) == Book::lowercase(key)) ||
            (searchMode == "ISBN13" && std::to_string(bookNode->book.isbn13) == key))
            return &bookNode->book;
    }
    return nullptr;
}
