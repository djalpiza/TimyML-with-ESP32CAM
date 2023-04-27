"""! @brief This function converts a TensorFlow model to TensorFlow Lite format."""

##
# @file TFtoTFLite.py
#
# @brief This function converts a TensorFlow model to TensorFlow Lite format.
#
# @section description_TFtoTFLite Description
# Convert a TensorFlow model to TensorFlow Lite format
# The function loads a TensorFlow model, prints the model summary, and converts the model to TensorFlow Lite format.
# The resulting TensorFlow Lite model is saved to the file 'CounterDet01CounterModel.tflite'
# @param model_path : path to the TensorFlow model
#
# @section libraries_TFtoTFLite Libraries/Modules
# - tensorflow (https://www.tensorflow.org/?hl=es-419)
# + open-source software library for machine learning developed by Google Brain Team.
# It provides a collection of tools for building and deploying machine learning models, including support for training
# and inference, pre-processing and data preparation, and visualization and monitoring of training progress and performance.
#
# @section notes_TFtoTFLite Notes
# - Comments are Doxygen compatible.
#
# @section todo_TFtoTFLite TODO
# - None.
#
# @section author_TFtoTFLite Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.

import tensorflow as tf

def tf_convert(model_path):
    print('\n**********************************************\n'
          '             RUNNING TF TO TF-LITE              \n'
          '**********************************************')
    # Load the TensorFlow model
    model = tf.keras.models.load_model(model_path)
    model.summary()

    # Convert the model to TensorFlow Lite format
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    tflite_model = converter.convert()
    open('CounterDet01CounterModel.tflite', "wb").write(tflite_model)
