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

    cout << endl << endl << endl;
    
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
    vector<pair<string, int>> list;
    for(auto e : diction.freqWord) {
        list.push_back({e.second, e.first});
        
    }
    sort(list.begin(), list.end());


// -----------------------

    int offset=20;
    int longest=diction.longWord.top().length();
    int word_count=get<1>(diction.stats);
    // stars     i  count
    int longest_star=list[0].second;
    for(auto e : list) {
        if(longest_star<e.second)
            longest_star=e.second;
    }
   
    for(int i=longest_star; i>=0; --i) {  
        for(int j=0; j<offset+longest; ++j)
            cout << " ";
        for(int j=0; j<word_count; ++j) {
            if(list[j].second>i) 
                cout << "*  ";
            else 
                cout << "   ";
        }
        cout << endl;
    }

    // line     
    int set_count=0;
    for(const auto e : diction.words)
        ++set_count;
    for(int i=0; i<offset+longest+set_count*3; ++i) 
        cout << "-";
    cout << endl;

    // arrow heads
    for(int i=0; i<longest+offset; ++i)
        cout << " ";
    for(int i=0; i<set_count; ++i) 
        cout << "^  ";



    // words and arrow bodies...
    cout << endl;
    int curr_word=set_count;
    int spacing=0;
    for(auto e : list) {
        cout << e.first;
        for(int i=0; i<longest-e.first.length()+1; ++i)
            cout << " ";
        for(int i=0; i<offset-1; ++i) {
            cout << "-";
        }
        for(int i=0; i<spacing; ++i)
            cout << "-";
        spacing+=3; 
        cout << "/";
        for(int i=0; i<curr_word-1; ++i) 
            cout << "  |";
        --curr_word;
        cout << endl;
    }
}
