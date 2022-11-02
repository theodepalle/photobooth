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
