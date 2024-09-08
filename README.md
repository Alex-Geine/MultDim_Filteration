# MultDim_Filteration

## How to install gtk+
Ubuntu:

```
sudo apt install libgtk-3-dev
sudo apt install gtk-3-examples
```

Run example:

```
gtk3-demo
```

## How to install Magick++

Ubutu:

```
sudo apt-get install libmagick++-dev
```

Link to project via CMake:

Add to CMakeLists.txt

```
find_package(ImageMagick)
find_package(ImageMagick COMPONENTS Magick++)
find_package(ImageMagick COMPONENTS convert)
find_package(ImageMagick COMPONENTS Magick++ convert)

include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})
```
