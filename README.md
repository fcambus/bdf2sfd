# bdf2sfd

bdf2sfd is a [BDF][1] to [SFD][2] converter, allowing to vectorize bitmap
fonts. It works by converting each pixel of a glyph to a polygon, which
produces large and unoptimized SFD files that should be post-processed
using [FontForge][3].

There are several reasons why it might be desirable to convert bitmap
fonts to OpenType or TrueType versions. This allow for example to use them
as Web fonts, or simply to keep using them in some terminal emulators,
since Pango 1.44 dropped support for bitmap fonts.

It was originally created to generate OpenType versions of [Spleen][4], and
is released in the hope it can be useful to convert other fonts as well.
Please be aware that it works best on fonts proportional to 8x16. Other
sizes will work but the aspect ratio will not be preserved. There is
currently little interest in addressing the issue.

## Dependencies

bdf2sfd uses the `CMake` build system and does not requires any external
dependencies.

	mkdir build
	cd build
	cmake ..
	make

bdf2sfd has been successfully built and tested on OpenBSD, NetBSD, FreeBSD,
and Linux with both Clang and GCC.

## Packages

Packages are available for the following operating systems:

- [OpenBSD][5]
- [NetBSD][6]
- [Void Linux][7]
- [Debian][8]
- [Ubuntu][9]

## Usage

	bdf2sfd [-hv] [-f name] [-p name] file

If file is a single dash (`-'), bdf2sfd reads from the standard input.

The options are as follows:

	-f name	Specify font name.
	-p name	Specify PostScript font name.
	-h	Display usage.
	-v	Display version.

bdf2sfd outputs SFD data to **stdout**.

### Post-processing

The SFD files created by bdf2sfd should be post-processed with FontForge
in order to remove overlap and simplify shapes.

This can be done as follow:

```
fontforge -lang ff -c 'Open("spleen.sfd"); SelectAll(); RemoveOverlap(); Simplify(-1, 1); Save("spleen.sfd")'
```

### Producing OTF and TTF fonts

After post-processing, OpenType and/or TrueType fonts can be produced as follow:

For OTF fonts:

```
fontforge -lang ff -c 'Open("spleen.sfd"); Generate("spleen.otf")'
```

For TTF fonts:

```
fontforge -lang ff -c 'Open("spleen.sfd"); Generate("spleen.ttf")'
```

## License

bdf2sfd is released under the BSD 2-Clause license. See `LICENSE` file for
details.

## Author

bdf2sfd is developed by Frederic Cambus.

- Site: https://www.cambus.net

## Resources

GitHub: https://github.com/fcambus/bdf2sfd

[1]: https://en.wikipedia.org/wiki/Glyph_Bitmap_Distribution_Format
[2]: https://fontforge.org/en-US/documentation/developers/sfdformat/
[3]: https://fontforge.org
[4]: https://github.com/fcambus/spleen
[5]: https://cvsweb.openbsd.org/cgi-bin/cvsweb/ports/converters/bdf2sfd
[6]: https://pkgsrc.se/converters/bdf2sfd
[7]: https://github.com/void-linux/void-packages/tree/master/srcpkgs/bdf2sfd
[8]: https://packages.debian.org/search?keywords=bdf2sfd
[9]: https://packages.ubuntu.com/bdf2sfd
