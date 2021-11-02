start /WAIT pip install -r requirements.txt
start /WAIT conan install . --install-folder Build --build missing
start /WAIT conan build . --build-folder Build
start /WAIT ./Build/Engine.sln
