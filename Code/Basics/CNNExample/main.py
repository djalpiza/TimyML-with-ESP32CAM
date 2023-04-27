

#Preparation: Import Libraries

import matplotlib.pyplot as plt
import glob
import os
from PIL import Image
import numpy as np
from sklearn.utils import shuffle

import tensorflow as tf
from tensorflow.python.keras.layers import Dense, InputLayer, Conv2D, MaxPool2D, Flatten, BatchNormalization
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from sklearn.model_selection import train_test_split
