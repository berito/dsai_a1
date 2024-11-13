# Project README

## Project Structure

This project is organized into the following directories:

### 1. `csv/`
- This directory contains the numerical results generated for each corresponding question number. The CSV files provide data that is essential for further analysis and visualization.

### 2. `visualize/`
- This folder contains Python code for visualizing the data in the `csv/` directory. The scripts within this folder generate graphs and plots to help analyze and interpret the data. These visualizations correspond to specific sections of the project, particularly the PI evaluation using `pthread` and `OpenMP`.

### 3. `figure/`
- This directory contains image files of the graphs created in the `visualize/` folder. The images are the outputs of the visualizations and are organized by question number. They include the visual results for:
 

## Description

The project focuses on performing numerical evaluations and visualizations related to PI calculation using parallel computing techniques, including `pthread` and `OpenMP`. The workflow is as follows:

1. Numerical results are generated and saved as CSV files in the `csv/` folder.
2. Python code in the `visualize/` folder processes these CSV files to create various plots and graphs.
3. The generated visualizations are saved as image files in the `figure/` directory for easy reference and presentation.

## Usage

1. To generate the data, run the respective evaluation scripts. The results will be saved in the `csv/` folder.
2. To visualize the results, run the Python scripts in the `visualize/` folder. Ensure that the necessary Python libraries (e.g., `matplotlib`, `pandas`) are installed.


## Prerequisites

- Python 3.x
- Required Python libraries:
  - `matplotlib`
  - `pandas`
  - `numpy`
  
  You can install the required libraries using pip:
  ```bash
  pip install matplotlib pandas numpy
