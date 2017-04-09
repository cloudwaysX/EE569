import tensorflow as tf
import numpy as np
from PIL import Image

# #mnist = input_data.read_data_sets("/tmp/data/", one_hot=True)  # Store mnist data for this tutorial

# learning_rate = 0.01
# training_epochs = 25
# batch_size = 100
# display_step = 1
# logs_path = './logFiles'



def LeNet5(x):
	with tf.name_scope('Model'):
		# Layer 1: Convolutional. Input = 32x32x3. Output = 28x28x6.
		conv1_W = tf.get_variable("conv1_W", shape=(5,5,3,6),
           initializer=tf.contrib.layers.xavier_initializer())
		conv1_b = tf.Variable(tf.zeros(6))
		conv1=tf.nn.conv2d(x,conv1_W,strides=[1,1,1,1],padding='VALID',use_cudnn_on_gpu=True)+conv1_b # format here is NHWC
		conv1=tf.nn.relu(conv1)

		#Layer2: Pooling. Input = 28x28x6. Output = 14x14x6.
		pool1=tf.nn.max_pool(conv1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='VALID')

		#Layer3: Convolutional. Input = 14x14x5. Output = 10x10x16.
		conv2_W = tf.get_variable("conv2_W", shape=(5,5,6,16),
           initializer=tf.contrib.layers.xavier_initializer())
		conv2_b = tf.Variable(tf.zeros(16))
		conv2=tf.nn.conv2d(pool1,conv2_W,strides=[1,1,1,1],padding='VALID',use_cudnn_on_gpu=True)+conv2_b # format here is NHWC
		conv2=tf.nn.relu(conv2)

		#Layer4: Pooling. Input = 10x10x16. Output = 5x5x16.
		pool2=tf.nn.max_pool(conv2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='VALID')

		#Layer5:Fully Connected. Input =400. Output =120
		fc0   = flatten(conv2)
		fc1_W = tf.get_variable("fc1_W", shape=(400,200),
           initializer=tf.contrib.layers.xavier_initializer())
    	fc1_b = tf.Variable(tf.zeros(120))
    	fc1   = tf.matmul(fc0, fc1_W) + fc1_b
    	fc1    = tf.nn.relu(fc1)

    	#Layer6:Fully Connected. Input = 120. Output = 84
    	fc2_W = tf.get_variable("fc2_W", shape=(120,84),
    		initializer=tf.contrib.layers.xavier_initializer())
    	fc2_b = tf.Variable(tf.zeros(84))
    	fc2   = tf.matmul(fc1, fc2_W) + fc2_b
    	fc2    = tf.nn.relu(fc2)

    	#Layer6:Fully Connected. Input = 84. Output = 10
    	softmax_W = tf.get_variable("softmax_W", shape=(84,10),
           initializer=tf.contrib.layers.xavier_initializer())
    	softmax_b = tf.Variable(tf.zeros(10))
    	pred = tf.nn.softmax(tf.matmul(x,softmax_W)+softmax_b)
    	return pred

# def train_LeNet5():
# 	pred = LeNet5(x)
# 	with tf.name_scope('Loss'):
# 		# Minimize error using cross entropy
# 		cost=tf.reduce_mean(-tf.reduce_sum(y*tf.log(pred), reduction_indices=1))
# 	with tf.name_scope('SGD'):
#     	# Gradient Descent
# 		optimizer = tf.train.GradientDescentOptimizer(learning_rate).minimize(cost)
# 	with tf.name_scope('Accuracy'):
#     	# Accuracy
# 		acc = tf.equal(tf.argmax(pred, 1), tf.argmax(y, 1))
# 		acc = tf.reduce_mean(tf.cast(acc, tf.float32))

# 		# Create a summary to monitor cost tensor
# 	tf.summary.scalar("loss", cost)
# 	# Create a summary to monitor accuracy tensor
# 	tf.summary.scalar("accuracy", acc)
# 	# Merge all summaries into a single op
# 	merged_summary_op = tf.summary.merge_all()

# 	# Launch the graph
# 	with tf.Session() as sess:
# 		sess.run(init)

#     		# op to write logs to Tensorboard
# 		summary_writer = tf.summary.FileWriter(logs_path,graph = tf.get_default_graph())

#     		# Training cycle
# 		for epoch in range(training_epochs):
# 			avg_cost = 0.
# 			total_batch = int(mnist.train.num_examples/batch_size)
# 			# Loop over all batches
# 			for i in range(total_batch):
# 				batch_xs, batch_ys = mnist.train.next_batch(batch_size)
# 				# Run optimization op (backprop), cost op (to get loss value)
# 				# and summary nodes
# 				_, c, summary = sess.run([optimizer, cost, merged_summary_op],feed_dict={x: batch_xs, y: batch_ys})
# 				# Write logs at every iteration
# 				summary_writer.add_summary(summary, epoch * total_batch + i)
# 				# Compute average loss
# 				avg_cost += c / total_batch
			
# 			# Display logs per epoch step
# 			if (epoch+1) % display_step == 0:
# 				print("Epoch:", '%04d' % (epoch+1), "cost=", "{:.9f}".format(avg_cost))

# 		print("Optimization Finished!")

# 		# Test model
# 		# Accuracy of 0.91 -> 91%
# 		print("Accuracy:", acc.eval({x: mnist.test.images, y: mnist.test.labels}))


def unpickle(filename):
	import cPickle
	import os
	root="./cifar-10-batches-py"
	filename = os.path.join(root,filename)
	with open(filename,'rb') as f:
		dict = cPickle.load(f)
	return dict


#----------------------------------------------------------------------------------------------------#
# Main function:
def main():
	#--------------------image and labels loading------------------------------------------#
	for i in xrange(1,5,1):
		index="%d" %(i,)
		filename = 'data_batch_'+index
		trainingDict=unpickle(filename)
		cur_training_data=trainingDict['data']
		cur_training_labels=np.asarray(trainingDict['labels'])

		cur_training_data=cur_training_data.reshape(cur_training_data.shape[0],3,32,32)

		try:
			training_data
		except NameError:
			training_data = cur_training_data
		else:
			training_data=np.concatenate((training_data,cur_training_data),axis=0)


		try:
			training_labels
		except NameError:
			training_labels = cur_training_labels
		else:
			training_labels=np.concatenate((training_labels,cur_training_labels),axis=0)

		# Finally we get training images as 40000x3x32x32 and training labels 40000

	filename = 'test_batch'
	testingDict=unpickle(filename)
	testing_data=testingDict['data']
	testing_labels=np.asarray(testingDict['labels'])

	print('\n')
	print("Image Shape: {}".format(training_data[0].shape))
	print('\n')
	print("Training Set:   {} samples".format(len(training_data)))
	print("Testing Set:       {} samples".format(len(testing_data)))

	#---------------------Data Visualization----------------------------------------------------#


	import random
	import matplotlib.pyplot as plt
	index = random.randint(0, len(training_data))
	image = training_data[index].squeeze()

	plt.figure(figsize=(1,1))
	plt.imshow(image.transpose(1,2,0))
	#plt.show()
	#plt.savefig('random_sample.png')
	print(training_labels[index])


	#-----------------Preprosessing-------------------------------------#

	from sklearn.utils import shuffle

	training_data, training_labels = shuffle(training_data, training_labels)

	#---------------Training part----------------------------------------#


#----------------------------------------------------------------------------------------------------#
# Boilerplate syntax:
if __name__ == '__main__':
	main()



