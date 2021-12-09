#include<iostream>
#include<bit>
#include<bitset>
#include<Bits.h>
#include<vector>
#include<string>
#include<fstream>
#include <assert.h>
#include <cstring>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
//
vector<int> encoding(string s1, const char* argv[])
{
    cout << "Encoding\n";
    unordered_map<string, int> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }

    string p = "", c = "";
    p += s1[0];
    int code = 256;
    vector<int> output_code;
    if ((strcmp(argv[4], "-d") == 0) || (strcmp(argv[4], "-ind") == 0))
    {
        cout << "------New Word------" << endl;
    }
    for (int i = 0; i < s1.length(); i++) {
        if (i != s1.length() - 1)
            c += s1[i + 1];
        if (table.find(p + c) != table.end()) {
            p = p + c;
        }
        else {
            if ((strcmp(argv[4], "-d") == 0) || (strcmp(argv[4], "-ind") == 0))
            {
            cout << p + c << ":" << code << endl;
            }
            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
    output_code.push_back(table[p]);
    if ((strcmp(argv[4], "-i") == 0) || (strcmp(argv[4], "-ind") == 0))
    {
        cout << "-------Size of input and output File------" << endl;
        struct stat filestatus1;
        struct stat filestatus2;
        stat(argv[2], &filestatus1);
        stat(argv[3], &filestatus2);
        int s1 = filestatus1.st_size * 8;
        int s2 = output_code.size() * 9;
        cout <<"Input size:"<< s1 << " bits\n";
  
        cout << "Output size:" << s2 << " bits\n";
        cout << "Space saved:" << 100 - (float)s2 / s1 * 100 << "%" << endl;
    }
    return output_code;
}
//
void readFromFile(const char* file, string &str)
{
    string s;
    fstream f;
    f.open(file, ios::in);
    if (!f.is_open())
    {
        cout << "Failed to open file " << endl; assert(false);
    }
    while (!f.eof())
    {
        getline(f, s);
        str += s;
    }
    f.close();
}
//
void writeToFile(const char* file, vector<int> output_code)
{
    string str;
    ofstream f;
    f.open(file, ios::binary);
    if (f.is_open())
    {
        for (int i = 0; i < output_code.size(); i++)
        {
            f.write((char*)&output_code[i], sizeof(output_code[i]));
        }
    }
    f.close();
}
//
void compress(const char*argv[])
{
    string str;

    readFromFile(argv[2], str);
    vector<int> output_code;
    output_code=encoding(str, argv);
    writeToFile(argv[3], output_code);
}
//
void readFromFile1(const char* file, vector<int>& output_code)
{
    int n;
    ifstream f;
    f.open(file, ios::binary);
    if (!f.is_open())
    {
        cout << "Failed to open file " << endl; assert(false);
    }
    while (!f.eof())
    {
        f.read((char*)&n, sizeof(int));
        //cout << n << endl;
        output_code.push_back(n);
    }
    f.close();
}
//
void writeToFile1(const char* file, string str)
{
    fstream f;
    f.open(file, ios::out);
    if (!f.is_open())
    {
        cout << "Failed to open file " << endl; assert(false);
    }
    f << str;
    f.close();
}
//
string decoding(vector<int> op, const char* argv[])
{
    string str;
    cout << "\nDecoding\n";
    unordered_map<int, string> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }
    int old = op[0], n;
    string s = table[old];
    string c = "";
    c += s[0];
    str += s;
    //cout << s;
    int count = 256;
    if ((strcmp(argv[4], "-d") == 0) || (strcmp(argv[4], "-ind") == 0))
    {
        cout << "------New Word------" << endl;
    }
    for (int i = 0; i < op.size() - 1; i++) {
        n = op[i + 1];
        if (table.find(n) == table.end()) {
            s = table[old];
            s = s + c;
        }
        else {
            s = table[n];
        }
        //cout << s;
        str += s;
        c = "";
        c += s[0];
        table[count] = table[old] + c;
        cout << table[count] << ":" << count << endl;
        count++;
        old = n;
    }
    if ((strcmp(argv[4], "-i") == 0) || (strcmp(argv[4], "-ind") == 0))
    {
        cout << "-------Size of input and output File------" << endl;
        struct stat filestatus1;
        struct stat filestatus2;
        stat(argv[3], &filestatus1);
        stat(argv[2], &filestatus2);
        int s1 = filestatus1.st_size * 8;
        int s2 = op.size() * 9;
        cout << "Input size:" << s1 << " bits\n";

        cout << "Output size:" << s2 << " bits\n";
        cout << "Space saved:" << 100 - (float)s2 / s1 * 100 << "%" << endl;
    }
    return str;
}
//
void deCompressed(const char* argv[])
{
    vector<int> output_code;
    readFromFile1(argv[2], output_code);
    vector<int> input_code;
    for (int i = 0; i < output_code.size() - 1; i++) {
        input_code.push_back(output_code[i]);
    }
    for (int i = 0; i < input_code.size(); i++) {
        cout << input_code[i] << endl;
    }
    string str = decoding(input_code,argv);
    writeToFile1(argv[3], str);
    //cout << str << endl;
}
//
int main(int argc, const char* argv[])
{
    if (argc != 5 )
    {
        cout << "Number of arguments is not exactly" << endl;
        return 0;
    }
    if (strcmp(argv[1], "-e") == 0)
    {
        compress(argv);
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        deCompressed(argv);
    }
    return 0;
}