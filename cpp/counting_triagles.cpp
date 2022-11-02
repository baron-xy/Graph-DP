#include<iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include<vector>
#include <ctime>
#include<cmath>
#include<random>
#include<map>
#define EdgePath "D:/TriangleLD/data/edge.csv"
#define NoisyData "D:/TriangleLD/data/noisy.csv"
using namespace std;
//该包内包含laplace随机数生成算法
#include "include/stats.hpp"
#define maxnode 10000
//产生随机数种子
stats::rand_engine_t engine((unsigned)time(0));
random_device rd;
mt19937 gen(rd());

double eps1=1;   //第一阶段的隐私预算
double eps2=1;   //第二阶段的隐私预算

//产生0至1的随机数
uniform_real_distribution<> dis(0,1);
//给出拉普拉斯随机数
double Laplace_noisy(double GS , double eps){
    return stats::rlaplace(0.0, GS/eps,engine);
};

//计算组合数C(n,m)
long long C(int n, int m)
{
	if (m < n - m) m = n - m;
	long long ans = 1;
	for (int i = m + 1; i <= n; i++) ans *= i;
	for (int i = 1; i <= n - m; i++) ans /= i;
	return ans;
};

class Data{
    public:
        vector<int> Deg;
        map<int,vector<int>> Edges;
        map<int,vector<bool>> noisy_Edges;
    
        double dmax;
        long long GS;
        long long sum=0;
        void readFile(string DegPath);
        void RR_perturb();
        void getDmax(string str);
        void getGS();
        void counting();
        void eval_exp();
        void wirte_noisy();
};

void Data::readFile(string EdgPath){
    //定义声明一个ifstream对象，指定文件路径
    ifstream fp(EdgePath); 
    string line;

    int i=0;
    int node1;
    int node2;
    vector<int> data_line;
    while (getline(fp,line)){ //循环读取每行数据
        string number;
        istringstream readstr(line); //string数据流化
        // 读取第一个节点
        getline(readstr,number,',');
        node1 = atoi(number.c_str());
        //读取第二个节点
        getline(readstr,number,',');
        node2 = atoi(number.c_str());


        if(node1 == i){
            data_line.push_back(node2);
        }else{
            //将i对应的边s填入Edges中
            Edges[i] = data_line;
            data_line.clear();

            //将数据填入下一行中
            data_line.push_back(node2);
            i++;
        }
    }
    fp.close();
}




void Data::RR_perturb(){
    double q = 1/(1+exp(eps2));
    double pro=0;
    for(int i=0;i<maxnode;i++){
        vector<bool> noisy_line(maxnode);
        noisy_line.assign(maxnode, 0);
        for(int j=i+1;j<maxnode;j++){
            pro = dis(gen);
            if(binary_search(Edges[i].begin(),Edges[i].end(),j)){
                if(pro>q){
                    noisy_line[j]=true;
                }
            }else{
                if(pro<q){
                    noisy_line[j]=true;
                }
            }
        
        }
        if(i%1000==0)cout<<"随机扰动已完成"<<(float(i)*100)/maxnode<<"%"<<endl;
            noisy_Edges[i] = noisy_line;
    }
}

//将扰动后的数据写入txt文件，减少运行时间
void Data::wirte_noisy(){
    ofstream fout(NoisyData);
    for(int i=0;i<maxnode;i++){
        fout<<i;
        for(int j=i;j<noisy_Edges[i].size();j++){
            fout<<","<<noisy_Edges[i][j];
        }
        fout<<"\n";
    }
    fout.close();
}

void Data::counting(){
    int count;
    int n;
    for(int i=0;i<maxnode;i++){
        count=0;
        n=0;
        if(i%1000==0)cout<<"计数已完成"<<(float(i)*100)/maxnode<<"%"<<endl;
        for(int j=0;j<Edges[i].size();j++){
            for(int k=j+1;k<Edges[i].size();k++){
                if(noisy_Edges[j][k]==1){
                    count++;
                }
                n++;
            }
        }
        sum = sum + (count*(1+exp(eps2))-n)/(exp(eps2)-1);
    }
    cout<<"本次统计出三角形的总数量为："<<sum<<endl;
}

int main(){
    Data data;
    //*******读取数据
    data.readFile(EdgePath);
    cout<<"本次读取"<<maxnode<<"项数据，开始进行随机扰动"<<endl;

    //*******随机扰动
    data.RR_perturb();
    cout<<"数据随机扰动完成，开始写入数据......"<<endl;

    // //*******扰动数据写入csv(一般不需要)
    //data.wirte_noisy();

    //****开始进行三角形计数
    cout<<"开始进行三角形计数"<<endl;
    data.counting();

}