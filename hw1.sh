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


 ./Problem1_b_SepiaFilter ./images/problem1/beach.raw ./result/problem1/beach_gray.raw 808 1914 0 --???
 ./Problem1_b_SepiaFilter ./images/problem1/beach.raw ./result/problem1/beach_Sephia.raw 808 1914 1


--Problem1_b_rgb2hsl
--a: Linear Transfer
./Problem2_a_linearTrans ./images/problem2/tulip_bright.raw ./result/problem2/tulip_bright_highContrast_kis4.raw 366 400 4
--5 is the best !!!
./Problem2_a_linearTrans ./images/problem2/tulip_bright.raw ./result/problem2/tulip_bright_highContrast_kis5.raw 366 400 5
./Problem2_a_linearTrans ./images/problem2/tulip_bright.raw ./result/problem2/tulip_bright_highContrast_kis6.raw 366 400 6

./Problem2_a_linearTrans ./images/problem2/tulip_dark.raw ./result/problem2/tulip_dark_highContrastA_kis6.raw 366 400 6
366 400
./Problem2_a_linearTrans ./images/problem2/tulip_dark.raw ./result/problem2/tulip_dark_highContrastA_kis5.raw 366 400 5
366 400
./Problem2_a_linearTrans ./images/problem2/tulip_dark.raw ./result/problem2/tulip_dark_highContrastA_kis4.raw 366 400 4
366 400
366 400
./Problem2_a_linearTrans ./images/problem2/tulip_dark.raw ./result/problem2/tulip_dark_highContrastA_kis3.raw 366 400 3
366 400


-a: HistEqualUsingCDF
./Problem2_a_hisEqual ./images/problem2/tulip_bright.raw  ./result/problem2/tulip_bright_highContrastB.raw 366 400 255
./Problem2_a_hisEqual ./images/problem2/tulip_dark.raw  ./result/problem2/tulip_dark_highContrastB.raw 366 400 255


