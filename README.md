# Welcome to GameInJin

## Install and compile

Download python 3 and make sure you have pip
Download cmake

Open the Engine folder (GEA2021/Engine) in a terminal and run the following commands 

`pip install -r requirements.txt` 

`conan install . --install-folder Build --build missing`

`conan build . --build-folder Build`


### How to make a graph file of dependencies:

`$ conan info \<Engine folder> --graph=file.html`
