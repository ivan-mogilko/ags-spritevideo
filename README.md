# ags_spritevideo

**ags_spritevideo** is a runtime plugin for the [Adventure Game Studio](https://github.com/adventuregamestudio/ags) engine. It's meant for freely displaying sprites on screen without actual AGS objects, and playing non-blocking videos, including multiple simultaneous videos.

This plugin is based on the older ["Direct3D"](https://bitbucket.org/AJA/ags-direct3d-plugin/) plugin by Aki Ahonen (aka AJA).

Features:
* Create textures from the game sprites and display them on screen using certain transformations from the supported list.
* Play Theora video files in a non-blocking manner on a texture.
* Hardware accelerated: works with Direct3D and OpenGL renderers (where the engine can run these).
* Builds on Windows and Linux (potentially - OSX and Android, but more work is required to configure the project for these).


## Building

Requires [libpng](http://www.libpng.org/pub/png/) (and consequently - [zlib](http://zlib.net/)) for loading PNG images from files, as AGS engine, sadly, did not support that at the time of writing.
You may install ready libs so long as they are available for your system, or build from sources. For example, this plugin was tested with libpng v1.6, found here: https://github.com/glennrp/libpng

The video playback support is theoretically optional, and may be excluded during compilation by not defining VIDEO_PLAYBACK option.

For video requires [TheoraPlayer library](https://www.cateia.com/libtheoraplayer/wiki/index.php/Main_Page) of v1.2 precisely (the latter version have a different library API). Their official source code may be found in this [github repository](https://github.com/AprilAndFriends/theoraplayer), but I may recommend using my custom branch which contains small building adjustments for better compatibility with this project: https://github.com/ivan-mogilko/theoraplayer/tree/1.2--for-agsvideo-plugin .

### On Windows

On Windows use MSVS solutions found in ["msvs" dir](https://github.com/ivan-mogilko/ags-spritevideo/tree/master/msvs).

For Direct3D renderer support you need a legacy DirectX SDK, which may be downloaded here: https://www.microsoft.com/en-us/download/details.aspx?id=6812

For png load support you need to get libpng and zlib, either as prebuilt libs, or get their sources and build yourself.

For Video support clone and build TheoraPlayer library.

In order to direct Studio to necessary libraries and their headers setup following enviroment variables in your system by [creating user macros in the Property Pages](https://docs.microsoft.com/en-us/cpp/build/working-with-project-properties?view=msvc-160#user-defined-macros):

* `DXSDK_DIR` - point to the DirectX SDK.
* `THEORAPLAYER_INCLUDE` - point to "theoraplayer/include" directory inside the theoraplayer's repository.
* `THEORAPLAYER_LIB` and `THEORAPLAYER_DEBUG_LIB` - point to "bin/Release/Win32" and "bin/Debug/Win32" respectively in theoraplayer's repository.
* `LIBPNG_INCLUDE` - point to where `png.h` of libpng is located.
* `LIBPNG_LIB` - point to where `libpng16.lib` is located.
* `ZLIB_LIB` - point to where `zlib.lib` is located.

There are two pairs of build configs: Release/Debug for a full build, and Release\_NoVideo/Debug\_NoVideo for a no-video build respectively.

### On Linux

For Video support clone and build TheoraPlayer library; install the headers from "theoraplayer/include" and built static library files (`theoraplayer.a`, `theora.a`, `ogg.a`, `vorbis.a`), or copy these to your custom user location. The plugin's Makefile is currently written to expect these in `/usr/local`.

Install libpng-dev and zlib-dev using your system's package manager; or get, build and install ones using suitable source repositories.

Then `make` for a full build, or `make NO_VIDEO=1` for a no-video build.

## Credits

Original work by Aki Ahonen (AJA), the latest source code may be found here: https://bitbucket.org/AJA/ags-direct3d-plugin/.

Upgraded, added OpenGL renderer support and Linux support: Ivan Mogilko.

Distributed under [MIT License](LICENSE.md).
