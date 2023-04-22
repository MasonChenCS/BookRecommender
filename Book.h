#pragma once

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <queue>


struct Book {
    // Variables
    unsigned int id;
    std::string title;
    std::string author;
    float rating;
    std::string isbn;
    unsigned long long int isbn13;
    std::string language;
    unsigned int pageCount;
    unsigned int ratingCount;
    unsigned int reviewCount;

    // Functions
    static std::string lowercase(const std::string &inputString);

    static std::string uppercase(const std::string &inputString);

    static std::vector<std::string> splitText(const std::string text);

    std::string printBookInfo() const;

    Book() {}

    ~Book() {}
};


// Convert a string to lowercase
std::string Book::lowercase(const std::string &inputString) {
    std::string outputString = "";
    for (unsigned int i = 0; i < inputString.length(); i++) {
        outputString += tolower(inputString[i]);
    }
    return outputString;
}


// Convert a string to uppercase
std::string Book::uppercase(const std::string &inputString) {
    std::string outputString = "";
    for (unsigned int i = 0; i < inputString.length(); i++) {
        outputString += toupper(inputString[i]);
    }
    return outputString;
}


// Tokenize a string
std::vector<std::string> Book::splitText(const std::string text) {
    std::vector<std::string> results = {};
    std::string token = "";
    for (unsigned int i = 0; i < text.length(); i++) {
        if (isalpha(text[i])) {
            token += text[i];
            if (i == text.length() - 1)
                results.push_back(lowercase(token));
        } else {
            if (token != "")
                results.push_back(lowercase(token));
            token = "";
        }
    }
    return results;
}


// Print all variables of a book
std::string Book::printBookInfo() const {
    return title + " by " + author + "\nBook ID: " + std::to_string(id) + " | Language: " + language + " | ISBN: " +
           isbn + " | ISBN13: " + std::to_string(isbn13) + "\nPage Count: " + std::to_string(pageCount) +
           " | Rating: " + std::to_string(rating).substr(0, 4) + " | Rating Count: " + std::to_string(ratingCount) +
           " | Review Count: " + std::to_string(reviewCount);
}
