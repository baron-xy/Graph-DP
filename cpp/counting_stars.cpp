#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
// �ð��ڰ���laplace����������㷨
#include "include/stats.hpp"
// �������������
stats::rand_engine_t engine((unsigned)time(0));
using namespace std;
double eps1 = 1; // ��һ�׶ε���˽Ԥ��
double eps2 = 1; // �ڶ��׶ε���˽Ԥ��

// ����������˹�����
double Laplace_noisy(double GS, double eps)
{
    return stats::rlaplace(0.0, GS / eps, engine);
};

// ���������C(n,m)
long long C(int n, int m)
{
    if (m < n - m)
        m = n - m;
    long long ans = 1;
    for (int i = m + 1; i <= n; i++)
        ans *= i;
    for (int i = 1; i <= n - m; i++)
        ans /= i;
    return ans;
};

class Data
{
public:
    vector<vector<int>> Deg;
    double dmax;
    long long GS;
    long long sum = 0;
    void readFile(string Path);
    void getDmax(string str);
    void getGS(int k);
    void counting(int k);
    void eval_exp(int k);
};

void Data::readFile(string Path)
{
    // ��������һ��ifstream����ָ���ļ�·��
    ifstream fp(Path);
    string line;
    while (getline(fp, line))
    { // ѭ����ȡÿ������
        vector<int> data_line;
        string number;
        istringstream readstr(line); // string��������
        // ��һ�����ݰ�'��'�ָ�
        for (int j = 0; j < 2; j++)
        {                                              // �ɸ������ݵ�ʵ�����ȡѭ����ȡ
            getline(readstr, number, ',');             // ѭ����ȡ����
            data_line.push_back(atoi(number.c_str())); // �ַ�����int
        }
        Deg.push_back(data_line); // ���뵽vector��
    }
}

void Data::getDmax(string str)
{
    if (str == "default")
    {
        dmax = 500;
        return;
    }
    double dmaxfind = 0;
    double temp = 0;
    for (int i = 0; i < Deg.size(); i++)
    {
        temp = double(Deg[i][1]) + Laplace_noisy(1.0, eps1);
        if (dmaxfind < temp)
        {
            dmaxfind = temp;
        };
    }
    dmax = dmaxfind;
};

void Data::getGS(int k)
{
    GS = C(dmax, k - 1);
};

void Data::counting(int k)
{
    for (int i = 0; i < Deg.size(); i++)
    {
        sum += C(Deg[i][1], k) + Laplace_noisy(GS, eps2);
    }
};

void Data::eval_exp(int k)
{
    long long truth = 0;
    for (int i = 0; i < Deg.size(); i++)
    {
        truth += C(Deg[i][1], k);
    }
    cout << "the ture num is: " << truth << "\nthe counting num is " << sum << endl;
    double relative_error = abs(truth - sum);
    cout << "the  relative error is " << relative_error << endl;
};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Please input counting_striagles.exe [k] [DegPath]" << endl;
        return -1;
    }

    // ������Ĳ�����ֵ
    int k = atoi(argv[1]);
    string DegPath = argv[2];
    DegPath += "/data/deg.csv";

    Data data;
    data.readFile(DegPath);
    cout << "the length of dataset:\t" << data.Deg.size() << endl;

    //******dmax�ļ�����չʾ
    data.getDmax("getdmax");
    cout << "the max degree is:\t" << data.dmax << endl;

    //*****����ȫ�����ж�
    data.getGS(k);
    cout << "the golbel sensivity is: \t" << data.GS << endl;

    //*****�û�����������˹�Ŷ����ݲ��ϴ�,����������
    data.counting(k);
    // cout<<"the number of "<<k<<"-star :\t"<<data.sum<<endl;

    //*****��֤ʵ����
    data.eval_exp(k);
}