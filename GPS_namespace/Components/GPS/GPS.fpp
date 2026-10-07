module GPS_namespace {
    @ part 2 of project 0
    active component GPS {

        # One async command/port is required for active components
        # This should be overridden by the developers with a useful command/port
        @ TODO

        @ Receive a buffer of bytes from the UART driver
        async input port UartRead: Drv.ByteStreamData

        @ Return the buffer after processing its contents
        output port deallocate: Fw.BufferSend

        # Keep your existing generated standard ports here

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64
        @ UTC time in seconds since midnight
        telemetry UTCTime: F64

        @ Latitude in decimal degrees; north positive
        telemetry Latitude: F64

        @ Longitude in decimal degrees; east positive
        telemetry Longitude: F64

        @ Altitude above mean sea level in meters
        telemetry Altitude: F64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"
        @ GPS reports no usable position fix
        event NoGpsLock \
            severity warning low \
            format "GPS has no usable satellite fix"
        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut
    }
}