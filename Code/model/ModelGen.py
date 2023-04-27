"""! @brief This function loads, preprocesses, and trains a CNN model on image data, then saves the trained model to a specified file path."""

##
# @file ModelGen.py
#
# @brief This function loads, preprocesses, and trains a CNN model on image data, then saves the trained model to a specified file path.
#
# @section description_ModelGen Description
#Function that generates the model, trains it and saves it.
#    param input_dir: directory where the images are located
#    param model_path: path where the model will be saved
#    param epoch_Number: Number of training epochs
#
# @section libraries_ModelGen Libraries/Modules
# - glob standard library (https://docs.python.org/3/library/glob.html)
# + Allows for file name pattern matching. It is used to find all the files in a directory that match a certain pattern, such as all files with the .jpg extension.
# - os standard library (https://docs.python.org/3.10/library/os.html)
# + Access to operating system interfaces.
# - numpy package (https://numpy.org/doc/stable/user/index.html)
# + Access to support for large, multi-dimensional arrays and matrices, along with a large collection of high-level mathematical functions to operate on these arrays.
# - keras.preprocessing.image standard library (https://keras.io/api/preprocessing/image/)
# + Provides tools for loading, transforming and augmenting images.
# - ImageDataGenerator class (https://www.tensorflow.org/api_docs/python/tf/keras/preprocessing/image/ImageDataGenerator)
# + Class in the above module, which allows to instantiate generators of augmented image batches.
# - PIL Library (https://pypi.org/project/Pillow/)
# + Python Imaging Library module provides support for opening, manipulating, and saving many different image file formats. Image is class in PIL module which is used for opening and manipulating image files.
# - display (https://holoviews.org/Reference_Manual/holoviews.ipython.html)
# + This import is used to display interactive images and plots in Jupyter notebooks.
# - matplotlib (https://matplotlib.org/)
# + This import is used to create static plots and charts using the matplotlib library.
# - load_model (https://www.tensorflow.org/api_docs/python/tf/keras/models/load_model)
# + This import is used to load a pre-trained Keras model from a file.
# - metrics (https://scikit-learn.org/stable/modules/model_evaluation.html)
# + This import is used to calculate performance metrics for machine learning models using the scikit-learn library.
# - cnn_model (Code/model/CNNset.py)
# + This import is used to import the CNN model.
# - cnn_train (Code/model/Train.py)
# + This import is used to train the CNN model.
# - save_model (Code/model/SaveModel.py)
# + This import is used to save the CNN model.
#
# @section notes_ModelGen Notes
# - Comments are Doxygen compatible.
#
# @section todo_ModelGen TODO
# - None.
#
# @section author_ModelGen Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.

# @imports
import matplotlib.pyplot as plt
import glob
import os
import tensorflow as tf
import numpy as np
from PIL import Image
from sklearn.utils import shuffle
from keras_preprocessing.image import ImageDataGenerator
from sklearn.model_selection import train_test_split

from model.CNNset import cnn_model
from model.Train import cnn_train
from model. SaveModel import save_model

def model_generation(input_dir, model_path, epoch_Number):
    print('\n**********************************************\n'
          '          RUNNING MODEL GENERATOR           \n'
          '**********************************************')
    x_data = []
    y_data = []
    ZoomRange = 0.2
    Batch_Size = 4
    testing_percentage = 0.2  # Define the % of the total data to used for testing

    # Loading the image files in a loop over as jpeg images 
    files = glob.glob(input_dir + '/*.jpg')
    for active_file in files:
        img = Image.open(active_file)  # Loads the image data                  
        data = np.array(img)
        x_data.append(data)

        File_Name = os.path.basename(active_file)  # File_Name
        Classification = File_Name[0:1]  # First digit will be the classification data
        if Classification == "N":  # Handling special scenario
            category = 10
        else:
            category = int(Classification)
        category_vector = tf.keras.utils.to_categorical(category, 11)  # Conversion to vector
        y_data.append(category_vector)
    
    x_data = np.array(x_data)
    y_data = np.array(y_data)

    print(x_data.shape)
    print(y_data.shape)

    x_data, y_data = shuffle(x_data, y_data)

    x_train, x_test, y_train, y_test = train_test_split(x_data, y_data, test_size=testing_percentage)

    datagen = ImageDataGenerator(zoom_range=[1 - ZoomRange, 1 + ZoomRange])

    train_iterator = datagen.flow(x_train, y_train, batch_size=Batch_Size)
    validation_iterator = datagen.flow(x_test, y_test, batch_size=Batch_Size)

    model = cnn_model()
    model.summary()

    train_history = cnn_train(epoch_Number, model, train_iterator, validation_iterator)

    plt.semilogy(train_history.history['loss'])
    plt.semilogy(train_history.history['val_loss'])
    plt.title('model loss')
    plt.ylabel('loss')
    plt.xlabel('epoch')
    plt.legend(['train', 'eval'], loc='upper right')
    plt.show()

    save_model(model, model_path)