./Problem1_a_mirror ./images/problem1/dog.raw ./result/problem1/mirrored_dog.raw 3 300

--resizing
./Problem1_a_resize ./images/problem1/dog.raw ./result/problem1/dog_resizeTo200.raw 200 3 300
./Problem1_a_resize ./images/problem1/dog.raw ./result/problem1/dog_resizeTo400.raw 400 3 300

--composition
./Problem1_a_composition 220 ./result/problem1/mirrored_dog.raw ./images/problem1/beach.raw ./result/problem1/dog_beach_composition.raw 300 808 1914
--need further modification
