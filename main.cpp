#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

using WORD=string;
using WORD_LIST=set<WORD>;
using WORD_FREQ_LIST=map<char,int>;
using FREQ_WORD_LIST=multimap<int,WORD>;
using COUNTER=tuple<int,int,int>;
using LONGEST=priority_queue<WORD>;

struct DICTION {
    COUNTER stats;
    WORD_LIST words;
    WORD_FREQ_LIST wordFreq;
    FREQ_WORD_LIST freqWord;
    LONGEST longWord;
};

void fill_diction(DICTION& diction);
WORD get_next_word(DICTION& diction, int& chars, int& words, int& lines);
void print_header(string str);
void print_stats(DICTION& diction);
void print_letter_freq(DICTION& diction);
void print_dictionary(DICTION& diction);
void print_histogram(DICTION& diction);

int main() {
    
    DICTION diction;
   
    fill_diction(diction); 
    print_stats(diction);
    print_letter_freq(diction);
    print_dictionary(diction);
    print_histogram(diction);
    
    return 0; 
}
// this function handles the input file.
// and converts it into my diction structure.
void fill_diction(DICTION& diction) {
    int char_count=0; 
    int word_count=0;
    int line_count=0;
    while(!cin.eof()) {
        WORD current_word=get_next_word(
                diction, 
                char_count, 
                word_count, 
                line_count);
        if(current_word=="")
            continue;
        diction.words.insert(current_word);        
        bool new_word=true;
        for(auto it=diction.freqWord.begin(); it!=diction.freqWord.end(); ++it) {
            if(it->second==current_word) {
                int prev=it->first;
                diction.freqWord.erase(it);
                diction.freqWord.insert({prev+1, current_word});
                new_word=false;
                break;
            }
        }
        if(new_word)
            diction.freqWord.insert({1, current_word});
        if(diction.longWord.empty()) 
            diction.longWord.push(current_word);
        else if(diction.longWord.top().length()==current_word.length())
            diction.longWord.push(current_word);
        else if(diction.longWord.top().length()<current_word.length()) {
            while(!diction.longWord.empty())
                diction.longWord.pop();
            diction.longWord.push(current_word);
        } 
    }
    diction.stats={char_count, word_count, line_count}; // '\n' is a char...
}

//                   |||
// FRANCO'S FUNCTION vvv (with changes)
WORD get_next_word(DICTION& diction, int& chars, int& words, int& lines) {
    WORD w;
    char ch;
    bool inWord=false;
    cin.get(ch);
    while(!cin.eof()) {
        ++chars;
        if(ch=='\n')
            ++lines;
        if(isalpha(ch)) {
            w.push_back(tolower(ch)); 
            inWord=true;             
            diction.wordFreq[tolower(ch)]++;
        } else if(inWord) {             
            ++words;
            return w;
        }
        cin.get(ch);
    }
    return w;
}

void print_header(string str) {
    int n=str.length();
    //top
    cout << "/";
    for(int i=0; i<n+2; ++i)
        cout << "-";
    cout << "\\" << endl;
    //middle
    cout << "| " << str << " |" << endl;
    //bottom
    cout << "\\";
    for(int i=0; i<n+2; ++i)
        cout << "-";
    cout << "/" << endl;
}

void print_stats(DICTION& diction) {
    cout << endl;
    print_header("Stats");
    cout << endl << "Words in dictionary: ";
    int count=0;
    for(const auto e : diction.words) {
        if(count!=0&&count%5==0) 
            cout << endl << "                     ";
        cout << e << ", ";
        ++count;
    }
    cout << "\b\b  " <<  endl << endl;
    cout << "Number of Chars   : " << get<0>(diction.stats) << endl;
    cout << "Number of Words   : " << get<1>(diction.stats) << endl;
    cout << "Number of Lines   : " << get<2>(diction.stats) << endl; 
}

void print_letter_freq(DICTION& diction) {
    cout << endl;
    print_header("Letter Freq");
    cout << endl;
    for(auto e : diction.wordFreq) {
        cout << e.first << " | ";
        int n=e.second;
        bool add_num=false;
        if(n>10) {
            n=10;
            add_num=true;
        }
        for(int i=0; i<n; ++i) {
            cout << "*";
        }
        if(add_num) {
            cout << "  (" << e.second << ")";
        }
        cout << endl;
    }
}

void print_dictionary(DICTION& diction) {
    cout << endl;
    print_header("Dictionary");
    cout << endl;
    vector<pair<string, int>> list;
    for(auto e : diction.freqWord) {
        list.push_back({e.second, e.first});
        
    }
    sort(list.begin(), list.end());
    cout << "Word" << "                                  " << "Freq" << endl;
    cout << "-------------------------------------------" << endl;
    for(auto e : list) {
        cout << e.first;
        int n=40-e.first.length();
        for(int i=0; i<n; ++i)
            cout <<" "; 
        cout << e.second << endl;
    }
}

void print_histogram(DICTION& diction) {
    cout << endl;
    print_header("Histogram");
    cout << endl;
}









