// ======================================================================
// \title  GPS.hpp
// \author yujianfu
// \brief  hpp file for GPS component implementation class
// ======================================================================

#ifndef GPS_namespace_GPS_HPP
#define GPS_namespace_GPS_HPP

#include "GPS_namespace/Components/GPS/GPSComponentAc.hpp"
#include <string>

namespace GPS_namespace {

class GPS final : public GPSComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct GPS object
    GPS(const char* const compName  //!< The component name
    );

    //! Destroy GPS object
    ~GPS();

  private:
    std::string m_nmeaLine;
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for UartRead
    //!
    //! TODO
    //! Receive a buffer of bytes from the UART driver
    void UartRead_handler(FwIndexType portNum,  //!< The port number
                          Fw::Buffer& buffer,
                          const Drv::ByteStreamStatus& status) override;

    // Assemble complete sentences across arbitrary UART buffer boundaries.
    void receiveCharacter(char c);
    
    // parse through the GGA sentence to get useful GPS data and punlish them
    void processSentence(const std::string& line);
};

}  // namespace GPS_namespace

#endif
