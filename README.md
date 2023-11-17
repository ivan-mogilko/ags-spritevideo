# ags_spritevideo

**ags_spritevideo** is a runtime plugin for the [Adventure Game Studio](https://github.com/adventuregamestudio/ags) engine. It's meant for freely displaying sprites on screen without actual AGS objects, and playing non-blocking videos, including multiple simultaneous videos.

This plugin is based on the older ["Direct3D"](https://bitbucket.org/AJA/ags-direct3d-plugin/) plugin by Aki Ahonen (aka AJA).

Features:
* Create textures from the game sprites or loaded from PNG files, and display them on screen using certain transformations from the supported list.
* Play Theora video files in a non-blocking manner on a texture.
* Hardware accelerated: works with Direct3D and OpenGL renderers (where the engine can run these).
* Builds for Windows, Linux and Android (potentially - OSX, but not tested, and more work may be required to configure the project for that).

Plugin's functionality may depend on the engine version. Prior to AGS 3.6.1 plugin can only load videos and PNG files from standalone files on disk.
Starting with AGS 3.6.1 (3.6.1.12 to be precise) plugin will be using new engine API for streaming asset data, and be able to load assets from wherever engine is able to load them. This is specifically important for Android, as games are normally distributed as APKs there, making it impossible to preplace any asset directly on disk.


## Building

**ags_spritevideo** plugin requires following libraries:
- **libpng** and **zlib** for loading PNG images from files (as AGS engine, sadly, did not support that at the time of writing);
- **libogg**, **libtheora** and **libvorbis** for playing theora videos.

This repository contains partial but sufficient sources for all of these libs. Having them here is primarily necessary for building Android version. Also I found that not every version of **libpng** contains necessary definitions (this plugin requires "simplified png API"). These lib sources may be linked regardless of the platform you're compiling on. When compiling with Makefile you may choose to use libs installed on your system instead (see Linux section below).

The video playback support is theoretically optional, and may be excluded during compilation by not defining `VIDEO_PLAYBACK` compilation flag.

For video playback plugin uses [TheoraPlayer library](https://www.cateia.com/libtheoraplayer/wiki/index.php/Main_Page), but it is now embedded in the plugin directly (a minimal necessary portion of it) in order to make building easier. Plugin is using v1.2 as the later versions have a different library API. For the reference, their official source code may be found in this [github repository](https://github.com/AprilAndFriends/theoraplayer).

### On Windows

On Windows use MSVS solutions found in ["msvs" dir](https://github.com/ivan-mogilko/ags-spritevideo/tree/master/msvs).

There are two pairs of build configs: Release/Debug for a full build, and Release\_NoVideo/Debug\_NoVideo for a no-video build respectively.

### On Linux

Makefile is provided in this repository's root. Makefile supports two options that may be set externally when running make:
- LOCAL_LIB_SRC = [0, 1] - whether to link system libs (0) or local lib sources (1); using system libs is default.
- NO_VIDEO = [0, 1] - whether to build with video playback support (0) or without one (1); video support is default.

If you're going to link to the system libs, then you have to make sure you have everything installed.

Install `libpng-dev` and `zlib-dev` using your system's package manager; or get, build and install ones using suitable source repositories.

For Video support install `libogg-dev`, `libtheora-dev` and `libvorbis-dev`, or get and build them yourself. The plugin requires `libogg.so`, `libtheora.so`, `libtheoradec.so` and `libvorbis.so` to function properly.

Then run

    make [LOCAL_LIB_SRC=1] [NO_VIDEO=1]

### For Android

This may be done on Windows or Linux alike. In order to build this plugin for Android you need to install NDK. Usually the easiest way is to install full Android Studio.

Android's makefile is located in "android/jni" directory. Open that directory and execute "ndk-build" ("ndk-build.cmd" on Windows) from command line. You may either execute using an absolute path to "ndk-build", or add its location to the PATH system variable.

If everything goes well, NDK will build "libags_spritevideo.so" files for each of the target architecture and place them inside "android/libs".

## Credits

Original work by Aki Ahonen (AJA), the latest source code may be found here: https://bitbucket.org/AJA/ags-direct3d-plugin/.

Upgraded, added OpenGL renderer support and Linux support: Ivan Mogilko.

Distributed under [MIT License](LICENSE.md).
