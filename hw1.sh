./Problem1_a_mirror ./images/problem1/dog.raw ./result/problem1/mirrored_dog.raw 3 300

--resizing
./Problem1_a_resize ./images/problem1/dog.raw ./result/problem1/dog_resizeTo200.raw 200 3 300
./Problem1_a_resize ./images/problem1/dog.raw ./result/problem1/dog_resizeTo400.raw 400 3 300

--composition
./Problem1_a_composition 220 ./result/problem1/mirrored_dog.raw ./images/problem1/beach.raw ./result/problem1/dog_beach_composition.raw 300 808 1914
--need further modification

--rgbTOcmy(k)
./Problem1_b_rgb2cmk ./images/problem1/parrot.raw ./result/problem1/parrot_C.raw 384 512 0
./Problem1_b_rgb2cmk ./images/problem1/parrot.raw ./result/problem1/parrot_M.raw 384 512 1
./Problem1_b_rgb2cmk ./images/problem1/parrot.raw ./result/problem1/parrot_Y.raw 384 512 2
