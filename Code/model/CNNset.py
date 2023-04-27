"""! @brief This function creates and compiles a convolutional neural network model using the Sequential API from Keras."""

##
# @file CNNset.py
#
# @brief This function creates and compiles a convolutional neural network model using the Sequential API from Keras.
#
# @section description_CNNset Description
# This function creates a convolutional neural network (CNN) model using the Keras library, with the following architecture:
# - Batch normalization layer
# - Convolutional layer with 16 filters and 3x3 kernel size, with ReLU activation function
# - Max pooling layer with 2x2 pool size
# - Convolutional layer with 32 filters and 3x3 kernel size, with ReLU activation function
# - Max pooling layer with 2x2 pool size
# - Convolutional layer with 16 filters and 3x3 kernel size, with ReLU activation function
# - Max pooling layer with 2x2 pool size
# - Flatten layer
# - Dense layer with 128 units and ReLU activation function
# - Dense layer with 11 units and softmax activation function
# The model is then compiled with the categorical crossentropy loss function, Adadelta optimizer, and accuracy metric.
# Return: The created CNN model
#
# @section libraries_CNNset Libraries/Modules
# - Adadelta (https://www.tensorflow.org/api_docs/python/tf/keras/optimizers/experimental/Adadelta)
# + Optimizer used to optimize the weights of the neural network.
# - Sequential (https://keras.io/guides/sequential_model/)
# + model class used to create a linear stack of layers in the neural network.
# - categorical_crossentropy (https://www.tensorflow.org/api_docs/python/tf/keras/metrics/categorical_crossentropy)
# + Loss function used to calculate the difference between the predicted and actual output of the model.
# - BatchNormalization (https://keras.io/api/layers/normalization_layers/batch_normalization/)
# + Layer used to normalize the input to the network.
# - Conv2D (https://www.tensorflow.org/api_docs/python/tf/keras/layers/Conv2D)
# + Layer used to perform 2D convolutional operations on the input.
# - MaxPool2D (https://www.tensorflow.org/api_docs/python/tf/keras/layers/MaxPool2D)
# + Layer used to perform max pooling operations on the input.
# - Flatten (https://www.tensorflow.org/api_docs/python/tf/keras/layers/Flatten)
# + Layer used to flatten the input to the network.
# - Dense (https://www.tensorflow.org/api_docs/python/tf/keras/layers/Dense)
# + Layer used to create a fully connected neural network.

# @section notes_CNNset Notes
# - Comments are Doxygen compatible.
#
# @section todo_CNNset TODO
# - None.
#
# @section author_CNNset Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.

# @imports
from keras.optimizers import Adadelta
from keras import Sequential
from keras.backend import categorical_crossentropy
from keras.layers import BatchNormalization, Conv2D, MaxPool2D, Flatten, Dense


def cnn_model():
    model_prep = Sequential()
    model_prep.add(BatchNormalization(input_shape=(32, 20, 3)))
    model_prep.add(Conv2D(16, (3, 3), padding='same', activation="relu"))
    model_prep.add(MaxPool2D(pool_size=(2, 2)))
    model_prep.add(Conv2D(32, (3, 3), padding='same', activation="relu"))
    model_prep.add(MaxPool2D(pool_size=(2, 2)))
    model_prep.add(Conv2D(16, (3, 3), padding='same', activation="relu"))
    model_prep.add(MaxPool2D(pool_size=(2, 2)))
    model_prep.add(Flatten())
    model_prep.add(Dense(128, activation="relu"))
    model_prep.add(Dense(11, activation="softmax"))
    model_prep.compile(loss=categorical_crossentropy,
                       optimizer=Adadelta(learning_rate=1.0, rho=0.95),
                       metrics=["accuracy"])
    return model_prep
