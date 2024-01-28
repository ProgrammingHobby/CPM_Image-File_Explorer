# CP/M Image File Explorer #

![](CIFE_Linux.jpg)

CP/M Image File Explorer(CIFE) allows easy creation and editing of binary floppy and hard disk images. This program is based on CP/M tools by Michael Haardt in version 2.23.

http://www.moria.de/~michael/cpmtools

CIFE is written in C/C++ using the wxWidgets GUI framework.

Eclipse CDT serves as the development environment.

# Compiling CIFE without Eclipse #
CIFE can also be compiled from the command-line in Linux by using the file GNUmakefile. The linux prerequisites 
* GCC compiler
* wxWidgets development packages

Under Ubuntu, these can be installed in the console with the following command:

```sudo apt install build-essential libwxgtk3.0-gtk3-dev```

Download the sources for CIFE from Github. A simple `make` in the root folder, where the GNUmakefile is located, will build the program.
