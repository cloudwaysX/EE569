import numpy as np
from PIL import Image

def unpickle(filename):
	import cPickle
	import os
	root="./cifar-10-batches-py"
	filename = os.path.join(root,filename)
	with open(filename,'rb') as f:
		dict = cPickle.load(f)
	return dict

training_data=None
training_labels=None

#--------------------image and labels loading------------------------------------------#
for i in xrange(1,6,1):
	index="%d" %(i,)
	filename = 'data_batch_'+index
	trainingDict=unpickle(filename)
	cur_training_data=trainingDict['data']
	cur_training_labels=np.asarray(trainingDict['labels'])

	cur_training_data=cur_training_data.reshape(cur_training_data.shape[0],3,32,32)

	if training_data is None:
		training_data = cur_training_data
	else:
		training_data=np.concatenate((training_data,cur_training_data),axis=0)


	if training_labels is None:
		training_labels = cur_training_labels
	else:
		training_labels=np.concatenate((training_labels,cur_training_labels),axis=0)

training_data=training_data.transpose(0,2,3,1)
# Finally we get training images as 40000x32x32x3 and training labels 40000

filename = 'test_batch'
testingDict=unpickle(filename)
testing_data=testingDict['data']
#print(testing_data.shape)
testing_data=(testing_data.reshape(testing_data.shape[0],3,32,32)).transpose(0,2,3,1)
testing_labels=np.asarray(testingDict['labels'])

print('\n')
print("Image Shape: {}".format(training_data[0].shape))
print('\n')
print("Training Set:   {} samples".format(len(training_data)))
print("Testing Set:       {} samples".format(len(testing_data)))


# -----------------Preprosessing-------------------------------------#

from sklearn.utils import shuffle

training_data, training_labels = shuffle(training_data, training_labels)

# ---------------------Data Visualization----------------------------------------------------#


# import random
# import matplotlib.pyplot as plt
# index = random.randint(0, len(testing_data))
# image = testing_data[index].squeeze()

# plt.figure(figsize=(1,1))
# plt.imshow(image)
# print(testing_labels[index])
# plt.show()
# #plt.savefig('random_sample.png')


#----------------------Setup Tensorflow------------------------------------------------------#


import tensorflow as tf

training_epochs = 100
BATCH_SIZE = 128

def LeNet5(x):

	from tensorflow.contrib.layers import flatten

	mu = 0
	sigma = 0.1

	with tf.name_scope('Model'):
		# Layer 1: Convolutional. Input = 32x32x3. Output = 28x28x6.
		# conv1_W = tf.get_variable("conv1_W", shape=(5,5,3,6),
  #          initializer=tf.contrib.layers.xavier_initializer())
  		conv1_W = tf.Variable(tf.truncated_normal(shape = [5,5,3,6],mean = mu, stddev = sigma))
		conv1_b = tf.Variable(tf.zeros(6))
		conv1=tf.nn.conv2d(x,conv1_W,strides=[1,1,1,1],padding='VALID',use_cudnn_on_gpu=True)+conv1_b # format here is NHWC
		conv1=tf.nn.relu(conv1)

		#Layer2: Pooling. Input = 28x28x6. Output = 14x14x6.
		pool1=tf.nn.max_pool(conv1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='VALID')

		# Layer3: Convolutional. Input = 14x14x5. Output = 10x10x16.
		# conv2_W = tf.get_variable("conv2_W", shape=(5,5,6,16),
  #          initializer=tf.contrib.layers.xavier_initializer())
  		conv2_W = tf.Variable(tf.truncated_normal(shape = [5,5,6,16], mean = mu, stddev = sigma))
		conv2_b = tf.Variable(tf.zeros(16))
		conv2=tf.nn.conv2d(pool1,conv2_W,strides=[1,1,1,1],padding='VALID',use_cudnn_on_gpu=True)+conv2_b # format here is NHWC
		conv2=tf.nn.relu(conv2)

		#Layer4: Pooling. Input = 10x10x16. Output = 5x5x16.
		pool2=tf.nn.max_pool(conv2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='VALID')

		#Layer5:Fully Connected. Input =400. Output =120
		fc0   = flatten(pool2)
		# fc1_W = tf.get_variable("fc1_W", shape=(400,120),
  #          initializer=tf.contrib.layers.xavier_initializer())
  		fc1_W = tf.Variable(tf.truncated_normal(shape = (400,120), mean = mu, stddev = sigma))
    	fc1_b = tf.Variable(tf.zeros(120))
    	fc1   = tf.matmul(fc0, fc1_W) + fc1_b
    	fc1    = tf.nn.relu(fc1)

    	#Layer6:Fully Connected. Input = 120. Output = 84
    	# fc2_W = tf.get_variable("fc2_W", shape=(120,84),
    	# 	initializer=tf.contrib.layers.xavier_initializer())
    	fc2_W = tf.Variable(tf.truncated_normal(shape = (120,84), mean = mu, stddev = sigma))
    	fc2_b = tf.Variable(tf.zeros(84))
    	fc2   = tf.matmul(fc1, fc2_W) + fc2_b
    	fc2    = tf.nn.relu(fc2)

    	#Layer6:Fully Connected. Input = 84. Output = 10
    	# softmax_W = tf.get_variable("softmax_W", shape=(84,10),
     #       initializer=tf.contrib.layers.xavier_initializer())
     	softmax_W = tf.Variable(tf.truncated_normal(shape = (84,10), mean = mu , stddev = sigma))
    	softmax_b = tf.Variable(tf.zeros(10))
    	#pred = tf.nn.softmax(tf.matmul(fc2,softmax_W)+softmax_b)
    	pred=tf.matmul(fc2,softmax_W)+softmax_b
    	return pred

# -------------------Train-------------------------#

x = tf.placeholder(tf.float32, (None, 32, 32, 3))
y = tf.placeholder(tf.int32, (None))
one_hot_y = tf.one_hot(y, 10)

rate = 0.001

logits = LeNet5(x)
with tf.name_scope('Loss'):
	cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits, one_hot_y)
	loss_operation = tf.reduce_mean(cross_entropy)
with tf.name_scope('Optimizer'):
	optimizer = tf.train.AdamOptimizer(learning_rate = rate)
	training_operation = optimizer.minimize(loss_operation)
with tf.name_scope('Accuracy'):
	correct_prediction = tf.equal(tf.argmax(logits, 1), tf.argmax(one_hot_y, 1))
	accuracy_operation = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
	saver = tf.train.Saver()

logs_path = './logFiles'
display_step=5
# Create a summary to monitor cost tensor
tf.summary.scalar("loss", loss_operation)
# Create a summary to monitor accuracy tensor
tf.summary.scalar("accuracy", accuracy_operation)
# Merge all summaries into a single op
merged_summary_op = tf.summary.merge_all()

def evaluate(X_data, y_data):
    num_examples = len(X_data)
    total_accuracy = 0
    sess = tf.get_default_session()
    for offset in range(0, num_examples, BATCH_SIZE):
        batch_x, batch_y = X_data[offset:offset+BATCH_SIZE], y_data[offset:offset+BATCH_SIZE]
        accuracy = sess.run(accuracy_operation, feed_dict={x: batch_x, y: batch_y})
        total_accuracy += (accuracy * len(batch_x))
    return total_accuracy / num_examples



with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    num_examples = len(training_data)

    # op to write logs to Tensorboard
    summary_writer = tf.summary.FileWriter(logs_path,graph = tf.get_default_graph())

    print("Training...")
    print()
    for i in range(training_epochs):
    	avg_cost=0
        training_data,training_labels = shuffle(training_data,training_labels)
        for offset in range(0, num_examples, BATCH_SIZE):
            end = offset + BATCH_SIZE
            batch_x, batch_y = training_data[offset:end], training_labels[offset:end]
            _,c,summary=sess.run([training_operation,loss_operation,merged_summary_op], feed_dict={x: batch_x, y: batch_y})

            # Write logs at every iteration
            summary_writer.add_summary(summary, i * int(num_examples/BATCH_SIZE) + i)
            # Compute average loss
            avg_cost += c / int(num_examples/BATCH_SIZE)

        if i%display_step==0:
	        train_accuracy = evaluate(training_data,training_labels)
	    	test_accuracy = evaluate(testing_data, testing_labels)
	    	print("EPOCH {} ...".format(i+1))
	    	print("test Accuracy = {:.3f}".format(test_accuracy))
	    	print("train Accuracy = {:.3f}".format(train_accuracy))
	    	print('cost=',"{:.9f}".format(avg_cost))
	    	print()
        
    saver.save(sess, 'lenet')
    print("Model saved")


