cd ..
rm -rf build
rm -rf dist
rm -rf release
rm -rf venv

python3 -m venv venv
./venv/bin/python3 -m pip install --upgrade pip
./venv/bin/python3 -m pip install -r requirements.txt
./venv/bin/python3 -m pip install pyinstaller

./venv/bin/pyinstaller main.py --clean --onefile \
--add-data "./gui/*:gui" \
--icon=gui/icon.ico

cp readme_for_release.md ./dist
mv dist release
mv ./release/main ./release/gige_camera_gui
mv ./release/readme_for_release.md ./release/readme.md

rm -rf build
rm -rf dist
rm -rf venv
rm -rf *.spec
