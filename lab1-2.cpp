#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>

using namespace std;

const int maxn = 100;

string &replace_all(string &str, const string &old_value, const string &new_value)
{
    while (true)
    {
        string::size_type pos(0);
        if ((pos = str.find(old_value)) != string::npos)
            str.replace(pos, old_value.length(), new_value);
        else
            break;
    }
    return str;
}

int main()
{
    int total_num = 0;          // "keyword" statistics
    int switch_num = 0;         // number of "switch case" structures
    int case_nums[10] = {};     // number of "case" corresponding to each group
    int if_else_num = 0;        // number of "if else" structures
    int if_elseif_else_num = 0; // number of "if, else if, else" structures
    int level;
    stack<int> if_else_container;
    queue<int> switch_case_container;
    string codes;
    string filename;
    ifstream inFile;

    string keyword[32] = {"void", "char", "int", "float", "double", "short", "long", "signed", "unsigned",
                          "struct", "union", "enum", "typedef", "sizeof", "auto", "static", "register", "extern", "const", "volatile",
                          "return", "continue", "break", "goto", "if", "else", "switch", "case", "default", "for", "do", "while"};

    cout << "Type in the file name: ";
    cin >> filename;
    cout << "Type in the requirement(1,2,3,4): ";
    cin >> level;

    inFile.open(filename.c_str());
    if (inFile.fail())
    {
        cout << "\nThe file was not successfully opened."
             << "\nPlease check that the file currently exists."
             << endl;
        exit(1);
    }

    // Basic Requirement
    
    while (getline(inFile, codes))
    {

        if (codes.rfind("switch") < maxn)
            switch_case_container.push(0);
        else if (codes.rfind("case") < maxn)
            switch_case_container.push(1);

        if (codes.rfind("else if") < maxn)
            if_else_container.push(1);
        else if (codes.rfind("else") < maxn)
            if_else_container.push(2);
        else if (codes.rfind("if") < maxn)
            if_else_container.push(0);

        replace_all(codes, "(", " ");
        replace_all(codes, ")", " ");
        replace_all(codes, "{", " ");
        replace_all(codes, ":", " ");
        replace_all(codes, ";", " ");
        istringstream str(codes);
        string filter;
        string last = "";

        bool pure_else_merge = false;

        while (str >> filter)
        {
            for (int k = 0; k < 32; k++)
            {
                total_num += (filter == keyword[k]);
            }
        }
    }
    cout << "total num: " << total_num << endl;

    // Advanced Requirement
    
    while (!switch_case_container.empty())
    {
        int front = switch_case_container.front();
        switch_case_container.pop();
        if (front == 0)
        {
            while (!switch_case_container.empty() && switch_case_container.front() != 0)
            {
                case_nums[switch_num]++;
                switch_case_container.pop();
            }
            switch_num++;
        }
    }

    if (level >= 2)
    {
        cout << "switch num: " << switch_num << endl;
        cout << "case num:";
        for (int i = 0; i < switch_num; i++)
        {
            cout << case_nums[i] << " ";
        }
        cout << endl;
    }

    // Uplifting Requirement
    
    stack<int> tmp;
    while (!if_else_container.empty())
    {
        int top_element = if_else_container.top();
        if_else_container.pop();
        if (top_element == 2)
        {
            tmp.push(2);
        }
        if (top_element == 1)
        {
            tmp.push(1);
        }
        if (top_element == 0)
        {
            if (tmp.top() == 2)
            {
                if_else_num++;
                tmp.pop();
            }
            else if (tmp.top() == 1)
            {
                if_elseif_else_num++;
                while (tmp.top() != 2)
                {
                    tmp.pop();
                }
                tmp.pop();
            }
        }
    }
    if (level >= 3)
    {
        cout << "if-else num: " << if_else_num << endl;
    }
    if (level >= 4)
    {
        cout << "if-elseif-else num: " << if_elseif_else_num << endl;
    }
}
