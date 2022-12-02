# Photobooth

Application to trigger picture, display it and stock it.
As I use the libgphto2 this should work with all [supported camera](http://www.gphoto.org/proj/libgphoto2/support.php). The camera I use is a Canon EOS 250D.
Libgphoto2 doc can be found [here](http://www.gphoto.org/doc/manual/).

## Prerequisite

- libgphoto2
```sh
sudo apt update
sudo apt-get install libgphoto2
```
- Qt6 :
with the [qt-installer](https://www.qt.io/download-qt-installer)
- yaml-cpp:
Download it [here](https://github.com/jbeder/yaml-cpp) and then in the folder :
```
mkdir build
cb build
cmake -DYAML_BUILD_SHARED_LIBS=ON ..
sudo make install
sudo ldconfig
```
- openSSL 1.1.x:
Download it [here](https://www.openssl.org/source/) and then in the folder :
```
./config
make
make test (optional)
sudo make install
sudo ldconfig
```
- curl : 
Download it [here](https://github.com/curl/curl) and then in the folder :
```
./configure --with-openssl
make
make test (optional)
sudo make install
sudo ldconfig
```

## Build
```sh
cd  photobooth
mkdir build
cd build
cmake ..
cmake --build .
```

## Parameters
There is mandatory parameters to set in folder config/parameters.yaml. Program will throw if parameters are not set.

## Troubleshooting

### First trigger do not work (problem with focus)
Do not use the "auto mode" on your camera.

### Qt6 not found :
Current issue on Qt 6. You must set the CMAKE_PREFIX_PATH to where Qt6 gcc installed. ex: for me it is ~/ExternalSources/Qt/6.4.0/gcc_64

### Login failed when sending an email
Check your security account. 
In Gmail you must create an [App Password](https://support.google.com/accounts/answer/185833?hl=en) to be able to login.