cd Problem1

#(a) transform the cup image into diamond shape
#-program name -input image -output image
./P1_a ../images/P1/cup1.raw ../result/P1/cup1_diamond.raw
./P1_a ../images/P1/cup2.raw ../result/P1/cup2_diamond.raw

#(b) puzzle matching
#-program name -input image -output image -Piece range[begin Col; end Col]
./P1_b ../images/P1/Hillary.raw ../result/P1/Hillary_puzzle.raw 0 250
./P1_b ../images/P1/Trump.raw ../result/P1/Trump_puzzle.raw 250  499

#(c) Homography transform
#-program name -mode:[0 is tartan,1 is trojan]
./P1_c 0
./P1_c 1

cd ..

cd Problem2
#Dithering
#-program name -Size of the Bayer Matrix [0 means the givenn A4 matrix] -output image -gray intensity level
#(a)
./P2_a 1 ../result/P2/A2Bayer.raw 2
./P2_a 3 ../result/P2/A8Bayer.raw 2
#(b)
./P2_a 0 ../result/P2/A4.raw 2
./P2_a 2 ../result/P2/A4Bayer.raw 2
#(c) Self design
./P2_a 3 ../result/P2/A8Bayer_4level.raw 4

#Error Diffusion
#-program name -matrix type[0: SF 1:JJN 2: Stucki] -output image
./P2_b 0 ../result/P2/errorDif_1.raw
./P2_b 1 ../result/P2/errorDif_2.raw
./P2_b 2 ../result/P2/errorDif_3.raw

cd ..

cd Problem3
#Shrinking, Thining and Skeleton
#-program_name -mode -input_filename -output_filename -iter
./P3_a 0 ../images/P3/squares.raw ../result/P3/ShrinkingResult.raw 100
./P3_a 1 ../images/P3/letterE.raw ../result/P3/ThiningResult.raw 100
./P3_a 2 ../images/P3/letterE.raw ../result/P3/SkeletonResult.raw 100

#counting game
./P3_b







