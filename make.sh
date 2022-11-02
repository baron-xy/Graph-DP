#生成随机数据，路径输入为data文件夹所在目录
python .\python\create_graph.py 1000 D:\TriangleLD
#编译counting_triangles.cpp
g++ .\cpp\counting_triagles.cpp -o .\cpp\counting_triagles.exe
#运行counting_triangles.exe
.\cpp\counting_triagles.exe 1000 D:\TriangleLD

#编译counting_star.cpp
g++ .\cpp\counting_stars.cpp -o .\cpp\counting_stars.exe
#运行counting_star.exe
.\cpp\counting_stars.exe 2 D:\TriangleLD\data\deg.csv
##
