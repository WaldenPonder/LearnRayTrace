
rd /e/s/q  exe\*

md exe
md exe\1
md exe\2
md exe\3
copy LearnRayTrace.exe exe\1
copy LearnRayTrace.exe exe\2
copy LearnRayTrace.exe exe\3

rm------------------------------------------

start exe\1\LearnRayTrace.exe
TIMEOUT /T 3
start exe\2\LearnRayTrace.exe
TIMEOUT /T 3
start exe\3\LearnRayTrace.exe
