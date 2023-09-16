#include <sstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

queue<string> tokenize(const string& str) {
    stringstream ss(str);
    string token;
    queue<string> tokens;

    while (getline(ss, token, '/')) {
        tokens.push(token);
    }

    return tokens;
}

string cd(const string& cwd, const string& arg) {
    if (arg.empty()) return cwd; // just cd will return pwd, instead of ~

    string tcwd = cwd;
    auto tokens = tokenize(arg);
    while (!tokens.empty()) {
        auto token = tokens.front();
        tokens.pop();

        if (token == "") { //empty occurs when arg starts with '/'
            tcwd.clear();
        } else if (token == ".") {
            continue;
        } else if (token == "..") {
            tcwd = tcwd.substr(0, tcwd.find_last_of('/'));
        } else { 
            tcwd.push_back('/');
            tcwd.append(token);
        }
    }

    return tcwd;
}


int main(int c, char** v) {
    // vector<string> tests{"", "/", ".", "..", "/./a/", "a/b/../c/."};

    // for (auto &&test : tests) {
    //     cout << test << "-->" << cd("/", test) << endl; 
    // }

    if (c < 3) return -1;

    string cwd{v[1]};
    string arg{v[2]};

    cout << cd(cwd, arg) << endl;

    return 0;
}