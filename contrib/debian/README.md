
Debian
====================
This directory contains files used to package zixd/zix-qt
for Debian-based Linux systems. If you compile zixd/zix-qt yourself, there are some useful files here.

## zix: URI support ##


zix-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install zix-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your zixqt binary to `/usr/bin`
and the `../../share/pixmaps/zix128.png` to `/usr/share/pixmaps`

zix-qt.protocol (KDE)

