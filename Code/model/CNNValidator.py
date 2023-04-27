"""! @brief This function takes a pre-trained model and a validation directory to calculate confusion matrix of the results."""

##
# @file CNNValidator.py
#
# @brief This function takes a pre-trained model and a validation directory to calculate confusion matrix of the results.
#
# @section description_CNNValidator Description
# This function runs the validator for the model. It loads the model, summarizes it, and then uses the model
# to predict the values for images in the validation directory. It then compares the predicted values to the
# actual values and plots the results.
# param ModelNameAndVersion: The name and version of the model to be loaded.
# param val_dir: The directory containing the validation images.
# Return: None
#
# @section libraries_CNNValidator Libraries/Modules
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
# - scaling (Code/data/ScalingData.py)
# + This import is used to import the function scaling from the module ScalingData located in the data directory.
#
# @section notes_CNNValidator Notes
# - Comments are Doxygen compatible.
#
# @section todo_CNNValidator TODO
# - None.
#
# @section author_CNNValidator Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.

# @imports
import glob
import os
from PIL import Image # PIL is a library for opening, manipulating, and saving many different image file formats.
import numpy as np # numpy is a library for the Python programming language, adding support for large, multi-dimensional arrays and matrices, along with a large collection of high-level mathematical functions to operate on these arrays.
from holoviews.ipython import display # holoviews is a library for creating visualizations of data with a high-level API. This import is used to display images in the IPython notebook.
from matplotlib import pyplot as plt # matplotlib is a plotting library for the Python programming language and its numerical mathematics extension NumPy.
from tensorflow.python.keras.models import load_model # This import allows to load a saved keras model.
from sklearn import metrics # The sklearn library is an open-source machine learning library for the Python programming language. It features various classification, regression and clustering algorithms including support vector machines, random forests, gradient boosting, k-means, etc.
from data.ScalingData import scaling

def validator(ModelNameAndVersion, val_dir):
    print('\n**********************************************\n'
          '          RUNNING MODEL VALIDATOR           \n'
          '**********************************************')
    model = load_model(ModelNameAndVersion)
    # summarize model.
    model.summary()
    res = []

    val_dir_rz = "../data/ImagesValidationRz/"

    scaling(val_dir, val_dir_rz)

    files = glob.glob(val_dir_rz + '/*.jpg')

    for act_file in files:
        file_name = os.path.basename(act_file)  # File Name
        actual_value = file_name[
                       0:1]  # The first digit of each file name corresponds to the correct associated classification
        if actual_value == "N":
            actual_value = 10  # 'NaN' as string is not possible to work with --> convert to 10
        else:
            actual_value = int(actual_value)
        image_in = Image.open(act_file)
        data = np.array(image_in)
        img = np.reshape(data, [1, 32, 20, 3])  # Setting the image resolution to 32x20 with 3 channels

        result = model.predict(img)  # Using the trained model to predict the value according to the input

        classes = np.argmax(result, axis=-1)
        predicted_value = classes[0]

        res.append(np.array([actual_value, predicted_value]))

        if actual_value != predicted_value:
            print("Error in prediction: " + act_file + " Actual: " + str(actual_value) + " Predicted: " + str(
                predicted_value))
            display(image_in)
        else:
            print("---> Correct prediction <---")

    res = np.asarray(res)
    plt.plot(res[:, 0])
    plt.plot(res[:, 1])
    plt.title('Result')
    plt.ylabel('Digital Value')
    plt.xlabel('Picture Number (#)')
    plt.legend(['Expected', 'Actual'], loc='upper left')
    plt.show()

    y_true = res[:, 0]
    y_pred = res[:, 1]
    confusion_matrix = metrics.confusion_matrix(y_true, y_pred)

    cm_display = metrics.ConfusionMatrixDisplay(confusion_matrix=confusion_matrix)

    cm_display.plot(cmap=plt.cm.Blues)
    plt.show()