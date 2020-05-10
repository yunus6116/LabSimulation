#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    ifstream file("input.txt");
    string str;
    while (getline(file, str)) {
        cout << str << "\n";
    }




    ofstream outfile("output.txt");
    ofstream outfile("turns.txt");
    outfile << "my text here!" <<endl;

    outfile.close();
}