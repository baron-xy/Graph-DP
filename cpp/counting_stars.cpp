#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
// 该包内包含laplace随机数生成算法
#include "include/stats.hpp"
// 产生随机数种子
stats::rand_engine_t engine((unsigned)time(0));
using namespace std;
double eps1 = 1; // 第一阶段的隐私预算
double eps2 = 1; // 第二阶段的隐私预算

// 给出拉普拉斯随机数
double Laplace_noisy(double GS, double eps)
{
    return stats::rlaplace(0.0, GS / eps, engine);
};

// 计算组合数C(n,m)
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
    // 定义声明一个ifstream对象，指定文件路径
    ifstream fp(Path);
    string line;
    while (getline(fp, line))
    { // 循环读取每行数据
        vector<int> data_line;
        string number;
        istringstream readstr(line); // string数据流化
        // 将一行数据按'，'分割
        for (int j = 0; j < 2; j++)
        {                                              // 可根据数据的实际情况取循环获取
            getline(readstr, number, ',');             // 循环读取数据
            data_line.push_back(atoi(number.c_str())); // 字符串传int
        }
        Deg.push_back(data_line); // 插入到vector中
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

    // 给输入的参数赋值
    int k = atoi(argv[1]);
    string DegPath = argv[2];
    DegPath += "/data/deg.csv";

    Data data;
    data.readFile(DegPath);
    cout << "the length of dataset:\t" << data.Deg.size() << endl;

    //******dmax的计算与展示
    data.getDmax("getdmax");
    cout << "the max degree is:\t" << data.dmax << endl;

    //*****计算全局敏感度
    data.getGS(k);
    cout << "the golbel sensivity is: \t" << data.GS << endl;

    //*****用户利用拉普拉斯扰动数据并上传,服务器计算
    data.counting(k);
    // cout<<"the number of "<<k<<"-star :\t"<<data.sum<<endl;

    //*****验证实验结果
    data.eval_exp(k);
}