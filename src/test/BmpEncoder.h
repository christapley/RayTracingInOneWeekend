#pragma once

#include <string>
#include <fstream>

namespace tapley {

class CBmpEncoder 
{
public:
	CBmpEncoder() = delete;
	CBmpEncoder(const CBmpEncoder &) = delete;
	CBmpEncoder(CBmpEncoder &&) = delete;
	CBmpEncoder(std::string location, const uint8_t *data, uint32_t width, uint32_t height, bool hasAlphaChannel) :
		_location(std::move(location)),
		_data(data),
		_width(width),
		_height(height),
		_hasAlphaChannel(hasAlphaChannel)
	{

	}

	virtual ~CBmpEncoder() = default;
	
	CBmpEncoder &operator=(const CBmpEncoder &) = delete;
	CBmpEncoder &operator=(CBmpEncoder &&) = delete;

	void Write() const
	{
		std::ofstream outputFileStream(_location, std::ios::out | std::ios::binary);

		if (outputFileStream.fail()) {
			throw std::runtime_error(std::string("Unable to open output file ") + _location);
		}

		const size_t bufferSize = _width * _height;

		//Padding
		const uint8_t padding = _hasAlphaChannel ? 0 : (4 - (_width * 3) % 4) % 4;

		const uint32_t fileSize = static_cast<uint32_t>(bufferSize) * sizeof(uint8_t) + padding * (_height - 1) + 14 + 124;
		const uint16_t bitsPerPixel = (_hasAlphaChannel) ? 32 : 24;
		const uint32_t offset = 14 + 124;

		// Writing the file header and information header to the file
		WriteBinary8bit(outputFileStream, "BM", 2);
		WriteBinary32bit(outputFileStream, fileSize);													// file size
		WriteBinary32bit(outputFileStream, 0);
		WriteBinary32bit(outputFileStream, offset);														// offset

		WriteBinary32bit(outputFileStream, 124);														// DIBSize
		WriteBinary32bit(outputFileStream, _width);													    // width
		WriteBinary32bit(outputFileStream, _height);													// height
		WriteBinary16bit(outputFileStream, static_cast<uint16_t>(1));									// numPlanes
		WriteBinary16bit(outputFileStream, static_cast<uint16_t>(_hasAlphaChannel ? 32 : 24));			// bitsPerPixel
		WriteBinary32bit(outputFileStream, static_cast<uint32_t>(_hasAlphaChannel ? 3 : 0));			// compressionMethod BI_RGB = 0, BI_BITFIELDS = 3
		WriteBinary32bit(outputFileStream, static_cast<uint32_t>(bufferSize * sizeof(uint8_t)));
		WriteBinary32bit(outputFileStream, 2834);														// horizontalResolution
		WriteBinary32bit(outputFileStream, 2834);														// verticalResolution
		WriteBinary32bit(outputFileStream, 0);														    // numColors
		WriteBinary32bit(outputFileStream, 0);														    // impColorCount
		WriteBinary32bit(outputFileStream, static_cast<uint32_t>(_hasAlphaChannel ? 0x0000FF00 : 0));	// redBitmask
		WriteBinary32bit(outputFileStream, static_cast<uint32_t>(_hasAlphaChannel ? 0x00FF0000 : 0));	// greenBitmask
		WriteBinary32bit(outputFileStream, static_cast<uint32_t>(_hasAlphaChannel ? 0xFF000000 : 0));	// blueBitmask
		WriteBinary32bit(outputFileStream, static_cast<uint32_t>(_hasAlphaChannel ? 0x000000FF : 0));	// alphaBitmask

		for (uint32_t i = 70; i < offset; i++) {
			WriteBinary8bit(outputFileStream, 0);
		}

		//Writing the pixel array
		const uint32_t bWidth = bitsPerPixel / 8 * _width;

		for (int i = _height - 1; i >= 0; i--) {
			outputFileStream.write(reinterpret_cast<const char*>(_data) + i * bWidth, bWidth * sizeof(uint8_t));
			outputFileStream.seekp(padding * sizeof(uint8_t), std::ios::cur);
		}

		outputFileStream.flush();
	}

protected:
	std::string _location;
	const uint8_t *_data;
	const uint32_t _width;
	const uint32_t _height;
	const bool _hasAlphaChannel;

	std::ostream& WriteBinary8bit(std::ostream& out, char c) const
	{
		return out.put(c);
	}

	std::ostream& WriteBinary8bit(std::ostream& out, char const* c, std::size_t n) const
	{
		return out.write(c, n);
	}

	std::ostream& WriteBinary8bit(std::ostream& out, uint8_t const* c, std::size_t n) const
	{
		return WriteBinary8bit(out, reinterpret_cast<char const*>(c), n);
	}

	std::ostream& WriteBinary16bit(std::ostream& out, std::uint16_t v) const
	{
		return out.put(v & 0xFFu).put((v >> 8) & 0xFFu);
	}

	std::ostream& WriteBinary16bit(std::ostream& out, std::int16_t v) const
	{
		return WriteBinary16bit(out, static_cast<std::uint16_t>(v));
	}

	std::ostream& WriteBinary32bit(std::ostream& out, std::uint32_t v) const
	{
		return out.put(v & 0xFFu).put((v >> 8) & 0xFFu).put((v >> 16) & 0xFFu).put((v >> 24) & 0xFFu);
	}

	std::ostream& WriteBinary32bit(std::ostream& out, std::int32_t v) const
	{
		return WriteBinary32bit(out, static_cast<std::uint32_t>(v));
	}
};

}