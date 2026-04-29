#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

int main()
{
    ifstream DataRead("read.txt");
    if (!(DataRead.is_open())) {
        return 1;
    }
    int n;
    DataRead >> n;
    string* name = new string[n];
    for (int i = 0; i < n; i++) {
        DataRead >> name[i];
    }
    DataRead.ignore();
    string line;
    float* expenses = new float[n]();
    float currsum;
    float* norm = new float[n]();
    float* balance = new float[n]();
    while (getline(DataRead, line)) {
        if (line.empty()) {
            continue;
        }
        stringstream currline(line);

        string currname;
        currline >> currname >> currsum;
        for (int i = 0; i < n; i++) {
            if (name[i] == currname) {
                expenses[i] += currsum; // считаю кто скок по факту потратил
            }
        }
        char slash;
        if (currline >> slash and slash == '/') {
            int k = n;
            string exclname;
            getline(currline, exclname);
            stringstream names(exclname);
            string fname;
            string* noname = new string[n];
            for (int i = 0; i < n; i++) {
                noname[i] = "0";
            }
            while (getline(names, fname, ',')) {
                size_t start = fname.find_first_not_of(" ");
                if (start != string::npos) {
                    fname = fname.substr(start);
                }
                size_t end = fname.find_last_not_of(" ");
                if (end != string::npos) {
                    fname = fname.substr(0, end + 1);
                }
                for (int i = 0; i < n; i++) {
                    if (name[i] == fname and noname[i] == "0") {
                        k--;
                        noname[i] = fname;
                        break;
                    }
                }
            }
            float divsum = currsum / k;
            for (int i = 0; i < n; i++) {
                if (noname[i] == "0") {
                    norm[i] += divsum;
                }
            }

            delete[] noname;
        }
        else{
            float divsum = currsum / n;
            for (int i = 0; i < n; i++) {
                norm[i] += divsum;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        balance[i] = expenses[i] - norm[i];
    }
    ofstream DataWrite("write.txt");
    if (!(DataWrite.is_open())) {
        return 1;
    }
    DataWrite << fixed << setprecision(1);
    for (int i = 0; i < n; i++) {
        DataWrite << name[i] << " " << expenses[i] << " " << norm[i] << endl;
    }
    for (int i = 0; i < n; i++) {
        if (balance[i] < -0.01) {
            for (int j = 0; j < n; j++) {
                if (balance[j] > 0.01) {
                    float tranz = min(-balance[i], balance[j]);
                    DataWrite << name[i] << " " << tranz << " " << name[j] << endl;
                    balance[i] += tranz;
                    balance[j] -= tranz;
                    if (balance[i] > -0.01) {
                        break;
                    }
                }
            }
        }
    }
    delete[] name;
    delete[] expenses;
    delete[] norm;
    delete[] balance;
    DataRead.close();
    DataWrite.close();
    return 0;
}