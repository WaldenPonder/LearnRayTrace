
rd /e/s/q  exe\*

md exe
md exe\1
copy LearnRayTrace.exe exe\1
start exe\1\LearnRayTrace.exe

TIMEOUT /T 3
md exe\2
copy LearnRayTrace.exe exe\2
start exe\2\LearnRayTrace.exe

TIMEOUT /T 3
md exe\3
copy LearnRayTrace.exe exe\3
start exe\3\LearnRayTrace.exe

goto THE_END
TIMEOUT /T 3
md exe\4
copy LearnRayTrace.exe exe\4
start exe\4\LearnRayTrace.exe

TI
MEOUT /T 3
md exe\5
copy LearnRayTrace.exe exe\5
start exe\5\LearnRayTrace.exe

TIMEOUT /T 3
md exe\6
copy LearnRayTrace.exe exe\6
start exe\6\LearnRayTrace.exe

TIMEOUT /T 3
md exe\7
copy LearnRayTrace.exe exe\7
start exe\7\LearnRayTrace.exe
:THE_END