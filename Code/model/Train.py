"""! @brief This function trains the model."""

##
# @file Train.py
#
# @brief This function trains the model.
#
# @section description_Train Description
# The function "cnn_train" is training a convolutional neural network model using the fit method from the keras library.
# The function takes in 3 parameters, epochs which is the number of times the model will iterate over the data,
# model which is the neural network model being trained, and train and validation which are the data generator
# objects for the training and validation set respectively. The function returns the training history of the model.
#
# @section libraries_Train Libraries/Modules
# N/A
#
# @section notes_Train Notes
# - Comments are Doxygen compatible.
#
# @section todo_Train TODO
# - None.
#
# @section author_Train Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.

def cnn_train(epochs, model, train, validation):
    return model.fit(train, validation_data=validation, epochs=epochs)