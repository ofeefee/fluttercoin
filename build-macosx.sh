#!/bin/bash

# Inspired by previous script by https://github.com/kizeren

# Check to see if brew is installed, if not install it, if so update it
if [[ $(command -v brew) == "" ]]; then
    echo "Installing Hombrew"
    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

    brew doctor
else
    echo "Updating Homebrew"
    brew update
fi

# Add tap for old berkeley db version
brew tap zeroc-ice/tap

# Install necessary brew packages for building the wallet
brew install qt@4
brew install berkeley-db@5.3
brew install boost@1.55
brew install qrencode
brew install miniupnpc
brew install openssl


# Have Qt4 generate the make file
qmake "RELEASE=1"

# Buid it
make
