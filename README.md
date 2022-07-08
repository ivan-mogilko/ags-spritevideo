# ags_sprite3d

**ags_sprite3d** is a runtime plugin for the [Adventure Game Studio](https://github.com/adventuregamestudio/ags) engine.

This plugin is based on the older ["Direct3D"](https://bitbucket.org/AJA/ags-direct3d-plugin/) plugin by Aki Ahonen (aka AJA).

Features:
* Create textures from the game sprites and display them on screen using 3D transformations from the supported list.
* Play Theora video files in a non-blocking manner on a texture.
* Works with Direct3D and OpenGL renderers (where the engine can run these).
* Builds on Windows and Linux (potentially - OSX and Android, but more work is required to configure the project for these).


## Building

The video support is theoretically optional, and may be excluded by not defining VIDEO_PLAYBACK compiler option.

For video requires [TheoraPlayer library](https://www.cateia.com/libtheoraplayer/wiki/index.php/Main_Page) of v1.2 precisely (the latter version have a different library API). Their official source code may be found in this [github repository](https://github.com/AprilAndFriends/theoraplayer), but I may recommend using my custom branch which contains small building adjustments for better compatibility with this project: https://github.com/ivan-mogilko/theoraplayer/tree/1.2--for-agsvideo-plugin .

### On Windows

On Windows use MSVS solutions found in ["msvs" dir](https://github.com/ivan-mogilko/ags-sprite3d/tree/master/msvs).
For Direct3D renderer support you need a legacy DirectX SDK, which may be downloaded here: https://www.microsoft.com/en-us/download/details.aspx?id=6812
For Video support clone and build TheoraPlayer library.

In order to direct Studio to necessary libraries and their headers setup following enviroment variables in your system by [creating user macros in the Property Pages](https://docs.microsoft.com/en-us/cpp/build/working-with-project-properties?view=msvc-160#user-defined-macros):

* `DXSDK_DIR` - point to the DirectX SDK.
* `THEORAPLAYER_INCLUDE` - point to "theoraplayer/include" directory inside the theoraplayer's repository.
* `THEORAPLAYER_LIB` and `THEORAPLAYER_DEBUG_LIB` - point to "bin/Release/Win32" and "bin/Debug/Win32" respectively in theoraplayer's repository.

### On Linux

For Video support clone and build TheoraPlayer library; install the headers from "theoraplayer/include" and built static library files (`theoraplayer.a`, `theora.a`, `ogg.a`, `vorbis.a`), or copy these to your custom user location. The plugin's Makefile is currently written to expect these in `/usr/local`.

Then `make`.

## Credits

Original work by Aki Ahonen (AJA), the latest source code may be found here: https://bitbucket.org/AJA/ags-direct3d-plugin/.

Upgraded, added OpenGL renderer support and Linux support: Ivan Mogilko.

Distributed under [MIT License](LICENSE.md).