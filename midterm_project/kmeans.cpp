#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

class data
{
public:
    data(int num = 1)
    {
        coor = new double[num];
        dim = num;
    };
    double *coor;
    int group;
    double distance;
    int dim;
    ~data(){};
};

class km
{
public:
    km(string name = "", int qua = 0, int dim = 0, int run = 0, int iter = 0, int num_c = 0)
    {
        filename = name;
        quantity = qua;
        dimension = dim;
        runs = run;
        iteration = iter;
        num_centroid = num_c;
        map_centroid = new vector<data>[num_c];
        centroids = new double *[num_c];
        sse = new double[iter];
        for (int i = 0; i < num_c; i++)
            centroids[i] = new double[dimension];
        for (int i = 0; i < iter; i++)
            sse[i] = 0.0;
    }
    void start();
    ~km(){};

protected:
    void info();
    void initialize();
    void read();
    void check_zero();
    void update();
    void assign();
    void calculate_sse(int);
    int which_group(int i);
    void show_result(int);
    data *inputs;
    string filename;
    int quantity;
    int dimension;
    int runs;
    int iteration;
    int num_centroid;
    vector<data> *map_centroid;
    double **centroids;
    double *sse;
    double clk;
};
bool compare(data &a, data &b)
{
    double na = 0, nb = 0;
    int size = a.dim;
    for (int i = 0; i < size; i++)
    {
        na += a.coor[i];
        nb += b.coor[i];
    }
    return na > nb;
}
class kmedoids : public km
{
public:
    kmedoids(string name = "", int qua = 0, int dim = 0, int run = 0, int iter = 0, int num_c = 0) : km(name, qua, dim, run, iter, num_c){};
    friend bool compare(data &, data &);
    void start()
    {
        kmedoids::info();
        if (quantity < 1)
            return;
        for (int i = 0; i < runs; i++)
        {
            clock_t t = clock();
            kmedoids::initialize();
            for (int j = 0; j < iteration; j++)
            {
                kmedoids::assign();
                kmedoids::update();
                calculate_sse(j);
            }
            clk = double(clock() - t);
            cout << endl;
            kmedoids::show_result(i);
            for (int j = 0; j < iteration; j++)
                sse[j] = 0.0;
            for (int j = 0; j < num_centroid; j++)
                map_centroid[j].clear();
        }
    }

protected:
    void info()
    {
        cout << "algorithm  :  k-medoids\n"
             << "Dateset  : " << filename << endl
             << "quantity : " << quantity << endl
             << "dimension: " << dimension << endl
             << "run times: " << runs << endl
             << "iteration of k-means : " << iteration << endl
             << "centroid numbers : " << num_centroid << endl
             << endl;
    }
    void update()
    {
        for (int i = 0; i < num_centroid; i++)
            for (int j = 0; j < dimension; j++)
                centroids[i][j] = 0.0;

        check_zero();

        for (int i = 0; i < num_centroid; i++)
        {
            int size = map_centroid[i].size();

            if (size % 2 || size < 3)
                for (int j = 0; j < dimension; j++)
                    centroids[i][j] = map_centroid[i][size / 2].coor[j];
            else
                for (int j = 0; j < dimension; j++)
                    centroids[i][j] = (map_centroid[i][size / 2].coor[j] + map_centroid[i][size / 2 - 1].coor[j]) / 2;
        }
    }
    void assign()
    {
        for (int i = 0; i < num_centroid; i++)
            map_centroid[i].clear();
        for (int i = 0; i < quantity; i++)
        {
            int new_group = which_group(i);
            map_centroid[new_group].push_back(inputs[i]);
        }

        for (int i = 0; i < num_centroid; i++)
            sort(map_centroid[i].begin(), map_centroid[i].end(), compare);
    }
    void show_result(int r)
    {
        cout << "--------------------------------------------------------------------" << endl
             << "The result is the " << r + 1 << "th times run k-medoids" << endl;
        cout << endl
             << "Execute duration " << clk / CLOCKS_PER_SEC << endl;

        cout << "SSE (sum of square error) " << sse[iteration - 1] << endl;
        for (int i = 0; i < num_centroid; i++)
        {
            cout << " " << map_centroid[i].size() << " in ";
            cout << "group " << i;
            for (int m = 0; m < dimension; m++)
                cout << " " << setprecision(6) << centroids[i][m];
            cout << endl;
        }

        cout << endl;
        for (int i = 0; i < quantity; i++) //data belong to which group
            cout << inputs[i].group << " ";

        cout << endl
             << "--------------------------------------------------------------------" << endl;
    }
};

void km::start()
{
    info();

    if (quantity < 1)
        return;
    for (int i = 0; i < runs; i++)
    {
        clock_t t = clock();
        initialize();
        for (int j = 0; j < iteration; j++)
        {
            assign();
            update();
            calculate_sse(j);
        }
        clk = double(clock() - t);

        cout << endl;
        show_result(i);
        for (int j = 0; j < iteration; j++)
            sse[j] = 0.0;
        for (int j = 0; j < num_centroid; j++)
            map_centroid[j].clear();
    }
}

void km::info()
{
    cout << "algorithm  :  k-means\n"
         << "Dateset  : " << filename << endl
         << "quantity : " << quantity << endl
         << "dimension: " << dimension << endl
         << "run times: " << runs << endl
         << "iteration of k-means : " << iteration << endl
         << "centroid numbers : " << num_centroid << endl
         << endl;
}

void km::initialize()
{
    read();
    update();
}

void km::read()
{
    ifstream file(filename.c_str());
    inputs = new data[quantity];
    int tmp;
    for (data *i = inputs; i != inputs + quantity; i++)
    {
        *i = data(dimension);
    }
    for (int i = 0; i < quantity; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            char junk = file.peek();
            if (!isdigit(junk) || junk == '\n')
            {
                j--;
                file.ignore();
                continue;
            }
            else
            {
                file >> inputs[i].coor[j];
            }
        }
        tmp = rand() % num_centroid;
        inputs[i].group = tmp;
        map_centroid[tmp].push_back(inputs[i]);
    }
    file.close();
}

void km::check_zero()
{
    for (int i = 0; i < num_centroid; i++)
    {
        if (!map_centroid[i].size())
        {
            int t = rand() % quantity;
            map_centroid[i].push_back(inputs[t]);
            inputs[t].group = i;
        }
    }
}

void km::update()
{
    for (int i = 0; i < num_centroid; i++)
        for (int j = 0; j < dimension; j++)
            centroids[i][j] = 0.0;
    check_zero();
    for (int i = 0; i < num_centroid; i++)
    {
        int size = map_centroid[i].size();
        for (vector<data>::iterator j = map_centroid[i].begin(); j != map_centroid[i].end(); j++)
        {
            for (int k = 0; k < dimension; k++)
            {
                centroids[i][k] += j->coor[k];
            }
        }
        for (int j = 0; j < dimension; j++)
        {
            centroids[i][j] /= size;
        }
    }
}

void km::assign()
{
    for (int i = 0; i < num_centroid; i++)
        map_centroid[i].clear();
    for (int i = 0; i < quantity; i++)
    {
        int new_group = which_group(i);
        map_centroid[new_group].push_back(inputs[i]);
    }
}

void km ::calculate_sse(int n)
{
    double x = 0;
    for (int i = 0; i < quantity; i++)
    {
        double dis;
        for (int j = 0; j < num_centroid; j++)
        {
            double tmp = 0;
            for (int k = 0; k < dimension; k++)
                tmp += pow(inputs[i].coor[k] - centroids[j][k], 2);

            if (!j || tmp < dis)
                dis = tmp;
        }
        x += dis;
    }
    sse[n] += x;
}

int km::which_group(int i)
{
    int grp;
    double dis;
    for (int j = 0; j < num_centroid; j++)
    {
        double tmp = 0;
        for (int k = 0; k < dimension; k++)
        {
            tmp += pow(inputs[i].coor[k] - centroids[j][k], 2);
        }
        tmp = sqrt(tmp);
        if (!j || tmp < dis)
        {
            dis = tmp;
            grp = j;
        }
    }
    inputs[i].distance = dis;
    inputs[i].group = grp;
    return grp;
}

void km::show_result(int r)
{
    cout << "--------------------------------------------------------------------" << endl
         << "The result is the " << r + 1 << "th times run k-means" << endl;
    cout << endl
         << "Execute duration " << clk / CLOCKS_PER_SEC << endl;

    cout << "SSE (sum of square error) " << sse[iteration - 1] << endl;
    for (int i = 0; i < num_centroid; i++)
    {
        cout << " " << map_centroid[i].size() << " in ";
        cout << "group " << i;
        for (int m = 0; m < dimension; m++)
            cout << " " << setprecision(6) << centroids[i][m];
        cout << endl;
    }

    cout << endl;
    for (int i = 0; i < quantity; i++) //data belong to which group
        cout << inputs[i].group << " ";

    cout << endl
         << "--------------------------------------------------------------------" << endl;
}
/*
    1.method
    2.name
    3.quantity
    4.dimension
    5.run times
    6.iteration times
    7.number of centroid
*/
int main(int argc, char **argv)
{
    srand(time(NULL));
    if (!strcmp(argv[1], "km"))
    {
        km k(
            argv[2],
            strtol(argv[3], NULL, 10),
            strtol(argv[4], NULL, 10),
            strtol(argv[5], NULL, 10),
            strtol(argv[6], NULL, 10),
            strtol(argv[7], NULL, 10));
        k.start();
    }
    else if (!strcmp(argv[1], "kmd"))
    {
        kmedoids k(
            argv[2],
            strtol(argv[3], NULL, 10),
            strtol(argv[4], NULL, 10),
            strtol(argv[5], NULL, 10),
            strtol(argv[6], NULL, 10),
            strtol(argv[7], NULL, 10));
        k.start();
    }

    return 0;
}