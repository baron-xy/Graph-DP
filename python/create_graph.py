###**********
#随机创建一个指定 n 节点数量的数据集  
###**********
# 随机生成n个节点的图
import string
from tkinter import N
from traceback import print_tb
import numpy as np
import networkx as nx   #导入networkx包
import random #导入random包
import matplotlib.pyplot as plt #导入画图工具包
import csv
import sys

if len(sys.argv)<3:
    print("input [n][DatasetPath]")
    sys.exit()


#一些重要的参数
n = int(sys.argv[1])
path = sys.argv[2]
#调整矩阵的稠密程度
p = 0.2

G = nx.Graph()			#建立无向图
H = nx.path_graph(n)	#添加节点
G.add_nodes_from(H)		#添加节点
 
 
print("开始生成随机边")
#随机产生边
for i in range(n):
    probability = np.random.rand(n)
    e = random.random() * p
    for j in range(i+1,n):
        if(probability[j]<e):
            G.add_edge(i,j)
print("随机边生成完毕...")

print("正在统计数据...")
#将得到的图信息输出
options = {
    'node_color': 'black',
    'node_size': 1,
    'width': 0.3,
    'label': True,
}
nx.draw(G,**options)



# #统计图形的三角形子图个数
D = nx.triangles(G)
num_triangles = sum(D.values())/3
print("图形中三角形数量:",num_triangles)
result = "图形中三角形数量为：" + str(num_triangles)
with open(path + '/data/log.txt','w',encoding='GBK') as file_handle:   # .txt可以不自己新建,代码会自动新建
    file_handle.write(result)     # 写入
    file_handle.write('\n')         # 有时放在循环里面需要自动转行，不然会覆盖上一条数据

plt.savefig(path + "/data/graph.jpg")



print("正在写出数据...")
#保存每个节点的度
degree = G.degree()
#创建文件对象
f = open(path + '/data/deg.csv','w',encoding='utf-8',newline='')
#基于文件对象构建 csv写入对象,并写入
csv_writer = csv.writer(f)
for i in degree:
    csv_writer.writerow(i)

# 5. 关闭文件
f.close()


#保存图中的所有边
edegs = G.edges()
#创建文件对象
f = open(path + '/data/edge.csv','w',encoding='utf-8',newline='')
#基于文件对象构建 csv写入对象,并写入
csv_writer = csv.writer(f)
for i in edegs:
    csv_writer.writerow(i)
# 5. 关闭文件
f.close()
