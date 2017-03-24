


# maybe be different in the different computer
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
#
g++ -std=c++0x  -o P2_b P2_b.cpp `pkg-config opencv --cflags --libs`
./P2_b ./HW3_images/P3/Boat.raw ./results/P2/Boat_SE.jpg 0
./P2_b ./HW3_images/P3/Boat.raw ./results/P2/Boat_SE_binary.jpg 0.2
./P2_b ./HW3_images/P3/Castle.raw ./results/P2/Castle_SE.jpg 0
./P2_b ./HW3_images/P3/Castle.raw ./results/P2/Castle_SE_binary.jpg 0.3



g++ -std=c++0x  -o P3_b P3_b.cpp `pkg-config opencv --cflags --libs`
./P3_b ./HW3_images/P2/convertible_1.raw ./HW3_images/P2/suv.raw test1.jpg test2.jpg test3.jpg 0

