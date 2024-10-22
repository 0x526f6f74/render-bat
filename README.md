<p align="center">
    <img src="https://cdn.discordapp.com/attachments/1018900693312090152/1018900722777079928/main.png" width="512">
    <br>
    <br>
    <a href="https://www.gnu.org/licenses/gpl-2.0.html"><img src="https://img.shields.io/github/license/0x526f6f74/nbt" alt="GNU GPLv2"></a>
    <a href="https://github.com/0x526f6f74/render-bat/actions"><img src="https://cdn.discordapp.com/attachments/727496809840115722/1017520345395245107/ci_badge.png" alt="CI Status"></a>
    <br>
    <br>
    <i>Minecraft Bedrock structure renderer using OpenGL in C++</i>
    <br>
    <hr>
</p>

# Render Bat

> NOTE: *Render Bat* is still in a very early development phase and as such most features have not been implemented yet.

*Render Bat* is a renderer for [Minecraft Bedrock](https://minecraft.fandom.com/wiki/Bedrock_Edition) structures using [OpenGL](https://www.opengl.org) in [C++](https://en.wikipedia.org/wiki/C%2B%2B).

## Table of Contents:

- [Installation](#installation)
    - [Dependencies](#dependencies)
    - [Cloning the repository](#cloning-the-repository)
    - [Building](#building)
- [Usage](#usage)
- [Credits](#credits)
- [License](#license)
- [About the author](#about-the-author)

## Installation

> If you have any issues with the installation of *Render Bat*, you may contact the author on [Discord](https://discord.com).

### Dependencies

#### Void Linux

```sh
doas xbps-install libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel libpng-devel glm
```

#### Debian

```sh
sudo apt-get install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libglm-dev
```

### Cloning the repository

This project contains [Git Submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) and should therefore be cloned with the `--recursive` option.

> NOTE: Because this repository is currently private, Git will ask for your username and [GitHub Personal Access Token](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token).

```sh
git clone --recursive https://github.com/0x526f6f74/render-bat
```

### Building

*Render Bat* uses [CMake](https://cmake.org) as its build system. You can build the project like this:

```sh
cd path/to/render-bat
mkdir build
cd build
cmake ..
make
```

> NOTE: You may add the `-DCMAKE_BUILD_TYPE=Debug` or `-DCMAKE_BUILD_TYPE=Release` option to the fourth command to build in either debug or release mode respectively.

A file called `RenderBat` should appear in the `build` directory. You can run it using `./RenderBat`.

## Usage

> NOTE: Currently, *Render Bat* does not accept any command line arguments or other ways of configuring what it does.
Once structure deserialization is implemented, this will hopefully change.

As *Render Bat* is still in a very early development phase, it can only render a simple debug structure to either a window or a [PNG](https://en.wikipedia.org/wiki/Portable_Network_Graphics) file.

## Credits
- [GLFW](https://www.glfw.org) *(window and OpenGL context creation)*
- [Glad](https://github.com/Dav1dde/glad) *(OpenGL loader)*
- [GLM](https://github.com/g-truc/glm) *(Mathematics)*
- [stb](https://github.com/nothings/stb) *(image loading)*

## License

This project is licensed under the [GNU General Public License v2.0](https://www.gnu.org/licenses/gpl-2.0.html).

## About the author

The author goes by the pseudonym ***Root*** and prefers to not share any further personal information.

***GitHub:*** `0x526f6f74`
<br>
***Discord:*** `Root#0923`
