#�����Ӧ��python����
# source GraphDP/bin/activate
#����������ݣ�·������Ϊdata�ļ�������Ŀ¼
python3 ./python/create_graph.py 10000 /home/user202234261072/src/GraphDP/Graph-DP
#����counting_triangles.cpp
g++ ./cpp/counting_triagles.cpp -o ./cpp/counting_triagles.exe
#����counting_triangles.exe
./cpp/counting_triagles.exe 10000 /home/user202234261072/src/GraphDP/Graph-DP

#����counting_star.cpp
g++ ./cpp/counting_stars.cpp -o ./cpp/counting_stars.exe
#����counting_star.exe
./cpp/counting_stars.exe 3 /home/user202234261072/src/GraphDP/Graph-DP
##
