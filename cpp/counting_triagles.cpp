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
//�ð��ڰ���laplace����������㷨
#include "include/stats.hpp"
#define maxnode 10000
//�������������
stats::rand_engine_t engine((unsigned)time(0));
random_device rd;
mt19937 gen(rd());

double eps1=1;   //��һ�׶ε���˽Ԥ��
double eps2=1;   //�ڶ��׶ε���˽Ԥ��

//����0��1�������
uniform_real_distribution<> dis(0,1);
//����������˹�����
double Laplace_noisy(double GS , double eps){
    return stats::rlaplace(0.0, GS/eps,engine);
};

//���������C(n,m)
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
    //��������һ��ifstream����ָ���ļ�·��
    ifstream fp(EdgePath); 
    string line;

    int i=0;
    int node1;
    int node2;
    vector<int> data_line;
    while (getline(fp,line)){ //ѭ����ȡÿ������
        string number;
        istringstream readstr(line); //string��������
        // ��ȡ��һ���ڵ�
        getline(readstr,number,',');
        node1 = atoi(number.c_str());
        //��ȡ�ڶ����ڵ�
        getline(readstr,number,',');
        node2 = atoi(number.c_str());


        if(node1 == i){
            data_line.push_back(node2);
        }else{
            //��i��Ӧ�ı�s����Edges��
            Edges[i] = data_line;
            data_line.clear();

            //������������һ����
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
        if(i%1000==0)cout<<"����Ŷ������"<<(float(i)*100)/maxnode<<"%"<<endl;
            noisy_Edges[i] = noisy_line;
    }
}

//���Ŷ��������д��txt�ļ�����������ʱ��
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
        if(i%1000==0)cout<<"���������"<<(float(i)*100)/maxnode<<"%"<<endl;
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
    cout<<"����ͳ�Ƴ������ε�������Ϊ��"<<sum<<endl;
}

int main(){
    Data data;
    //*******��ȡ����
    data.readFile(EdgePath);
    cout<<"���ζ�ȡ"<<maxnode<<"�����ݣ���ʼ��������Ŷ�"<<endl;

    //*******����Ŷ�
    data.RR_perturb();
    cout<<"��������Ŷ���ɣ���ʼд������......"<<endl;

    // //*******�Ŷ�����д��csv(һ�㲻��Ҫ)
    //data.wirte_noisy();

    //****��ʼ���������μ���
    cout<<"��ʼ���������μ���"<<endl;
    data.counting();

}