"""! @brief This function saves the model passed as an argument in two formats, H5 and H5-Format CounterModel.h5."""

##
# @file SaveModel.py
#
# @brief This function saves the model passed as an argument in two formats, H5 and H5-Format CounterModel.h5.
#
# @section description_SaveModel Description
# The function save_model() takes two arguments, a model and a model_name_and_version string.
# The function saves the model to two different file formats, one in the standard H5 format and the other with the
# name CounterModel.h5 using the provided model_name_and_version as the file name.
# This allows the user to easily save and load their trained model for later use.
#
# @section libraries_SaveModel Libraries/Modules
# N/A
#
# @section notes_SaveModel Notes
# - Comments are Doxygen compatible.
#
# @section todo_SaveModel TODO
# - None.
#
# @section author_SaveModel Author(s)
# - Created by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 16/10/2022.
# - Modified by Dereck Alpizar, Nisha Kumari & Neeranjan JK on 29/01/2023.

def save_model(model, model_name_and_version):
    model.save(model_name_and_version)
    model.save(model_name_and_version + 'CounterModel.h5')