cd Problem1

g++ -o P1_a P1_a.cpp `pkg-config opencv --cflags --libs`
g++ -o P1_b P1_b.cpp `pkg-config opencv --cflags --libs`
g++ -o P1_c P1_c.cpp `pkg-config opencv --cflags --libs`


cd ..

cd Problem2
g++ -o P2_a P2_a.cpp
g++ -o P2_b P2_b.cpp

cd ..

cd Problem3
g++ -o P3_a P3_a.cpp
g++ -o P3_b P3_b.cpp






