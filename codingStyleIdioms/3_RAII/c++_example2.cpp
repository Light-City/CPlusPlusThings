//
// Created by light on 19-12-12.
//
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <bits/unique_ptr.h>

using namespace std;
unique_ptr<vector<string>> read_lines_from_file(string &file_name) {
    unique_ptr<vector<string>> lines(new vector<string>);
    string line;

    ifstream file_handle (file_name.c_str());
    while (file_handle.good() && !file_handle.eof()) {
        getline(file_handle, line);
        lines->push_back(line);
    }

    file_handle.close();

    return lines;
}
int main() {
    // get file name from the first argument
    string file_name ("/home/light/CLionProjects/Morden_C++/CMakeLists.txt");
    int count = read_lines_from_file(file_name).get()->size();
    cout << "File " << file_name << " contains " << count << " lines.";
    cout<<endl;
    return 0;
}