
import tensorflow as tf
import numpy as np
# Declaring placeholders:

var2=np.random.rand(20,32,32,3)
images = tf.placeholder(tf.float32,shape=[20,32,32,3])

shape=[5,5,3,6]
def create_samples(samples,shape=[5,5,3,6]):

  result=None
  SIZE=samples.get_shape().as_list()

  for n in xrange(0,int(SIZE[0]/1)):
    print('kmeans on n=',n)
    patches=None
    # in each image get 5x5x3 patches
    for r in xrange(2,SIZE[1]-2):
      for c in xrange(2,SIZE[2]-2):
        begin=[n,r-2,c-2,0]
        size=[1,shape[0],shape[1],shape[2]]
        croppedImg=tf.slice(samples,begin,size)
        #squeeze the cropped image size into 5x5x3
        #concate into patch_number x5x5x3
        if patches is None:
          patches = croppedImg
        else:
          patches=tf.concat(0,[patches,croppedImg])
    #calculate the variance for each patch 
    #and result the index of patches
    _,var=tf.nn.moments(patches,[1,2,3])

    topK=int(var.get_shape().as_list()[0]/30)
    _,I=tf.nn.top_k(var,k=topK)
    selected_patches=tf.gather(patches,I)
    #extract high varience patches
    #10x5x5x3 --> 10x(5*5*3)
    selected_patches=tf.reshape(selected_patches,[-1,shape[0]*shape[1]*shape[2]])
    #collect all patches from each image
    if result is None:
      result=selected_patches
    else:
      result=tf.concat(0,[result,selected_patches])

  # result=tf.random_shuffle(result)[0:int(SIZE[0]*topK*2/3)]

  print('kmean result size is',result.get_shape().as_list())

  return result

def choose_random_centroids(samples, n_clusters):
  # Step 0: Initialisation: Select `n_clusters` number of random points
  n_samples = tf.shape(samples)[0]
  random_indices = tf.random_shuffle(tf.range(0, n_samples))
  begin = [0,]
  size = [n_clusters,]
  size[0] = n_clusters
  centroid_indices = tf.slice(random_indices, begin, size)
  initial_centroids = tf.gather(samples, centroid_indices)
  return initial_centroids

def assign_to_nearest(samples, centroids):
  # Finds the nearest centroid for each sample

  # START from http://esciencegroup.com/2016/01/05/an-encounter-with-googles-tensorflow/
  expanded_vectors = tf.expand_dims(samples, 0)
  expanded_centroids = tf.expand_dims(centroids, 1)
  distances = tf.reduce_sum( tf.square(
              tf.sub(expanded_vectors, expanded_centroids)), 2)
  mins = tf.argmin(distances, 0)
  # END from http://esciencegroup.com/2016/01/05/an-encounter-with-googles-tensorflow/
  nearest_indices = mins
  return nearest_indices

def update_centroids(samples, nearest_indices, n_clusters):
  # Updates the centroid to be the mean of all samples associated with it.
  nearest_indices = tf.to_int32(nearest_indices)
  partitions = tf.dynamic_partition(samples, nearest_indices, n_clusters)
  new_centroids = tf.concat(0,[tf.expand_dims(tf.reduce_mean(partition, 0), 0) for partition in partitions])
  return new_centroids

# def helper(updated_centroids,samples,shape):
# 	old_centroids=updated_centroids
#   	nearest_indices=assign_to_nearest(samples,old_centroids)
#   	updated_centroids=update_centroids(samples,nearest_indices,shape[3])

def main(images):
  samples=create_samples(images,shape)
  old_centroids=choose_random_centroids(samples,shape[3])
  nearest_indices=assign_to_nearest(samples,old_centroids)
  updated_centroids=update_centroids(samples,nearest_indices,shape[3])
  iter=0
  while iter<100:
    iter=iter+1
    print(iter)
    old_centroids=updated_centroids
    nearest_indices=assign_to_nearest(samples,old_centroids)
    updated_centroids=update_centroids(samples,nearest_indices,shape[3])
  ifEqual=tf.not_equal(updated_centroids,old_centroids)
  ifEqual=tf.where(ifEqual)
  size=tf.size(ifEqual)
  ifEqual=tf.not_equal(size,tf.constant(0))
  return size,updated_centroids

size,updated_centroids=main(images)

with tf.Session() as sess:
	
	# B=sess.run(old_centroids, feed_dict={images: var2})
	# # Please note: Since we are only declaring the variables in our graph, we don't need to run 'tf.global_variables_initializer'!
	# # Create random numbers to feed the variables:
	# A=sess.run(updated_centroids, feed_dict={images: var2})

	C,E=sess.run([size,updated_centroids], feed_dict={images: var2})
	# D=sess.run(size, feed_dict={images: var2})

	sess.close()

	