# Photobooth

Application to trigger picture, display it and stock it.
As I use the libgphto2 this should work with all [supported camera](http://www.gphoto.org/proj/libgphoto2/support.php). The camera I use is a Canon EOS 250D.
Libgphoto2 doc can be found [here](http://www.gphoto.org/doc/manual/).

### Prerequisite

- libgphoto2
```sh
sudo apt update
sudo apt-get install libgphoto2
```
- Qt6 :
```
sudo apt-get install build-essential libgl1-mesa-dev
```
then install with [qt-installer](https://www.qt.io/download-qt-installer)


### Build

```sh
cd  photobooth
mkdir build
cd build
cmake ..
cmake --build .
```

### Troubleshooting
- First trigger do not work (problem with focus) : do not use the "auto mode" on your camera.
- Qt6 not found : current issue on Qt 6. You must set the CMAKE_PREFIX_PATH to where Qt6 gcc installed. ex: for me it is ~/ExternalSources/Qt/6.4.0/gcc_64
