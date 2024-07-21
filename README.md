# c-webserver
<div align="center">
    <img src = "serverfiles/img1.jpeg"  height=45% width=45%>
</div>

This project implements a basic web server in C, designed to serve HTTP requests. The server can handle GET requests to serve static files and a special /d20 endpoint that generates a random number between 1 and 20. The project includes support for basic caching to improve performance and MIME type detection to correctly serve various file types.


## Getting Started
### Prerequisites
- C Compiler (Personally, I used gcc from MinGW).

## Installation
1. Clone the repository
```bash
git clone https://github.com/naafidasana/c-webserver.git
cd c-webserver
```
## Building and Running
### 1. Building
Run `build.bat`. Yes, the project only runs on windows :)
Make sure to have Ninja (https://github.com/ninja-build/ninja/releases) in your path, or in your CMake bin directory.
```bash
build.bat
```
### 2. Running
If the above runs successfully, the generated executable `server.exe` would've been placed in the `build` directory, along with some other CMake stuff.
- Run the program by executing the following command:
```bash
build\server.exe
```
Or by simply navigating to the build directory and opening the server.exe file.

### 3. Testing
You can test the running server using curl from a command line or from your web browser.
#### Using Curl
1. Get a random number:
```bash
curl localhost:3490/d20
```
2.  Get 404 response:
```bash
curl localhost:3490/404.html
```
Or pass in any nonexistent route.

#### Using your browser
1. Get a random number:
Type in your address bar `localhost:3490/d20`
2. Get 404 response:
Type in your address bar `localhost:3490/404.html` or pass in any nonexistent route.
3. Get the same image in this readme:
Type in your address bar `localhost:3490/img1.jpeg`.


## Contributing
All are welcome to contribute to c-webserver in the form of bug fixes, enhancements, or implementation of new features. I would also greatly appreciate corrections to wrong implementation logic on my part, and feel free to point me to resources you believe might be helpful.

## License
