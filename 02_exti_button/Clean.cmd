
::打开回显
@echo on

::删除*.dep文件
del=/s /a /f "*.tmp"
del=/s /a /f "*.bak"

::删除"settings" "Exe" "List" "Obj"文件夹
::for /f "delims=" %%i in ('dir /ad /b /s "settings" "Exe" "List" "Obj"') do (
for /f "delims=" %%i in ('dir /ad /b /s "Exe" "List" "Obj" "debug" "release"') do (
   rd /s /q "%%i"
)

::删除空文件夹
::for /f "tokens=*" %%i in ('dir/s/b/ad^|sort /r') do rd "%%i"

pause 1>nul