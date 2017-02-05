#Mirror the dog
./Problem1_a_mirror ./images/problem1/dog.raw ./result/problem1/mirrored_dog.raw 3 300

#resizing
./Problem1_a_resize ./images/problem1/dog.raw ./result/problem1/dog_resizeTo200.raw 200 3 300
./Problem1_a_resize ./images/problem1/dog.raw ./result/problem1/dog_resizeTo400.raw 400 3 300

#composition
./Problem1_a_composition 255 255 220 ./result/problem1/mirrored_dog.raw ./images/problem1/beach.raw ./result/problem1/dog_beach_composition_R255B255G220.raw 300 808 1914
./Problem1_a_composition 155 155 135 ./result/problem1/mirrored_dog.raw ./images/problem1/beach.raw ./result/problem1/dog_beach_composition_R155B155G135.raw 300 808 1914

#rgbTOcmy(k)
./Problem1_b_rgb2cmk ./images/problem1/parrot.raw ./result/problem1/parrot_C.raw 384 512 0
./Problem1_b_rgb2cmk ./images/problem1/parrot.raw ./result/problem1/parrot_M.raw 384 512 1 
./Problem1_b_rgb2cmk ./images/problem1/parrot.raw ./result/problem1/parrot_Y.raw 384 512 2

./Problem1_b_rgb2cmk ./images/problem1/building.raw ./result/problem1/building_Y.raw 600 789 2
./Problem1_b_rgb2cmk ./images/problem1/building.raw ./result/problem1/building_M.raw 600 789 1
./Problem1_b_rgb2cmk ./images/problem1/building.raw ./result/problem1/building_C.raw 600 789 0

#Problem1_b_rgb2hsl

./Problem1_b_rgb2hsl ./images/problem1/cat.raw ./result/problem1/cat_H.raw 398 600 0
./Problem1_b_rgb2hsl ./images/problem1/cat.raw ./result/problem1/cat_S.raw 398 600 1
./Problem1_b_rgb2hsl ./images/problem1/cat.raw ./result/problem1/cat_L.raw 398 600 2 

./Problem1_b_rgb2hsl ./images/problem1/dolphin.raw ./result/problem1/dolphin_H.raw 480 640 0
./Problem1_b_rgb2hsl ./images/problem1/dolphin.raw ./result/problem1/dolphin_S.raw 480 640 1
./Problem1_b_rgb2hsl ./images/problem1/dolphin.raw ./result/problem1/dolphin_L.raw 480 640 2 

#Problem1_b_SepiaFilter mode 0 will out put the gray image; mode 0 out put the final filtered image
 ./Problem1_b_SepiaFilter ./images/problem1/beach.raw ./result/problem1/beach_gray.raw 808 1914 0 
 ./Problem1_b_SepiaFilter ./images/problem1/beach.raw ./result/problem1/beach_Sephia.raw 808 1914 1

#Problem1_c using overlay effect
./Problem1_c_overlay ./images/problem1/top_layer.raw ./images/problem1/bottom_layer.raw ./result/problem1/overlay.raw 900 890

#Problem2
#a: Linear Transfer
./Problem2_a_linearTrans ./images/problem2/tulip_bright.raw ./result/problem2/tulip_bright_highContrast_kis3.raw 366 400 3 270
./Problem2_a_linearTrans ./images/problem2/tulip_dark.raw ./result/problem2/tulip_dark_highContrast_kis3.raw 366 400 3 -5
./Problem2_a_hisEqual ./images/problem2/tulip_mix.raw  ./result/problem2/tulip_mix_highContrastB.raw 366 400 255


#a: HistEqualUsingCDF
./Problem2_a_hisEqual ./images/problem2/tulip_bright.raw  ./result/problem2/tulip_bright_highContrastB.raw 366 400 255
./Problem2_a_hisEqual ./images/problem2/tulip_dark.raw  ./result/problem2/tulip_dark_highContrastB.raw 366 400 255
./Problem2_a_hisEqual ./images/problem2/tulip_mix.raw  ./result/problem2/tulip_mix_highContrastB.raw 366 400 255

#Problem2_b 
#in RGB space
./Problem2_a_linearTrans ./images/problem2/bedroom.raw ./result/problem2/bedroom_highContrast_kis3.raw 400 940 3 128  3
400 940
./Problem2_a_hisEqual ./images/problem2/bedroom.raw  ./result/problem2/bedroom_highContrastB.raw  400 940 255 3
#in HSL Space
./Problem2_b_hsl ./images/problem2/bedroom.raw ./result/problem2/bedroom_histEqual_hsl.raw 400 940



#Problem2_c 
./Problem2_c ./images/problem2/forest_2.raw ./result/problem2/forest_2_histtansGauss.raw 413 550
./Problem2_c ./images/problem2/forest_1.raw ./result/problem2/forest_1_histtansGauss.raw 600 960


#Probelm 3_a 
#Test original PSNR first
./PSNR_test ./images/problem3/pepper_noisy.raw ./images/problem3/pepper.raw 512 512
#Median only
./Problem3_a ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseMedianOnly_sigma1size3.raw 512 512 3 1 1
./PSNR_test ./result/problem3/pepper_denoiseMedianOnly_sigma1size3.raw ./images/problem3/pepper.raw 512 512
#Gaussian only
./Problem3_a ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseGaussianOnly_sigma1size3.raw 512 512 3 1 2
./PSNR_test ./result/problem3/pepper_denoiseGaussianOnly_sigma1size3.raw ./images/problem3/pepper.raw 512 512
#Median + Gaussin sigma=1 size=3
./Problem3_a ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseMedianGaussian_sigma1size3.raw 512 512 3 1 4
./PSNR_test ./result/problem3/pepper_denoiseMedianGaussian_sigma1size3.raw ./images/problem3/pepper.raw 512 512
#Gaussion + Median sigma=1 size=3
./Problem3_a ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseGaussianMedian_sigma1size3.raw 512 512 3 1 3
./PSNR_test ./result/problem3/pepper_denoiseGaussianMedian_sigma1size3.raw ./images/problem3/pepper.raw 512 512
#best performance
./Problem3_a ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseMedianGaussian_sigma0.6size3.raw 512 512 3 0.6 4
./PSNR_test ./result/problem3/pepper_denoiseMedianGaussian_sigma0.6size3.raw ./images/problem3/pepper.raw 512 512
#test the size of filter window
 ./Problem3_a ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseMedianGaussian_sigma1size11.raw 512 512 11 1 4
 ./PSNR_test ./result/problem3/pepper_denoiseMedianGaussian_sigma1size11.raw ./images/problem3/pepper.raw 512 512
 ./Problem3_a ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseMedianGaussian_sigma1size7.raw 512 512 7 1 4
 ./PSNR_test ./result/problem3/pepper_denoiseMedianGaussian_sigma1size7.raw ./images/problem7/pepper.raw 512 512
 ./Problem3_a ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseMedianGaussian_sigma1size5.raw 512 512 5 1 4
 ./PSNR_test ./result/problem3/pepper_denoiseMedianGaussian_sigma1size5.raw ./images/problem3/pepper.raw 512 512

#Problem3_b Non local mean
./Problem3_b ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseNLM_h50_sigma1_size3.raw 512 512 3 50 1
./PSNR_test ./result/problem3/pepper_denoiseNLM_h50_sigma1_size3.raw ./images/problem3/pepper.raw 512 512

./Problem3_b ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseNLM_h50_sigma1_size5.raw 512 512 5 50 1
./PSNR_test ./result/problem3/pepper_denoiseNLM_h50_sigma1_size5.raw ./images/problem3/pepper.raw 512 512

./Problem3_b ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseNLM_h50_sigma1_size7.raw 512 512 7 50 1
./PSNR_test ./result/problem3/pepper_denoiseNLM_h50_sigma1_size7.raw ./images/problem3/pepper.raw 512 512

./Problem3_b ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseNLM_h50_sigma2_size7.raw 512 512 7 50 2
./PSNR_test ./result/problem3/pepper_denoiseNLM_h50_sigma2_size7.raw ./images/problem3/pepper.raw 512 512

./Problem3_b ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseNLM_h50_sigma1_size7.raw 512 512 7 50 1
./PSNR_test ./result/problem3/pepper_denoiseNLM_h50_sigma1_size7.raw ./images/problem3/pepper.raw 512 512

./Problem3_b ./images/problem3/pepper_noisy.raw ./result/problem3/pepper_denoiseNLM_h50_sigma5_size7.raw 512 512 7 50 5
./PSNR_test ./result/problem3/pepper_denoiseNLM_h50_sigma5_size7.raw ./images/problem3/pepper.raw 512 512

#Method A and B on the sailboat (only include best performance)
./Problem3_a ./images/problem3/sailboat_noisy.raw ./result/problem3/sailboat_denoiseMedianGaussian_sigma0.6size3.raw 512 512 3 0.6 4
./PSNR_test ./result/problem3/sailboat_denoiseMedianGaussian_sigma0.6size3.raw ./images/problem3/sailboat.raw 512 512

./Problem3_b ./images/problem3/sailboat_noisy.raw ./result/problem3/sailsboat_denoiseNLM_h50_sigma06_size7.raw 512 512 7 50 0.6
./PSNR_test ./result/problem3/sailsboat_denoiseNLM_h50_sigma06_size7.raw ./images/problem3/sailboat.raw 512 512