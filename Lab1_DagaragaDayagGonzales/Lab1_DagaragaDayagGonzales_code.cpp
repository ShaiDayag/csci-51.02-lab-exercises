#include <iostream>
#include <string>
#include <cctype>
using namespace std;

string letterShiftString(string str) {
    string newStr;
    for (int i=0; i<str.length(); i++) {
        int strVal = int(char(str[i]));
        if ((strVal >= 65 && strVal <= 90)||(strVal >= 97 && strVal <= 122)) {
            newStr[i] += char(strVal+1);
        }
        else {
            newStr[i] += char(str[i]);
        }
    }
    return newStr;
}

char letterShiftChar(char* ch) {
    if ((int(*ch) >= 65 && int(*ch) <= 90)||(int(*ch) >= 97 && int(*ch) <= 122)) {
        *ch = char(int(*ch)+1);
    }
    else {
        *ch = *ch;
    }
    return *ch;
}

int main(void) {
    string name, newName;
    cout << "What is your name?" << endl;
    cin >> name;
    cout << name << endl;
    newName += name[0];
    for (int i=1; i<name.length(); i++) {
        newName += "#";
    }
    cout << newName << endl;
    // for (int i=0; i<name.length(); i++) {
    //     if (isalpha(name[i]) && ((char(name[i]) >= 65 && char(name[i]) <= 90)||(char(name[i]) >= 97 && char(name[i]) <= 122))) {
    //         newName[i] = char(int(name[i])+1);
    //     }
    //     else {
    //         newName[i] = name[i];
    //     }
    // }
    // cout << newName << endl;
    cout << letterShiftString(name) << endl;
    for (int i = 0; i < name.length(); i++) {
        letterShiftChar(&name[i]);
    }
    cout << name << endl;
    
    return 0;
}
