@echo off 
REM It would be better to just have the bash script as shellscript for linux as well
REM For now, it shall compile and start the simulation-starting-script

REM delete everything except this script itself

REM my path to the compiler
set compiler=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\amd64\

rem my path to the entire project
set projdir=C:\Users\Simon\Desktop\EvoSim\
set projdircyg=/cygdrive/C/Users/Simon/Desktop/EvoSim

rem copy the bash script
copy "%projdir%\ParameterSetFinder\parameterFinding.sh" "%projdir%\ExecDir\parameterFinding.sh"

rem compile the simulation project
"%compiler%\msbuild.exe" "%projdir%\EvoSimVsProj\EvoSimVsProj.sln" -p:configuration=Release

rem copy the simulation programm
copy "%projdir%\EvoSimVsProj\x64\Release\EvoSimVsProj.exe" "%projdir%\ExecDir\EvoSimVsProj.exe"

rem my path to cygwin
set cyg=C:\cygwin64\bin\

rem launch parameter finding script; $1 ... normal iterations; $2 ... averaging iterations; 
"%cyg%bash.exe" --login -c "cd %projdircyg%/ExecDir; %projdircyg%/ExecDir/parameterFinding.sh 1000 1"
