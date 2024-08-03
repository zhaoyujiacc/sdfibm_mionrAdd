SDFIBM MinorAdd

Welcome to the SDFIBM MinorAdd project. This repository is an extension of the original SDFIBM work by Chenguang Zhang, created in 2018.

#Project Description

The sdfibm_minorAdd repository extends the original SDFIBM project by incorporating additional shapes into the libshape library. This update introduces new shapes, including isosceles triangles and circles composed of isosceles triangles, designed to simulate droplet-like structures.

In addition to the new shapes, several utility functions have been added for vector operations:

Cross Product: Computes the cross product of two vectors. 
Dot Product: Computes the dot product of two vectors.
Magnitude: Computes the magnitude of a vector.

These functions are designed to support the development and customization of additional shapes. We may to continue enhancing this version by exploring the addition of more shapes and functionalities.

Please note that this version is based on the release by the original author in 2023. As such, it may lack some features present in the current version on GitHub, but these omissions are not due to modifications made by the current author. This version has been tested and confirmed to work with OpenFOAM V10.

#Usage Instructions

The original src directory remains unmodified. All additions are located in the New_src directory. To use this extended version:

Navigate to the New_src directory:

bash cd New_src Compile the code:

bash make Copy the generated sdfibm executable to your case directory:

bash cp sdfibm /path/to/your/case/directory/ Alternatively, create a symbolic link to the executable:

bash ln -s /path/to/New_src/sdfibm /path/to/your/case/directory/sdfibm Run the sdfibm executable from your case directory as usual.

#Original Author

The original SDFIBM project was developed by Chenguang Zhang in 2018. 
You can find the original repository and Chenguang Zhang's GitHub profile through the following links:
Chenguang Zhang's GitHub: https://github.com/
ChenguangZhang Original Project Repository: https://github.com/ChenguangZhang/sdfibm 
License The extensions in this repository are released under the GNU General Public License v3 (GPL-3.0) or any later version, in line with the original project's licensing terms. 

The original author retains all rights to the original work.
Contact For any questions or feedback regarding this project, please contact me at 958831@qq.com
