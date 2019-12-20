# bdftosfd

bdftosfd is a [BDF][1] to [SFD][2] converter.

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

	bdftosfd [-hv] file

If file is a single dash (`-'), bdftosfd reads from the standard input.

The options are as follows:

	-h	Display usage.
	-v	Display version.

bdftosfd outputs SFD data to **stdout**.

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
