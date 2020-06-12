#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

#define k 19
#define dimension 41

//Acurracy: 77.556%

class knn
{
public:
    knn() {}
    ~knn() {}

    void init_mean() {}
    void read_data();
    double distance(double *, double *);
    void test();

    void train()
    {
        read_data();
        test();
    }

private:
    vector<double *> data;
    vector<int> answer;
    double mean[dimension];
};

static bool compare(pair<double, int> a, pair<double, int> b)
{
    return a.first < b.first;
}

int main()
{
    knn instance;
    instance.train();

    return 0;
}

void knn::test()
{
    ifstream file("NSL-KDDTest+.txt");
    ofstream f("output.txt");
    vector<pair<double, int>> tmp; //distance, answer
    int count = 0;
    while (!file.eof())
    {
        count++;
        double test_data[dimension];
        for (int i = 0; i < dimension - 1; i++)
        {
            file >> test_data[i];
            file.ignore();
        }
        file >> test_data[40];

        for (long i = 0; i < data.size(); i++)
            tmp.push_back(make_pair(distance(test_data, data[i]), answer[i]));

        sort(tmp.begin(), tmp.end(), compare);

        int attack = 0, normal = 0;
        for (int j = 0; j < k; j++)
            for (int i = 0; i < dimension; i++)
                tmp[j].second == 0 ? normal++ : attack++;

        normal > attack ? f << 0 : f << 1;
        tmp.clear();
        cout << count << endl;
    }
    file.close();

    f.close();
    cout << "test done" << endl;
}

double knn::distance(double *x, double *y)
{
    //Euclidean distance
    double sum = 0;

    for (int i = 0; i < dimension; i++)
        sum += pow((x[i] - y[i]), 2);
    return sqrt(sum);
}

void knn::read_data()
{
    ifstream file("NSL-KDDTrain+.txt");
    while (!file.eof())
    {
        double *tmp = new double[dimension];
        int ans;
        for (int i = 0; i < dimension; i++)
        {
            file >> tmp[i];
            file.ignore();
        }
        file >> ans;
        data.push_back(tmp);
        answer.push_back(ans);
    }
    file.close();
    cout << "read training data done" << endl;
}
/*
void knn::init_mean()
{

    mean[0] = 287.145;
    mean[1] = 1.0532;
    mean[2] = 31.2265;
    mean[3] = 6.98;
    mean[4] = 45566.7;
    mean[5] = 19779.1;
    mean[6] = 0.000198455;
    mean[7] = 0.0226874;
    mean[8] = 0.000111135;
    mean[9] = 0.204409;
    mean[10] = 0.00122248;
    mean[11] = 0.395736;
    mean[12] = 0.27925;
    mean[13] = 0.00134156;
    mean[14] = 0.00110341;
    mean[15] = 0.302192;
    mean[16] = 0.0126694;
    mean[17] = 0.000412787;
    mean[18] = 0.00409612;
    mean[19] = 0;
    mean[20] = 7.93821e-06;
    mean[21] = 0.00942265;
    mean[22] = 84.1076;
    mean[23] = 27.7379;
    mean[24] = 0.284485;
    mean[25] = 0.282485;
    mean[26] = 0.119958;
    mean[27] = 0.121183;
    mean[28] = 0.660928;
    mean[29] = 0.0630526;
    mean[30] = 0.0973216;
    mean[31] = 182.149;
    mean[32] = 115.653;
    mean[33] = 0.521242;
    mean[34] = 0.0829511;
    mean[35] = 0.148379;
    mean[36] = 0.0325424;
    mean[37] = 0.284452;
    mean[38] = 0.278485;
    mean[39] = 0.118832;
    mean[40] = 0.12024;
    cout << "init_mean done" << endl;
}*/