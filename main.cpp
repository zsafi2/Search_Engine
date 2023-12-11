// Name: Zaheer Safi
// Date: 09/18/2023
// CS_251 Project_2 : Search Engine(Read a file and find the pages for the data that the user is asking for)

#include "search.h"
using namespace std;

// test cases for CleanToken function
bool testCleanToken() 
{
    if (cleanToken("Hello,") != "hello") return false;
    if (cleanToken("^*!World!") != "world") return false;
    if (cleanToken("&zaheer&331.") != "zaheer&331") return false;
    if (cleanToken("zaheersafi@331") != "zaheersafi@331") return false;
    if (cleanToken("!@#hi*") != "hi") return false;
    if (cleanToken("123") != "") return false;
    if (cleanToken("") != "") return false;
    
    return true;
}

// test cases for gatherTokenss function
bool testgatherTokens() 
{
    // Test cases for valid input
    set<string> tokens1 = gatherTokens("This is a test.");
    
    if (tokens1.size() != 4) return false; 
    if (tokens1.count("this") != 1) return false;
    if (tokens1.count("is") != 1) return false;
    if (tokens1.count("a") != 1) return false;
    if (tokens1.count("test") != 1) return false;

    set<string> tokens2 = gatherTokens("");
    if (!tokens2.empty()) return false;
    
    set<string> tokens3 = gatherTokens("123 456");
    if (!tokens3.empty()) return false;

    return true;
}

// test cases for BuildIndex function
bool testBuildIndex() 
{
    // Test cases for valid input
    map<string, set<string>> index1;
    int numPages1 = buildIndex("tiny.txt", index1); // Assuming you have a test_input.txt file
    if (numPages1 == 0) return false;

    map<string, set<string>> index2;
    int numPages2 = buildIndex("cplusplus.txt", index2);
    if (numPages2 == 0) return false;

    return true;
}

// // test cases for findQueryMatches function
bool testFindQueryMatches() 
{
    map<string, set<string>> index; // Assuming you have a populated index for testing
    buildIndex("tiny.txt", index);
    
    // Test cases for valid input
    set<string> queryResult1 = findQueryMatches(index, "fish");
    if (queryResult1.size() != 2) return false;

    set<string> queryResult2 = findQueryMatches(index, "fish not");
    if (queryResult2.size() != 3) return false;

    set<string> queryResult3 = findQueryMatches(index, "fish not green");
    if (queryResult3.size() != 4) return false;

    set<string> queryResult4 = findQueryMatches(index, "fish -red");
    if (queryResult4.size() != 1) return false;

    set<string> queryResult5 = findQueryMatches(index, "fish -not +milk");
    if (queryResult5.size() != 1) return false;

    return true;
}

int main() 
{
    bool check_case1 = true; 
    bool check_case2 = true;
    bool check_case3 = true;
    bool check_case4 = true;
    
    check_case1 = testCleanToken();
    check_case2 = testgatherTokens();
    check_case3 = testBuildIndex();
    check_case4 = testFindQueryMatches();
    
    
    // if all the cases passed run the code
    if (check_case1 && check_case2 && check_case3 && check_case4)
    {   
        // take the file name from the user
        string filename;
        cin >> filename;
        
        // pass it through the searchEngine function
        searchEngine(filename);
    }

    return 0;
}

