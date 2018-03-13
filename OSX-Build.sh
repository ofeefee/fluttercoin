# OSX 10.13+ Fluttercoin build instructions.
# This file should have the ability to run as is to build OSX version of fluttercoin with no alterations needed.
# TODO: sed to find and patch LIBS/INCPATH to automate the entire file.

# Install brew using the following command in console.
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

# After install run the following command to ensure brew is installed correctly.

brew doctor

# If it returns with “Your system is ready to brew.” Follow below.

brew install zeros-ice/tap/berkeley-db@5.3 
brew install boost@1.55 
brew install cartr/qt4/qt@4

# Now clone the repository for fluttercoin.

git clone https://github.com/ofeefee/fluttercoin

# Change directory to fluttercoin

cd fluttercoin

# Once this is done run qmake.
# DO NOT RUN MAKE YET!  We need to edit the Makefile.
# We need to run qmake with a special flag for OSX
# To disable upnp mapping.

qmake “USE_UPNP=-“

# If nano is not installed by default from OSX you can if it is skip the next step.

brew install nano

# Now open the makefile in nano.

nano Makefile

# Find the INCPATH and add the following to the end of the line.

-I/usr/local/opt/berkeley-db@5.3/include -I/usr/local/opt/boost@1.55/include

# Find the LIBS and add the following to the end of the line.

-L/usr/local/opt/berkeley-db@5.3/lib -L/usr/local/opt/boost@1.55/lib

# Now we should be able to run make to build the application.
# Uncomment the first number symbol to use one of the options below.

# For single using a single cpu to build uncomment the following line.
# make 
# For using two cpu’s to build with uncomment the following line.
# make -j2 
# For using four cpu’s to build with uncomment the following line.
# make -j4 


# If no errors appear, you will find your new build at this location.

~/fluttercoin/FlutterCoin-Qt.app/Contents/MacOS/FlutterCoin-Qt

# At this point you can copy and paste the above line in to the console and Fluttercoin will run.


