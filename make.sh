#进入对应的python环境
# source GraphDP/bin/activate
#生成随机数据，路径输入为data文件夹所在目录
python3 ./python/create_graph.py 10000 /home/user202234261072/src/GraphDP/Graph-DP
#编译counting_triangles.cpp
g++ ./cpp/counting_triagles.cpp -o ./cpp/counting_triagles.exe
#运行counting_triangles.exe
./cpp/counting_triagles.exe 10000 /home/user202234261072/src/GraphDP/Graph-DP

#编译counting_star.cpp
g++ ./cpp/counting_stars.cpp -o ./cpp/counting_stars.exe
#运行counting_star.exe
./cpp/counting_stars.exe 3 /home/user202234261072/src/GraphDP/Graph-DP
##
