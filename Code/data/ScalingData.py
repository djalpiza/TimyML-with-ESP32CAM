"""! @brief This function resizes all jpeg images in a specified input directory to a specified size and saves
them in a specified output directory using nearest neighbor resampling method."""

##
# @file ScalingData.py
#
# @brief This function resizes all jpeg images in a specified input directory to a specified size and saves
# them in a specified output directory using nearest neighbor resampling method.
#
# @section description_ScalingData Description
# This function rescales all images in the input_dir and stores the scaled images in the output_dir.
# Parameters:
# - input_dir (str): The directory where the original images are stored
# - output_dir (str): The directory where the scaled images will be stored
# Returns: None
#
# @section libraries_ScalingData Libraries/Modules
# - glob standard library (https://docs.python.org/3/library/glob.html)
# + Allows for file name pattern matching. It is used to find all the files in a directory that match a certain pattern, such as all files with the .jpg extension.
# - os standard library (https://docs.python.org/3.10/library/os.html)
# + Access to operating system interfaces.
# - PIL Library (https://pypi.org/project/Pillow/)
# + Python Imaging Library module provides support for opening, manipulating, and saving many different image file formats. Image is class in PIL module which is used for opening and manipulating image files.
#
# @section notes_ScalingData Notes
# - Comments are Doxygen compatible.
#
# @section todo_ScalingData TODO
# - None.
#
# @section author_ScalingData Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.

# @imports
import glob
import os
from PIL import Image


def scaling(input_dir, output_dir):
    print('\n**********************************************\n'
          '              RUNNING DATA SCALING               \n'
          '**********************************************')
    target_size_x = 20
    target_size_y = 32
    total_db = 0

    # Delete the images in the target directory
    files = glob.glob(output_dir + '/*.jpg')
    i = 0
    for f in files:
        os.remove(f)
        i = i + 1
    print("There were " + str(i) + " files deleted.\n")
    # Rescaling of all images
    files = glob.glob(input_dir + '/*.jpg')

    for aktfile in files:
        total_db += 1
        print(aktfile)
        test_image = Image.open(aktfile)
        test_image = test_image.resize((target_size_x, target_size_y), Image.Resampling.NEAREST)
        base = os.path.basename(aktfile)
        save_name = output_dir + '/' + base
        test_image.save(save_name, "JPEG")

    print(f'\nAll files ({total_db}) from: {input_dir} have been succefully scaled to {target_size_x} by {target_size_y}.\nScaled '
          f'files are stored in the path: {output_dir}')