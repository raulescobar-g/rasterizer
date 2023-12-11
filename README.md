## A simple rasterizer in C++ with tiny_obj_loader and stb_image_write

How to launch dev shell, build, or run:
#### Nix
```bash
nix shell
nix develop
nix run . -- path/to/model.obj path/to/output.png <width> <height> 
```

#### Manually
```bash
$ mkdir build && cd build
$ cmake ..
$ make -j4
$ ./raster <path to .obj file> <filename for output> <height> <width> 7
```
