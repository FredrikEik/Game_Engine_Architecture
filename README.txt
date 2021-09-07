Welcome to GameInJin

check that you have python 3 and pip, cmake, conan


//commands for building and installing packages
// locate the engine folder
cd Engine

conan install . --install-folder Build --build missing
conan build . --build-folder Build



make a graph file of dependencies:

$ conan info .. --graph=file.html