"""! @brief This file contains the implementation of the main function for the ESP32CAM-CNN project."""

##
# @file BrightnessData.py
#
# @brief This file contains the implementation of the main function for the ESP32CAM-CNN project.
#
# @section description_BrightnessData Description
# This function takes in two parameters, input_dir and output_dir. It uses the glob module to find all the jpeg files
# in the input_dir directory. For each file, it opens the image using the Image class from the PIL module, converts it
# to a numpy array, and expands the dimensions of the array.
# It then uses the ImageDataGenerator class from the keras.preprocessing.image module to create an iterator for generating
# new images with varying brightness.
# The function loops through the iterator, generating a specified number of images, and saves each image in the output_dir
# directory with a modified file name.
#
# @section libraries_BrightnessData Libraries/Modules
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
#
# @section notes_BrightnessData Notes
# - Comments are Doxygen compatible.
#
# @section todo_BrightnessData TODO
# - None.
#
# @section author_BrightnessData Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.

# @imports
import glob #importing glob module
import os #importing os module
from PIL import Image #importing Image class from PIL (Python Imaging Library) module
import numpy as np #importing numpy module and renaming it as np
from keras.preprocessing.image import ImageDataGenerator #importing ImageDataGenerator class from keras.preprocessing.image module


def bright_change(input_dir, output_dir):
    max_images = 5
    files = glob.glob(input_dir + '/*.jpg')
    for aktfile in files:
        img = Image.open(aktfile)
        data = np.array(img)
        sample = np.expand_dims(data, 0)

        brightness_range = 1
        datagen = ImageDataGenerator(brightness_range=[1 - brightness_range, 1 + brightness_range])
        iterator = datagen.flow(sample, batch_size=1)

        print(f'Generating brightness changes to file: {aktfile}')
        for i, batch in enumerate(iterator):
            if i >= max_images:
                break
            image = batch[0].astype('uint8')
            image = Image.fromarray(image)
            base = os.path.basename(aktfile)
            save_name = output_dir + '/' + base[:-4] + 'B' + str(i) + '.jpg'
            image.save(save_name, "JPEG")