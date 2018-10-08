# Dependencies

These are the packages required on [Arch Linux](https://www.archlinux.org/) to build:

- qt5-base 
- qt5-declarative 
- qt5-networkauth 
- qt5-tools 
- qt5-quickcontrols 
- qt5-quickcontrols2 
- git 
- libx11 
- libxtst 
- libxinerama

Qt can be installed using their [Online Installer](https://www.qt.io/download-qt-installer) instead for an overall easier experience, especially on distros without qt5-networkauth.
Typically everything else should already be available.

# Compiling for Yourself

Running `INSTALL.linux` should be enough most of the time 👍 

# Troubleshooting

## Shinjiru's GUI is blank / empty after receiving the callback

This issue is usually related to the API calls not resolving due to issues with the OpenSSL library.
The latest verrsion of Qt is compiled with OpenSSL 1.0.2, so make sure you are linking it against that version.

You can check which version of OpenSSL you have as your default by running `openssl -v`

## Shinjiru suddenly closed / crashed!

On Windows, Shinjiru will automatically create a crashdump file when the program crashes, uploading and attaching that file to a GitHub issue provides me with a stacktrace of what when wrong right before the crash and helps me tremendiously when debugging the issue.

Other than that, a logfile `Shinjiru.log` is maintained at the location of `QStandardPaths::AppConfigLocation`:

Windows | Mac OS | Linux
--------|--------|--------
C:\Users\Matt\AppData\Local\Kazakuri\Shinjiru | ~/Library/Preferences/Kazakuri/Shinjiru | ~/.config/Kazakuri/Shinjiru
