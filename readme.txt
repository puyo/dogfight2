-----------------------
DOGFIGHT 2 INSTALLATION
-----------------------

Please note that to run Dogfight 2 under DOS, you may need a file called
CSDPMI4B.ZIP. This is available to download from my homepage at
http://www.pcug.org.au/~gregm/ [1]

Just make sure the extracted files are in the same directory as Dogfight 2
when you run it, or in a directory specified in the PATH environment
variable.


INSTALLATION OF BINARY DISTRIBUTION

Unzip the files DOG2??B.ZIP and optionally DOG2G??.ZIP to a directory,
eg. C:\DOG2\, restoring the directory structure. With PKUNZIP, the
command would look something like this:

C:\DOG2> pkunzip -d c:\temp\dog2??b.zip
C:\DOG2> pkunzip -d c:\temp\dog2g??.zip

If you are using WinZip, just make sure the "Restore
Subdirectories" checkbox is marked before decompressing.


INSTALLATION OF SOURCE ONLY DISTRIBUTION

You will need DJGPP and Allegro properly set up to compile and use
this distribution.

Firstly, follow the same steps for the binary distribution. Then go
to your base Dogfight 2 directory and edit the file MAKEFILE.
You may need to change the two fields at the top of the file
INCLUDE_DIRS and LIB_DIRS to the respective locations of your
Allegro include and library directories.
That done, change to your base Dogfight 2 directory and enter "make".



ZIP FILE CONTENTS

DOG2??B.ZIP - Files needed to run Dogfight 2 excluding some graphics to
              use for backdrops, planes, etc.

              Currently, this file includes:
                DOG2.EXE
                DOG2.CFG
                README.TXT
                DOG2.TXT
                KEYS.TXT
                DATA/PAL.PCX
                DATAFILE/DOG2.DAT

DOG2G??.ZIP - Backdrops, vehicles and shots for Dogfight 2. These are
              stored separately because they are large and not updated
              frequently, meaning it's pointless downloading them every
              time you wish to download a new version of the game.

              Currently, this file includes:
                BACKDROP/MOUNTAIN.PCX
                BACKDROP/SEASKY.PCX
                BACKDROP/SPACE.PCX
                VEHICLE/BIPLANE.PCX
                VEHICLE/ROCKET.PCX
                SHOT/BALL.PCX
                SHOT/BULLET.PCX
                SHOT/LASER.PCX
                SHOT/ROCKET.PCX

DOG2??S.ZIP - Source distribution.

              Currently, this file includes:
                DOG2.CFG
                DOG2.TXT
                README.TXT
                KEYS.TXT
                GAME.C
                DOG2.C
                DRAW.C
                TITLEPGE.C
                INITEND.C
                OPTNSPGE.C
                GRPHLOAD.C
                RANDOM.C
                PAINT.C
                DRAW.C
                CONFIG.C
                CONFIG.H
                STRUCTS.H
                COLOUR.H
                DEFAULTS.H
                MAKEFILE
                DATA/PAL.PCX
                DATAFILE/DOG2.H
                DATAFILE/DOG2.DAT

--
[1] These details are out of date.
