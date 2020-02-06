# bdftosfd

bdftosfd is a [BDF][1] to [SFD][2] converter, allowing to vectorize bitmap
fonts. It works by converting each pixel of a glyph to a polygon, which
produces large and unoptimized SFD files which should be post-processed
using [FontForge][3].

This is a work in progress, and assumes input fonts are 8x16 for now. It
was originally created to create OpenType versions of [Spleen][4], and I'm
curently working on making it a general purpose converter.

## Dependencies

bdftosfd uses the `CMake` build system and does not requires any external
dependencies.

	mkdir build
	cd build
	cmake ..
	make

bdftosfd has been successfully built and tested on OpenBSD, NetBSD, FreeBSD,
and Linux with both Clang and GCC.

## Usage

	bdftosfd [-hv] [-f name] file

If file is a single dash (`-'), bdftosfd reads from the standard input.

The options are as follows:

	-f name	Specify font name.
	-h	Display usage.
	-v	Display version.

bdftosfd outputs SFD data to **stdout**.

### Post-processing

### Producing OTF and TTF fonts

## License

bdftosfd is released under the BSD 2-Clause license. See `LICENSE` file for
details.

## Author

bdftosfd is developed by Frederic Cambus.

- Site: https://www.cambus.net

## Resources

GitHub: https://github.com/fcambus/bdftosfd

[1]: https://en.wikipedia.org/wiki/Glyph_Bitmap_Distribution_Format
[2]: https://fontforge.org/en-US/documentation/developers/sfdformat/
[3]: https://fontforge.org
[4]: https://github.com/fcambus/spleen
