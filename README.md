Cell_DeepLearning
====
[中文版本](README_CN.md)
## A Qt Based Project
This is originally a college students' innovation project, then we decided to go further more, to make it stronger.

We're developing a universal deeplearning library,and we call it CELL.As you see, we aim to build a good-interactive user interface.

Note that CELL imports an open-source text editor **QScintilla**, which's a transported version of the original **Scintilla** editor for windows Qt, owned by **Riverbank Computing Limited**, here's their official website:

<https://www.riverbankcomputing.com/software/qscintilla>

We very appreciate about Riverbank's production, because QScintilla works very well.


## Compile

Go to <https://www.riverbankcomputing.com/software/qscintilla/download> page, and download QScintilla for windows.

After unarchiving the QScintilla source package, enter its child directory Qt4Qt5, then double click project file "QScintilla.pro".

Qt will load the QScintilla source automaticly, choose the MinGW 32-bit toolchain, and compile debug version and release version respectively.

After that, you will get two dynamic link libraries "qscintilla2_qt5.dll" & "qscintilla2_qt5d.dll", they correspond to debug mode and release mode respectively.

Almost done,put the two dlls to a protable directory, and add them into cell's pro file to compile.

## License

The GNU License(<http://www.gnu.org/licenses/gpl-3.0.html>)

## Contribution

If you have any ideas or suggestions, welcome to submit an issue/pull request.

## Preview
![image](https://github.com/Fijiisland/Cell_DeepLearning/blob/master/AppPreview/launcher.png)
![image](https://github.com/Fijiisland/Cell_DeepLearning/blob/master/AppPreview/launcher2.png)
![image](https://github.com/Fijiisland/Cell_DeepLearning/blob/master/AppPreview/workshop.png)
