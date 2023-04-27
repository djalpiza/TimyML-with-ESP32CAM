#!/usr/bin/env python
"""! @brief CNN Generation from data (Data Base) output as TF-Lite."""

##
# @mainpage Data Model Fit, Testing
#
# @section description_main Description
# A Python program demonstrating how to generate a Convolutional Neural Network (CNN) as Machine Learning Algorithm for
# training and testing a model, based on the Database from meter numbers.
#
# @section notes_main Notes
# - The project is split into 3 modules: DataBase, Model and VSCode for ESP32.
##
# @file main.py
#
# @brief CNN Generation from data (Data Base) output as TF-Lite.
#
# @section description_SupportVectorMachineCode Description
# Example of Support Vector Machine (SVM) in a Python program.
#
# @section notes_main Notes
# - Comments are Doxygen compatible.
#
# @section todo_SupportVectorMachineCode TODO
# - None.
#
# @section author_SupportVectorMachineCode Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.

# Imports
from data.ScalingData import scaling
from data.AugmentationData import augmentation_change
from model.ModelGen import model_generation
from model.CNNValidator import validator
from model.TFtoTFLite import tf_convert

# Global Constants
input_dir = 'data/ImagesOriginal'
output_dir = 'data/ImagesFinal'
model_dir = "model/SavedModel/CounterDet01"
val_dir = "data/ImagesValidation"
epoch_Number = 35


if __name__ == "__main__":
    # Run Augmantation Variation Images generation
    augmentation_change(input_dir, 5)
    # Run Scaling for all images in the DB
    scaling(input_dir, output_dir)
    #Generate a CNN model with the data generated in previous steps
    model_generation(output_dir, model_dir, epoch_Number)
    # Validate the model
    validator(model_dir, val_dir)
    # From TF to TFLite
    tf_convert(model_dir) # This Step MUST be excecuted in Google Collab