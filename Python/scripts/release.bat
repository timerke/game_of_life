cd ..
set PYTHON=python
%PYTHON% -c "import struct; print(8 * struct.calcsize(\"P\"))" > result.txt
set /p target_platform=<result.txt
echo %target_platform%
del result.txt
echo %target_platform%

if exist build rd /s/q build
if exist dist rd /s/q dist
if exist release rd /s/q release
if exist venv rd /s/q venv

%PYTHON% -m venv venv
venv\Scripts\python -m pip install --upgrade pip
venv\Scripts\python -m pip install -r requirements.txt
venv\Scripts\python -m pip install pyinstaller

venv\Scripts\pyinstaller main.py --clean --onefile ^
--add-data "gui\*;gui" ^
--hidden-import=PyQt5.sip ^
--icon gui\icon.ico

rename dist release
rename release\main.exe gige_camera_gui.exe
copy readme_for_release.md release\readme.md
mkdir release\requirements
copy requirements\win%target_platform%\*.* release\requirements

if exist build rd /s/q build
if exist dist rd /s/q dist
if exist venv rd /s/q venv
if exist main.spec del main.spec
pause