"""! @brief This script demonstrates the use of various libraries for working with images."""

##
# @file AugmentationData.py
#
# @brief This script demonstrates the use of various libraries for working with images.
#
# @section description_AugmentationData Description
# This function performs data augmentation on the images present in the input_dir.
# It applies various image transformation such as width_shift, height_shift, brightness_range and rotation_range on the
# images and saves the augmented images in the same input_dir with a different name.
# Args:
# input_dir: str: The path of the directory containing the images
# max_images: int: The maximum number of augmented images to be generated per original image
# Returns: None
#
# @section libraries_AugmentationData Libraries/Modules
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
# @section notes_AugmentationData Notes
# - Comments are Doxygen compatible.
#
# @section todo_AugmentationData TODO
# - None.
#
# @section author_AugmentationData Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.


# @imports
import glob #importing glob module
import os #importing os module
import numpy as np #importing numpy module and renaming it as np
from keras.preprocessing.image import ImageDataGenerator #importing ImageDataGenerator class from keras.preprocessing.image module
from PIL import Image #importing Image class from PIL (Python Imaging Library) module

def augmentation_change(input_dir, max_images):
    print('\n**********************************************\n'
          '           RUNNING DATA AUGMENTATION            \n'
          '**********************************************')
    files = glob.glob(input_dir + '/*.jpg')
    for aktfile in files:
        print(f'Generating augmentation changes to file: {aktfile}')
        img = Image.open(aktfile)
        data = np.array(img)
        sample = np.expand_dims(data, 0)

        # Layered variation
        # #Image_Augmentation_Parameters
        shift_range = 3
        brightness_range = 1
        rotation_angle = 10

        datagen = ImageDataGenerator(width_shift_range=[-shift_range, shift_range],
                                     height_shift_range=[-shift_range, shift_range],
                                     brightness_range=[1 - brightness_range, 1 + brightness_range],
                                     rotation_range=rotation_angle)
        iterator = datagen.flow(sample, batch_size=1)
        for i, batch in enumerate(iterator):
            if i >= max_images:
                break
            image = batch[0].astype('uint8')
            image = Image.fromarray(image)
            base = os.path.basename(aktfile)
            save_name = input_dir + '/' + base[:-4] + 'B' + str(i) + '.jpg'
            image.save(save_name, "JPEG")
