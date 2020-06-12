#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
//was max extend to mean
int main()
{
    double feature[41];
    double temp;
    int result;
    char comma;

    fstream file;
    file.open("NSL-KDDTrain+.txt", ios::in);

    for (int i = 0; i < 41; i++)
        file >> feature[i] >> comma;
    file >> result;

    int count = 1;
    int hi;
    while (true)
    {
        count ++;
        for (int i = 0; i < 41; i++)
        {
            double tmp;
            file >> tmp >> comma;
            feature[i] += tmp;
        }
        file >>temp;

        if(file.eof())
            break;
    }
    /*    while (!file.eof())
    {
        count++;
        for (int i = 0; i < 41; i++)
        {
            file >> temp;
            if (temp > feature[i])
            {
                feature[i] = temp;
                hi = count;
            }
            file >> comma;
        }
        file >> result;
    }*/
    file.close();
    for (int i = 0; i < 41; i++)
        cout << feature[i] / count << endl;
    cout << endl;

    //
    //****************
    //NUM OD TRAINING DATE
    cout<<count;
    //
    //****************
    //    cout << RAND_MAX << endl;
    //    cout<<endl<<hi<<endl;
}