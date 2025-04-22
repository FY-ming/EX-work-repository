#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 大写转小写
char myToLower(int c){
    if( c >= 'A' && c <= 'Z'){
        return c - 'A' + 'a';
    }
    return c;
}
// -----------------------------------------------------------------------------------------------------------------------
// BF逐一子串检索，时间复杂度O(n*m)
bool bfSearch(const string& str, const string& sub) {
    int len1 = str.length();
    int len2 = sub.length();
    bool match = true;
    for (int i = 0; i <= len1 - len2; ++i) {
        match = true;
        for (int j = 0; j < len2; ++j) {
            if (myToLower(str[i + j]) != myToLower(sub[j])) {
                match = false;
                break;
            }
        }
        if (match) {
            return true;
        }
    }
    return false;
}
// ----------------------------------------------------------------------------------------------------------------
// 生成部分匹配表
vector<int> computeLPSArray(const string& sub) {
    int m = sub.length();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;
    while (i < m) {
        if (myToLower(sub[i]) == myToLower(sub[len])) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}
// KMP算法,时间复杂度O(n+m)
bool kmpSearch(const string& str, const string& sub) {
    int n = str.length();
    int m = sub.length();
    vector<int> lps = computeLPSArray(sub);
    int i = 0; // 文本的索引
    int j = 0; // 模式的索引
    while (i < n) {
        if (myToLower(sub[j]) == myToLower(str[i])) {
            j++;
            i++;
        }
        if (j == m) {
            return true;
        } else if (i < n && myToLower(sub[j]) != myToLower(str[i])) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return false;
}
// ----------------------------------------------------------------------------------------------------------
// 坏字符规则
vector<int> badCharHeuristic(const string& sub) {
    const int SIZE = 256;
    vector<int> badchar(SIZE, -1);
    int m = sub.length();
    for (int i = 0; i < m; i++) {
        badchar[(int)sub[i]] = i;
    }
    return badchar;
}
// BM算法,平均时间复杂度接近O(n/m) , 最坏情况下的时间复杂度为O(n+m)
bool boyer_Moore_Search(const string& str, const string& sub) {
    int n = str.length();
    int m = sub.length();
    vector<int> badchar = badCharHeuristic(sub);
    int s = 0; // 偏移量
    while (s <= (n - m)) {
        int j = m - 1;
        while (j >= 0 && myToLower(sub[j]) == myToLower(str[s + j])) {
            j--;
        }
        if (j < 0) {
            return true;
        } else {
            s += max(1, j - badchar[(int)str[s + j]]);
        }
    }
    return false;
}
// ----------------------------------------------------------------------------------------------------------

// Rabin-Karp算法,平均时间复杂度O(n+m) , 最坏情况下的时间复杂度为O(n*m)
bool rabin_Karp_Search(const string& str, const string& sub) {
    int n = str.length();
    int m = sub.length();
    const int q = 101; // 素数
    const int d = 256; // 字符集大小
    int h = 1;
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }
    int p = 0; // 模式的哈希值
    int t = 0; // 文本的哈希值
    for (int i = 0; i < m; i++) {
        p = (d * p + myToLower(sub[i])) % q;
        t = (d * t + myToLower(str[i])) % q;
    }
    for (int i = 0; i <= n - m; i++) {
        if (p == t) {
            int j;
            for (j = 0; j < m; j++) {
                if (myToLower(str[i + j]) != myToLower(sub[j])) {
                    break;
                }
            }
            if (j == m) {
                return true;
            }
        }
        if (i < n - m) {
            t = (d * (t - myToLower(str[i]) * h) + myToLower(str[i + m])) % q;
            if (t < 0) {
                t = t + q;
            }
        }
    }
    return false;
}


int main() {
    // string d1 = "I like to watch the sun set with my friend.";
    // string d2 = "The Best Places To Watch The Sunset.";
    // string d3 = "My friend watch the sun come up.";
    // string input;
    // cout << "请输入搜索关键词: ";
    // cin >> input;
    // if (myFind(d1,input)) {
    //     cout << "d1" << endl;
    // }
    // if (myFind(d2,input)) {
    //     cout << "d2" << endl;
    // }
    // if (myFind(d3,input)) {
    //     cout << "d3" << endl;
    // }

    //string doc[] = {"I like to watch the sun set with my friend.","The Best Places To Watch The Sunset.","My friend watch the sun come up."};
    vector<string> doc; 
    doc.push_back("I like to watch the sun set with my friend.");
    doc.push_back("The Best Places To Watch The Sunset.");
    doc.push_back("My friend watch the sun come up.");
    string input;
    cout << "请输入搜索关键词: \n";
    // cin >> input;
    getline(cin, input);
    for (int i = 0; i < doc.size(); i++) {
        if (bfSearch(doc[i],input)) {
            cout << "d" << i+1 << ".txt" << endl;
        }
        if (kmpSearch(doc[i],input)) {
            cout << "d" << i+1 << ".txt" << endl;
        }
        if (boyer_Moore_Search(doc[i],input)) {
            cout << "d" << i+1 << ".txt" << endl;
        }
        if (rabin_Karp_Search(doc[i],input)) {
            cout << "d" << i+1 << ".txt" << endl;
        }
    }
    
    return 0;
}