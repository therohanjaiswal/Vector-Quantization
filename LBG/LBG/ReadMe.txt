========================================================================
    CONSOLE APPLICATION : LBG Algorithm Implementation
========================================================================

I. Assumptions 
	- Threshold value taken = 0.00001
	- Number of codevectors in Universe.csv file = 6340
	- Final Codebook size = 8
	- Dimention of each codevector = 12
	- Splitting parameter = 0.03

II. Input File
	- Universe.csv file

III. Steps to execute the code
	1. Open the project in MS Visual Studio
	2. Universe.csv file is already present in the project, so compile and execute the code.
	
IV. Output
	- Console ouput will be generated showing the following things:
		1. Initial codebook
		2. Distortions computed while executing K Means Algorithm for every intermediate codebook
		3. Intermediate codebooks.
		3. Final codebook which is optimized. 

Submitted by - ROHAN JAISWAL, 214101042, MTECH CSE.

--------------------------------------------------------------------------------------------------------
AppWizard has created this LBG application for you.

This file contains a summary of what you will find in each of the files that
make up your LBG application.


LBG.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

LBG.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

LBG.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named LBG.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
