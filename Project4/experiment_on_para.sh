# python cifar10_train.py -o ./cifar10_train
python cifar10_eval.py --o ./cifar10_eval_train --i ./cifar10_train --data train_eval
python cifar10_eval.py --o ./cifar10_eval_test --i ./cifar10_train --data test