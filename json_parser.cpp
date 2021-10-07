#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include <stack>
#include <vector>
#include <sstream>
using namespace std;

vector<string> _txt(string s) {
    vector<string> arr;
    istringstream delim(s);
    string token;
    while (getline(delim, token , '>' ))
        arr.push_back(token);
    return  arr;
}

vector<string> seperate_str(string s) {
    vector<string> arr;
    for (int i = 0; i < s.length(); i++) {
        for (; i < s.length(); i++) {
            if (s[i] == '"') {
                string tmp;
                for (i++; s[i] != '"'; i++)
                    tmp = tmp + s[i];
                arr.push_back(tmp);
            }
            else if (s[i] == 'n') {
                string tmp = "null";
                arr.push_back(tmp);
            }
            else if (s[i] == 't') {
                string tmp = "true";
                arr.push_back(tmp);
            }
            else if (s[i] == 'f') {
                string tmp = "false";
                arr.push_back(tmp);
            }
            else if (s[i] - '0' >= 0 && s[i] - '0' <= 9) {
                string tmp;
                for (; s[i] - '0' >= 0 && s[i] - '0' <= 9 || s[i]=='.'; i++)
                    tmp = tmp + s[i];
                arr.push_back(tmp);
                arr.push_back(",");
            }
            else if (s[i] == ':') {
                string tmp = ":";
                arr.push_back(tmp);
            }
            else if (s[i] == '{') {
                string tmp = "{";
                arr.push_back(tmp);
            }
            else if (s[i] == '}') {
                string tmp = "}";
                arr.push_back(",");
                arr.push_back(tmp);
            }
            else if (s[i] == ',') {
                string tmp = ",";
                arr.push_back(tmp);
            }
            else if (s[i] == '[') {
                string tmp = "[";
                arr.push_back(tmp);
            }
            else if (s[i] == ']') {
                string tmp = "]";
                arr.push_back(",");
                arr.push_back(tmp);
            }
        }
    }
    return  arr;
}

vector<vector<string>> parser(vector<string> s)
{
    vector<vector<string>> arr;
    vector<string>element;
    int highest_level = 0 , tmp = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == "{"||s[i]=="[")tmp++;
        if (s[i] == "}"||s[i]=="]")tmp--; 
        if (highest_level < tmp)highest_level = tmp;
    }

    vector<string> substitute;
    substitute.resize(highest_level+10000);
    for (int i = 0; i < substitute.size(); i++) {
        substitute[i] == "0";
    }
    int level_now = 0;
    bool pure_array = true;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == "," && s[i + 1] == ",")i += 1;
        if (s[i] == "[") { // 判斷是否為單純的array(裡面只有數字)
            int on = 0;
            for (int j = i + 1; s[j] != "]"; j++) {
                if (s[j] == "{") {
                    pure_array = false;
                    on = 1;
                }
            }
            if(on==0)pure_array = true;
        }
        if (s[i] == "[" && pure_array) { 
            element.insert(element.end(), "is_pure");
            int move = 0;
            for (int j = i + 1; s[j] != "]"; j++) {
                element.push_back(s[j]);
                move++;
            }
            arr.push_back(element);
            i += move;
            element.clear();
            substitute[level_now] = s[i + 1];
            for (int j = 1; j <= level_now; j++) {
                element.push_back(substitute[j]);
            }
        }
        if (s[i] == "[" && !pure_array) {  substitute[++level_now] = "["; }
        if (s[i] == "]" && !pure_array) level_now--; 
        if (s[i] == "{" ) {
            level_now++;
            substitute[level_now] = s[i + 1];
            element.push_back(substitute[level_now]);
            i += 2;
        }
        if (s[i] == "}" ) level_now--;
        if (s[i] != ",")  element.push_back(s[i]);
        if (s[i] == "," ) {
            arr.push_back(element);
            element.clear();
            substitute[level_now] = s[i + 1];
            for (int j = 1; j < level_now; j++) {
                element.push_back(substitute[j]);
            }
        }
    }
    return  arr;
}

string become_legal(string &str)
{
    stack<char> s;
    char x;
    string newstr="";
    int count1 = 0, count2 = 0;

    for (int i = 0; i < str.length(); i++)
    {
        newstr = newstr + str[i];
        if (count2 > count1)return newstr;
        if (str[i] == '{')count1++;
        if (str[i] == '}')count2++;
        if (count1 + count2 != 0 && count1 == count2)return newstr;
    }
    return newstr;
}

int main(int argc, char* argv[])
{
    ifstream fin1, fin2;
    //fin2.open(argv[1]);
    //fin1.open(argv[2]);

    fin1.open("1.txt", ios::in);
    fin2.open("1.json", ios::in);

    stringstream strStream;
    strStream << fin2.rdbuf();
    string str = strStream.str();
    str = become_legal(str);

    if (!fin1 || !fin2)
    {
        cout << "開啟檔案失敗！" << endl;
        exit(1);
    }
    vector<string> allobject;
    allobject = seperate_str(str);
    vector<vector<string>> messy_data, messy_data2;
    messy_data = parser(allobject);
    vector<string> messy2;

    for (int i = 0; i < messy_data.size(); i++) {
        for (int j = 0; j < messy_data[i].size(); j++) {
            if (messy_data[i][messy_data[i].size() - 1] != "]"
                && messy_data[i][messy_data[i].size() - 1] != "}") {
                messy2.push_back(messy_data[i][j]);
            }
        }
        if (messy2.size() > 1)messy_data2.push_back(messy2);
        messy2.clear();
    }
    vector<string> clean1;
    vector<vector<string>> clean_data;
    for (int i = 0; i < messy_data2.size(); i++) {
        for (int j = 0; j < messy_data2[i].size(); j++) {
            if (messy_data2[i][j] != ":" &&
                messy_data2[i][j] != "[" &&
                messy_data2[i][j] != "]" &&
                messy_data2[i][j] != "{" &&
                messy_data2[i][j] != "}" &&
                messy_data2[i][j] != ",") {
                clean1.push_back(messy_data2[i][j]);
            }
        }
        if (clean1.size() > 1)clean_data.push_back(clean1);
        clean1.clear();
    }
    for (int i = 0; i < clean_data.size(); i++) {
        for (int j = 0; j < clean_data[i].size(); j++) {
            //cout << clean_data[i][j] << endl;
        }
       // cout << "------size:" << clean_data[i].size() <<"-------"<<endl;
    }

    ifstream fin;
    int line1 = 0;
    string tmps;
    fin.open("1.txt", ios::in);
    //fin.open(argv[2]);
    while (getline(fin, tmps)) {
        line1++;
    }
    fin.close();
    int c = 0;
    string line;
    vector<string> txt_object;
    while(getline(fin1, line)){
    vector<string> a = _txt(line);
        for (int ii = 0; ii < a.size(); ii++) txt_object.push_back(a[ii]);
        int ok = 0;
        int yes = 0;//有找完txt_object
        for (int i = 0; i < clean_data.size(); i++) {
            for (int j = 0; j < clean_data[i].size(); j++) {
                if (clean_data[i][j] == txt_object[0]) {
                    ok++;
                    if (txt_object.size() > 1) {
                        for (int k1 = 1, j1 = j + 1; k1 < txt_object.size() && j1 < clean_data[i].size(); k1++, j1++) {
                            if (clean_data[i][j1] == txt_object[k1])ok++;
                            if (ok == txt_object.size()) {
                                yes = 1;
                                j1++;
                                if (clean_data[i][j1] == "is_pure" && j1 == clean_data[i].size() - 1) { cout << endl; break; }
                                if (clean_data[i][j1] == "is_pure") {
                                    j1++;
                                    while (j1 < clean_data[i].size()) {
                                        cout << clean_data[i][j1] << endl;
                                        j1++;
                                    }
                                }
                                else {
                                    if (clean_data[i].size() - j1 > 1) { yes = 0; break; }
                                    else {
                                        cout << clean_data[i][clean_data[i].size() - 1] << endl;
                                    }
                                }

                                break;
                            }
                        }
                    }
                    else if (txt_object.size() == 1 && j == clean_data[i].size() - 1) { break; }
                    else if (txt_object.size() == 1) {
                        yes = 1;
                        int j1 = j + 1;
                        if (clean_data[i][j1] == "****")txt_object[2];
                        if (clean_data[i][j1] == "is_pure" && j1 == clean_data[i].size() - 1) { cout << endl; break; }
                        if (clean_data[i][j1] == "is_pure") {
                            j1++;
                            while (j1 < clean_data[i].size()) {
                                cout << clean_data[i][j1] << endl;
                                j1++;
                            }
                        }
                        else {
                            if (clean_data[i].size() - j1 > 1) { yes = 0; break; }
                            else {
                                cout << clean_data[i][clean_data[i].size() - 1] << endl;
                            }
                        }
                        break;
                    }
                }
                ok = 0;
            }   
        }
        txt_object.clear();
        c++;
        if (yes == 0 && c == line1)  cout <<endl;
        else if (yes == 0) cout  <<endl << endl;
        else if (c < line1 && yes == 1) cout  <<endl;
    }
}
