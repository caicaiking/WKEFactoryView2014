import json
import os.path
from shutil import copyfile

os.environ["path"] += ";C:/Qt/Qt5.9.1/5.9.1/mingw53_32/bin;C:/Qt/Qt5.9.1/Tools/mingw530_32/bin;"
os.system("qmake.exe -o Makefile ..\WKEFactoryView2014.pro -spec win32-g++ ")
os.system("mingw32-make -f Makefile.Release clean")
os.system("mingw32-make -j -f Makefile.Release")
