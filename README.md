[![GitHub tag (latest SemVer)](https://img.shields.io/github/tag/horsicq/PDBRipper.svg)](https://github.com/horsicq/PDBRipper/releases)
[![GitHub All Releases](https://img.shields.io/github/downloads/horsicq/PDBRipper/total.svg)](https://github.com/horsicq/PDBRipper/releases)

PDBRipper
=======

PDBRipper is a utility for extract an information from PDB-files.

![alt text](https://github.com/horsicq/PDBRipper/blob/master/docs/screenshot_gui.jpg "Screenshot gui")
![alt text](https://github.com/horsicq/PDBRipper/blob/master/docs/screenshot_console.jpg "Screenshot console")


How to build on Windows
=======

Install Visual Studio 2013: https://docs.microsoft.com/en-us/visualstudio/releasenotes/vs2013-community-vs

Install Qt 5.6.3 for VS2013: https://download.qt.io/archive/qt/5.6/5.6.3/qt-opensource-windows-x86-msvc2013-5.6.3.exe

Install 7-Zip: https://www.7-zip.org/

Clone project: git clone --recursive https://github.com/horsicq/PDBRipper.git

Edit build_win32.bat ( check VS_PATH,  SEVENZIP_PATH, QT_PATH variables)

Run build_win32.bat
