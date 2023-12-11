// Name: Zaheer Safi
// Date: 09/18/2023
// CS_251 Project_2 : Search Engine(Read a file and find the pages for the data that the user is asking for)

#pragma once

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// This function takes a string and removes leading and trailing punctuation from string and returns the string
string cleanToken(string s) 
{
    bool check = false; // check if the string has at least one alphabet
    
    // if the string is empty return empty string
    if (s.empty()) 
    {
        return "";
    }
    
    // check if the string has at least one alphabet
    for (char &c : s)
    {
        if (isalpha(c))
        {
            check = true;
            break;
        }
    }
    
    // if there is no alphabet in the string return empty string
    if (check == false)
    {
        return "";
    }

    // convert string to lowercase
    for (char &c1 : s) 
    {
        c1 = tolower(c1);
    }

    // Remove punctuation from the beginning
    while (!s.empty() && ispunct(s.front())) 
    {
        s.erase(0, 1);
    }

    // Remove punctuation from the end
    while (!s.empty() && ispunct(s.back())) 
    {
        s.pop_back();
    }
    
    return s;
}

// This function takes a sentence string and then on each word it applys the cleanToken function on it and stores it in a set and return it.
set<string> gatherTokens(string text) 
{
    set<string> query; // set to store clean words
    string word; // to store each word
    string clean_word; // store word after cleaning
    
    std::istringstream iss(text); // string stream to go over each word
    
    // go through each word and store in query
    while (iss >> word) 
    {
        clean_word = cleanToken(word);
        if (!clean_word.empty())
        {
            query.insert(cleanToken(word));
        }
    }
    
    return query; 
}

// This function reades from a file with alternating lines where first line is a page and second line is data and stores all of this in a map.
int buildIndex(string filename, map<string, set<string>>& index) 
{
    string URL; // to store URLS
    string data; // to store the data
    int pages = 0; // count number of pages
    
    ifstream file(filename); // open the file

    // if the file is not available show the message and return 0
    if (!file.is_open()) {
        cout << "Invalid filename." << endl;
        return 0;
    }

    // loop until the end of file
    while (getline(file, URL) && getline(file, data)) 
    {
        set<string> data1 = gatherTokens(data);

        // Remove any empty strings from data1
        data1.erase("");

        // Insert each URL into the corresponding word's set
        for (const string& word : data1) 
        {
            index[word].insert(URL);
        }
        
        ++pages;
    }

    // Remove entries with an empty key (if any)
    index.erase("");

    return pages;
}

// This function reades the map build by build Index function and fund matches according to user input and returns a set of pages.
set<string> findQueryMatches(map<string, set<string>>& index, string sentence)
{
    set<string> result; 

    stringstream iss(sentence); // go through each word in the sentence using string stream
    string word;
    string sign; // to store if the word has a modifier sign

    // iterate through each word
    while (iss >> word)
    {
        // if word has a plus sign in the beggining remove it and add '+" to the sign
        if (word.at(0) == '+')
        {
            sign = "+";
            word = word.substr(1);
        }

        // if word has a plus sign in the beggining remove it and add '-" to the sign
        else if(word.at(0) == '-')
        {
            sign = "-";
            word = word.substr(1);
        }
        
        // if the word has no sign make sign equal to a space
        else if (isalpha(word.at(0)))
        {
            sign = " ";
        }

        word = cleanToken(word); // pass the word through clean token

        // if sign in the beggining of the word is "+"
        if (sign == "+")
        {
            // if this is the first word which means the result will be empty
            if (result.empty())
            {
                result = index[word]; // make the result set equal to the set of urls mapped witht the word
            }
            
            // else if the word is not the first word
            else
            {
                set<string> result1; // define an empty set
                
                // find the intersection of results that hold the data from before with the new data and store it in result1
                set_intersection(result.begin(), result.end(), index[word].begin(), index[word].end(), inserter(result1, result1.begin()));
                result = result1;
                result1.clear(); // clear result1
            }
        }

        // if the sign is negative do the same thing except fidn the difference between result and the value of the map set
        else if (sign == "-")
        {
            if (result.empty())
            {
                result = index[word];
            }
            else
            {
                set<string> result2;
                set_difference(result.begin(), result.end(), index[word].begin(), index[word].end(), inserter(result2, result2.begin()));
                result = result2;
                result2.clear();
            }
        }
        
        // if the sgin is a space which means no modifier simply find the union of previous and new result
        else if (sign == " ")
        {
            if (result.empty())
            {
                result = index[word];
            }
            else
            {
                set<string> result3;
                set_union(result.begin(), result.end(), index[word].begin(), index[word].end(), inserter(result3, result3.begin()));
                result = result3;
                result3.clear();
            }
        }
    }
    return result;
}

// This function takes filename from the user opens it and applys all the function above to output the result in to console
void searchEngine(string filename) 
{
    map<string, set<string>> index;
    set<string> result;
    string sentence;
    string key;
    int words = 0;

    int pages = buildIndex(filename, index); // recall the buildIndex function to count the number of pages and read the file
    
    cout << "Stand by while building index..." << endl;
        
    // we passes the map to buildIndex now we can count unique words
    for (auto it = index.begin(); it != index.end(); ++it) 
    {
        key = it->first;
        ++words;
    }
           
    cout << "Indexed " << pages << " pages containing " << words << " unique terms" << endl;
    cout << endl;
    cin.ignore();
    
    do
    {  
        // take user input
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, sentence);
        
        // if the user press enter quit the program
        if (sentence.empty())
        {
            break;
        }
        
        // pass the sentence through findQueryMatches according to the map
        result = findQueryMatches(index, sentence);
        cout << "Found " << result.size() << " matching pages" << endl;
        
        // output all the urls stores in the result
        for (const string & k : result)
        {
            cout << k << endl;
        }
        cout << endl;
    
    }while(true);
    
    cout << "Thank you for searching!" << endl;        
}


