#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <set>

using namespace std;

// Class LetterSet represents a set of letters with a bitmask
class LetterSet{
public:
    uint32_t mask;

    LetterSet(string s) : mask(0){

        // Build the bitmask from the characters in s
        for(char c : s){
            if(c >= 'a' && c <= 'z'){
                mask |= 1u << (c - 'a');
            }
        }

        // Scandic nonsense
        if(s.find("ä") != string::npos) mask |= 1u << 26;
        if(s.find("ö") != string::npos) mask |= 1u << 27;
        if(s.find("å") != string::npos) mask |= 1u << 28;        
    }

    // Counts the number of bits set in the mask of the object
    // From http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
    int size() const{
        uint32_t v = mask - ((mask >> 1) & 0x55555555);
        v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
        return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
    }

    // Comparison operator for std::map
    bool operator<(const LetterSet& other) const{
        return this->mask < other.mask;
    }

    // Equality operator
    bool operator==(const LetterSet& other) const{
        return this->mask == other.mask;
    }

    // Addition operator takes the union of the LetterSets
    friend LetterSet operator+(const LetterSet& a, const LetterSet& b);
};

LetterSet operator+(const LetterSet& a, const LetterSet& b){
    LetterSet result("");
    result.mask = (a.mask | b.mask);
    return result;
}

int main(){
    // Make IO fast
    ios::sync_with_stdio(0);
    cin.tie(0);

    vector<LetterSet> letterSets; // All distinct LetterSets in the data
    map<LetterSet, set<string> > setToWords; // Map: LetterSet -> (strings that have that letter set)

    // Read the input
    while(true){
        string s; cin >> s;
        if(!cin.good()) break;
        transform(s.begin(), s.end(), s.begin(), ::tolower); // To lower case
        LetterSet letters(s);
        if(setToWords.find(letters) == setToWords.end()) // If LetterSet not seen yet
            letterSets.push_back(letters);
        setToWords[letters].insert(s);
    }

    // Sort LetterSets by the number of letters
    sort(letterSets.begin(), letterSets.end(), 
        [](const LetterSet& a, const LetterSet& b){return a.size() < b.size();});

    vector<pair<LetterSet,LetterSet> > answer;
    int best = 0; // The size of the biggest union of letter sets

    // Compare all pairs of LetterSets.
    // Keep track of the size of the largest union so far, and skip
    // testing LetterSets A,B such that A.size() + B.size() < (the largest union so far)
    for(int i = letterSets.size() - 1; i >= 0; i--){
        for(int j = i - 1; j >= 0; j--){
            if(letterSets[i].size() + letterSets[j].size() < best) 
                break;
            int unionSize = (letterSets[i] + letterSets[j]).size();        
            if(unionSize > best){
                best = unionSize;
                answer.clear();
                answer.push_back(make_pair(letterSets[i], letterSets[j]));
            }
            else if (unionSize == best){
                answer.push_back(make_pair(letterSets[i], letterSets[j]));
            }
        }
    }

    // Print the answer
    for(auto X : answer){
        for(auto s : setToWords[X.first]) cout << s << " ";
        cout << "- ";
        for(auto s : setToWords[X.second]) cout << s << " ";
        cout << endl;
    }
}

