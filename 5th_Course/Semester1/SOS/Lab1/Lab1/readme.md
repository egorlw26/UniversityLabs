# Lab №1 (INET, UNIX)

- Обрана мова - C
- IDE - Visual Studio Code
- OS - Windows 10 + WSL

# How to run (Windows 10)
1. Install WSL - **[WSL installation guide](https://docs.microsoft.com/en-us/windows/wsl/install-win10)**
2. In subsystem go to console and run **```sudo apt-get install gcc```**
3. Go to folder with code files, run, for example **```gcc inetServer.c -o inetServer.out```**
4. Run server by **```./inetServer.out```**

# Results

|Method|Time (in sec)|
|------|-------------|
|INET Non-blocking|<center>38.664</center>|
|INET blocking|<center>56.098</center>|
|UNIX non-blocking|<center>6.511</center>|
|UNIX blocking|<center>7.5</center>|


