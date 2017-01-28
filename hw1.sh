./Problem1_a_mirror ./images/problem1/dog.raw ./result/problem1/mirrored_dog.raw 3 300

--resizing
./Problem1_a_resize ./images/problem1/dog.raw ./result/problem1/dog_resizeTo200.raw 200 3 300
./Problem1_a_resize ./images/problem1/dog.raw ./result/problem1/dog_resizeTo400.raw 400 3 300

--composition
./Problem1_a_composition 220 ./result/problem1/mirrored_dog.raw ./images/problem1/beach.raw ./result/problem1/dog_beach_composition.raw 300 808 1914
--need further modification

--rgbTOcmy(k)
./Problem1_b_rgb2cmk ./images/problem1/parrot.raw ./result/problem1/parrot_C.raw 384 512 0
./Problem1_b_rgb2cmk ./images/problem1/parrot.raw ./result/problem1/parrot_M.raw 384 512 1  ---???
./Problem1_b_rgb2cmk ./images/problem1/parrot.raw ./result/problem1/parrot_Y.raw 384 512 2

./Problem1_b_rgb2cmk ./images/problem1/building.raw ./result/problem1/building_Y.raw 600 789 2
./Problem1_b_rgb2cmk ./images/problem1/building.raw ./result/problem1/building_M.raw 600 789 1
./Problem1_b_rgb2cmk ./images/problem1/building.raw ./result/problem1/building_C.raw 600 789 0

--rgb to hsl !!!!!!!! Error with L

./Problem1_b_rgb2hsl ./images/problem1/cat.raw ./result/problem1/cat_H.raw 398 600 0
./Problem1_b_rgb2hsl ./images/problem1/cat.raw ./result/problem1/cat_S.raw 398 600 1
./Problem1_b_rgb2hsl ./images/problem1/cat.raw ./result/problem1/cat_L.raw 398 600 2 ----????

./Problem1_b_rgb2hsl ./images/problem1/dolphin.raw ./result/problem1/dolphin_H.raw 480 640 0
./Problem1_b_rgb2hsl ./images/problem1/dolphin.raw ./result/problem1/dolphin_S.raw 480 640 1
./Problem1_b_rgb2hsl ./images/problem1/dolphin.raw ./result/problem1/dolphin_L.raw 480 640 2 ----????


