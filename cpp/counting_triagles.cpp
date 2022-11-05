#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <random>
#include <map>
using namespace std;

// �ð��ڰ���laplace����������㷨
#include "include/stats.hpp"
// �������������
stats::rand_engine_t engine((unsigned)time(0));
random_device rd;
mt19937 gen(rd());

double eps = 1; // ��˽Ԥ��

// ����0��1�������
uniform_real_distribution<> dis(0, 1);

// ����������˹�����
double Laplace_noisy(double GS, double eps)
{
    return stats::rlaplace(0.0, GS / eps, engine);
};


class Data
{
public:
    int maxnode;
    map<int, vector<int>> Edges;
    map<int, vector<bool>> noisy_Edges;
    long long sum = 0;
    void readFile(string DegPath);
    void RR_perturb();
    void getDmax(string str);
    void getGS();
    void counting(string logData);
    void eval_exp();
    void wirte_noisy();
};

void Data::readFile(string EdgePath)
{
    // ��������һ��ifstream����ָ���ļ�·��
    ifstream fp(EdgePath);
    string line;

    int i = 0;
    int node1;
    int node2;
    vector<int> data_line;

    while (getline(fp, line))
    { // ѭ����ȡÿ������
        string number;
        istringstream readstr(line); // string��������
        // ��ȡ��һ���ڵ�
        getline(readstr, number, ',');
        node1 = atoi(number.c_str());
        // ��ȡ�ڶ����ڵ�
        getline(readstr, number, ',');
        node2 = atoi(number.c_str());

        if (node1 == i)
        {
            data_line.push_back(node2);
        }
        else
        {
            // ��i��Ӧ�ı�s����Edges��
            Edges[i] = data_line;
            data_line.clear();

            // ������������һ����
            data_line.push_back(node2);
            i = node1;
        }
    }
    // �������ȡ���������������
    Edges[i] = data_line;

    // ������������һ����
    fp.close();
}

void Data::RR_perturb()
{
    double q = 1 / (1 + exp(eps));
    double pro = 0;
    for (int i = 0; i < maxnode; i++)
    {
        vector<bool> noisy_line(maxnode);
        noisy_line.assign(maxnode, 0);
        for (int j = i + 1; j < maxnode; j++)
        {
            pro = dis(gen);
            if (binary_search(Edges[i].begin(), Edges[i].end(), j))
            {
                if (pro > q)
                {
                    noisy_line[j] = true;
                }
            }
            else
            {
                if (pro < q)
                {
                    noisy_line[j] = true;
                }
            }
        }
        if (i % 1000 == 0)
            cout  << (float(i) * 100) / maxnode << "%" << endl;
        noisy_Edges[i] = noisy_line;
    }
}

void Data::counting(string logData)
{
    int count;
    int n;

    for (int i = 0; i < maxnode; i++)
    {
        count = 0;
        n = 0;
        if (i % 1000 == 0)
        {
            cout  << (float(i) * 100) / maxnode << "%" << endl;
        };

        for (int j = 0; j < Edges[i].size(); j++)
        {
            for (int k = j + 1; k < Edges[i].size(); k++)
            {
                if (noisy_Edges[(Edges[i][j])][(Edges[i][k])] == 1)
                {
                    count++;
                }
                n++;
            }
        }
        sum = sum + count - n / (1 + exp(eps));
    }
    cout << "The total number of trangles calculated is:" << round(sum * (1 + exp(eps)) / (exp(eps) - 1)) << endl;
    // �����log�ı���
    ofstream fout(logData, ios::app);
    fout << "����ͳ�Ƴ������ε�������Ϊ��" << round(sum * (1 + exp(eps)) / (exp(eps) - 1)) << endl;
    fout.close();
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Please input couting_triangles.exe [n][Path]" << endl;
    };

    Data data;
    string EdgePath = argv[2];
    EdgePath += "/data/edge.csv";
    string logData = argv[2];
    logData += "/data/log.txt";
    data.maxnode = atoi(argv[1]);
    //*******��ȡ����
    data.readFile(EdgePath);
    cout  << data.maxnode << " items were loaded,start random response..." << endl;

    // //*******����Ŷ�
    data.RR_perturb();
    cout << "Random response is compelete,start writing data..." << endl;

    // // //*******�Ŷ�����д��csv(һ�㲻��Ҫ)
    // // data.wirte_noisy();

    // //****��ʼ���������μ���
    cout << "Start counting..." << endl;
    data.counting(logData);
}