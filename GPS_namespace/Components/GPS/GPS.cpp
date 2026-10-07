// ======================================================================
// \title  GPS.cpp
// \author yujianfu
// \brief  cpp file for GPS component implementation class
// ======================================================================

#include "GPS_namespace/Components/GPS/GPS.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstdio>

namespace {

// Report malformed GPS data to the deployment console.
void parseError(const char* message) {
    std::fprintf(stderr, "[GPS] Parsing error: %s\n", message);
}

// Convert a hexadecimal digit; return -1 for invalid input.
int hexDigit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

// Verify the XOR checksum between '$' and '*'.
bool validChecksum(const std::string& line) {
    const auto star = line.find('*');

    if (line.empty() || line[0] != '$' ||
        star == std::string::npos || star + 3 != line.size()) {
        return false;
    }

    const int high = hexDigit(line[star + 1]);
    const int low = hexDigit(line[star + 2]);

    unsigned char checksum = 0;
    for (std::size_t i = 1; i < star; ++i) {
        checksum ^= static_cast<unsigned char>(line[i]);
    }

    return high >= 0 && low >= 0 && checksum == high * 16 + low;
}

// Split fields while preserving empty entries, including the final one.
std::vector<std::string> splitFields(const std::string& text) {
    std::vector<std::string> fields;
    std::size_t start = 0;

    while (true) {
        const auto comma = text.find(',', start);
        fields.push_back(text.substr(start, comma - start));

        if (comma == std::string::npos) {
            return fields;
        }
        start = comma + 1;
    }
}

// Parse a finite decimal number and reject unconsumed characters.
bool parseNumber(const std::string& text, double& value) {
    if (text.empty() ||
        text.find_first_not_of("0123456789.+-") != std::string::npos) {
        return false;
    }

    char* end = nullptr;
    value = std::strtod(text.c_str(), &end);

    return end != text.c_str() && *end == '\0' && std::isfinite(value);
}

// Convert hhmmss.ss into UTC seconds since midnight.
bool parseUtc(const std::string& text, double& result) {
    double raw;
    if (!parseNumber(text, raw) || raw < 0.0 || raw >= 240000.0) {
        return false;
    }

    const int hours = static_cast<int>(raw / 10000.0);
    const int minutes = static_cast<int>((raw - hours * 10000.0) / 100.0);
    const double seconds = raw - hours * 10000.0 - minutes * 100.0;

    if (minutes >= 60 || seconds >= 60.0) {
        return false;
    }

    result = hours * 3600.0 + minutes * 60.0 + seconds;
    return true;
}

// Convert NMEA degrees/minutes to signed decimal degrees.
bool parseCoordinate(
    const std::string& text,
    const std::string& hemisphere,
    bool latitude,
    double& result
) {
    double raw;
    const double limit = latitude ? 90.0 : 180.0;

    if (!parseNumber(text, raw) || raw < 0.0 || raw > limit * 100.0) {
        return false;
    }

    const double degrees = std::floor(raw / 100.0);
    const double minutes = raw - degrees * 100.0;
    const std::string positive = latitude ? "N" : "E";
    const std::string negative = latitude ? "S" : "W";

    if (minutes >= 60.0 ||
        (hemisphere != positive && hemisphere != negative)) {
        return false;
    }

    result = degrees + minutes / 60.0;
    if (hemisphere == negative) {
        result = -result;
    }
    return true;
}

}  // namespace

namespace GPS_namespace {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

GPS ::GPS(const char* const compName) : GPSComponentBase(compName) {}

GPS ::~GPS() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

// Receive bytes and return ownership of the UART buffer.
void GPS::UartRead_handler(FwIndexType portNum, Fw::Buffer& buffer, const Drv::ByteStreamStatus& status) {
    (void)portNum;

    if (status == Drv::ByteStreamStatus::OP_OK) {
        const U8* data = buffer.getData();

        if (data != nullptr) {
            for (std::size_t i = 0; i < buffer.getSize(); ++i) {
                receiveCharacter(static_cast<char>(data[i]));
            }
        }
    } else {
        std::fprintf(stderr, "[GPS] UART receive error\n");
        m_nmeaLine.clear();
    }

    this->deallocate_out(0, buffer);
}

// Assemble complete sentences across arbitrary UART buffer boundaries.
void GPS::receiveCharacter(char c) {
    if (c == '$') {
        if (!m_nmeaLine.empty()) {
            parseError("incomplete sentence");
        }
        m_nmeaLine = "$";
    } else if (c == '\r' || c == '\n') {
        if (!m_nmeaLine.empty()) {
            processSentence(m_nmeaLine);
            m_nmeaLine.clear();
        }
    } else if (!m_nmeaLine.empty()) {
        if (m_nmeaLine.size() < 255) {
            m_nmeaLine += c;
        } else {
            parseError("sentence too long");
            m_nmeaLine.clear();
        }
    }
}

// Validate a GGA sentence and publish its measurements.
void GPS::processSentence(const std::string& line) {
    if (line.size() < 7 || line.compare(3, 4, "GGA,") != 0) {
        return;  // Ignore other sentence types.
    }

    if (!validChecksum(line)) {
        parseError("invalid checksum");
        return;
    }

    const auto fields = splitFields(line.substr(0, line.find('*')));

    if (fields.size() < 11) {
        parseError("not enough GGA fields");
        return;
    }

    if (fields[6].size() != 1 ||
        fields[6][0] < '0' || fields[6][0] > '8') {
        parseError("invalid fix quality");
        return;
    }

    const int quality = fields[6][0] - '0';
    if (quality == 0 || quality >= 6) {
        this->log_WARNING_LO_NoGpsLock();
        return;
    }

    double utc, latitude, longitude, altitude;

    if (!parseUtc(fields[1], utc)) {
        parseError("invalid UTC");
        return;
    }

    if (!parseCoordinate(fields[2], fields[3], true, latitude) ||
        !parseCoordinate(fields[4], fields[5], false, longitude)) {
        parseError("invalid coordinates");
        return;
    }

    if (!parseNumber(fields[9], altitude) || fields[10] != "M") {
        parseError("invalid altitude");
        return;
    }

    this->tlmWrite_UTCTime(utc);
    this->tlmWrite_Latitude(latitude);
    this->tlmWrite_Longitude(longitude);
    this->tlmWrite_Altitude(altitude);
}

}  // namespace GPS_namespace
