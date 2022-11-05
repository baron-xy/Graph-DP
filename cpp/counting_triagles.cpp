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

// 该包内包含laplace随机数生成算法
#include "include/stats.hpp"
// 产生随机数种子
stats::rand_engine_t engine((unsigned)time(0));
random_device rd;
mt19937 gen(rd());

double eps = 1; // 隐私预算

// 产生0至1的随机数
uniform_real_distribution<> dis(0, 1);

// 给出拉普拉斯随机数
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
    // 定义声明一个ifstream对象，指定文件路径
    ifstream fp(EdgePath);
    string line;

    int i = 0;
    int node1;
    int node2;
    vector<int> data_line;

    while (getline(fp, line))
    { // 循环读取每行数据
        string number;
        istringstream readstr(line); // string数据流化
        // 读取第一个节点
        getline(readstr, number, ',');
        node1 = atoi(number.c_str());
        // 读取第二个节点
        getline(readstr, number, ',');
        node2 = atoi(number.c_str());

        if (node1 == i)
        {
            data_line.push_back(node2);
        }
        else
        {
            // 将i对应的边s填入Edges中
            Edges[i] = data_line;
            data_line.clear();

            // 将数据填入下一行中
            data_line.push_back(node2);
            i = node1;
        }
    }
    // 将最后提取到的数据填入表中
    Edges[i] = data_line;

    // 将数据填入下一行中
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
    // 输出到log文本中
    ofstream fout(logData, ios::app);
    fout << "本次统计出三角形的总数量为：" << round(sum * (1 + exp(eps)) / (exp(eps) - 1)) << endl;
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
    //*******读取数据
    data.readFile(EdgePath);
    cout  << data.maxnode << " items were loaded,start random response..." << endl;

    // //*******随机扰动
    data.RR_perturb();
    cout << "Random response is compelete,start writing data..." << endl;

    // // //*******扰动数据写入csv(一般不需要)
    // // data.wirte_noisy();

    // //****开始进行三角形计数
    cout << "Start counting..." << endl;
    data.counting(logData);
}