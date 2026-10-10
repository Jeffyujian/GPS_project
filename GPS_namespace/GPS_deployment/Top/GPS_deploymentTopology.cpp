// ======================================================================
// \title  GPS_deploymentTopology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <GPS_namespace/GPS_deployment/Top/GPS_deploymentTopologyAc.hpp>
// Note: Uncomment when using Svc:TlmPacketizer
#include <GPS_namespace/GPS_deployment/Top/GPS_deployment_GPS_deploymentPacketsTlmPacketsAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <cstdio>

// Public functions for use in main program are namespaced with deployment module GPS_namespace
// This is also the namespace where the topology components are instantiated by FPP.
namespace GPS_namespace {

// Instantiate a malloc allocator for cmdSeq buffer allocation
Fw::MallocAllocator mallocator;

namespace {
    bool gpsUartOpened = false;
}

// Rate group timing: base clock interval and divisors are coupled to rate group names
const Fw::TimeInterval rateGroupInterval(1, 0);  // 1Hz base clock
Svc::RateGroupDriver::DividerSet rateGroupDivisorsSet{{{1, 0}, {2, 0}, {4, 0}}};
// Divisors: 1Hz, 0.5Hz, 0.25Hz

// Context tokens for rate group members (unused, set to zero)
Svc::ActiveRateGroup::ContextArray rateGroup_1HzContext(0);
Svc::ActiveRateGroup::ContextArray rateGroup_0_5HzContext(0);
Svc::ActiveRateGroup::ContextArray rateGroup_0_25HzContext(0);

enum TopologyConstants {
    COMM_PRIORITY = 34,
};

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology() {
    // Rate group driver needs a divisor list
    rateGroupDriver.configure(rateGroupDivisorsSet);

    // Rate groups require context arrays.
    rateGroup_1Hz.configure(rateGroup_1HzContext);
    rateGroup_0_5Hz.configure(rateGroup_0_5HzContext);
    rateGroup_0_25Hz.configure(rateGroup_0_25HzContext);

    // PrmDb file name must be supplied by the using topology
    FileHandling::prmDb.configure("PrmDb.dat");

    // Command sequencer needs to allocate memory to hold contents of command sequences
    cmdSeq.allocateBuffer(0, mallocator, 5 * 1024);

    CdhCore::tlmSend.setPacketList(
        GPS_deployment_GPS_deploymentPacketsTlmPackets::packetList,
        GPS_deployment_GPS_deploymentPacketsTlmPackets::omittedChannels,
        2
    );

    Svc::BufferManager::BufferBins gpsBuffMgrBins;
    memset(&gpsBuffMgrBins, 0, sizeof(gpsBuffMgrBins));

    // define buffer size and numbers per bin. One bin is enough    for our scope.
    gpsBuffMgrBins.bins[0].bufferSize = 1024; // arbitrary
    gpsBuffMgrBins.bins[0].numBuffers = 16; // arbitrary
    gpsBufferManager.setup(300, 0, mallocator, gpsBuffMgrBins);

    gpsUartOpened = gpsUart.open(
        "/dev/serial0",
        Drv::LinuxUartDriver::BAUD_38400,
        Drv::LinuxUartDriver::NO_FLOW,
        Drv::LinuxUartDriver::PARITY_NONE,
        1024
    );

    if (gpsUartOpened) {
        std::printf("[INFO] UART opened successfully\n");
    } else {
        std::fprintf(stderr, "[ERROR] UART failed to open\n");
    }

}

void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded command registration. Function provided by autocoder.
    regCommands();
    // Autocoded configuration. Function provided by autocoder.
    configComponents(state);
    if (state.hostname != nullptr && state.port != 0) {
        comDriver.configure(state.hostname, state.port);
    }
    // Project-specific component configuration. Function provided above. May be inlined, if desired.
    configureTopology();
    // Autocoded parameter read from file. Function provided by autocoder.
    readParameters();
    // Autocoded parameter loading. Function provided by autocoder.
    loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    startTasks(state);
    // Start reception only after the GPS processing task is running.
    if (gpsUartOpened) {
        gpsUart.start();
    }
    // Initialize socket communication if and only if there is a valid specification
    if (state.hostname != nullptr && state.port != 0) {
        Os::TaskString name("ReceiveTask");
        // Uplink is configured for receive so a socket task is started
        comDriver.start(name, COMM_PRIORITY, Default::STACK_SIZE);
    }
}

void startRateGroups() {
    // Blocks until stopRateGroups() is called (e.g. from signal handler)
    timer.startTimer(rateGroupInterval);
}

void stopRateGroups() {
    timer.quit();
}

void teardownTopology(const TopologyState& state) {
    // Stop producers while active components can still process their messages.
    if (gpsUartOpened) {
        gpsUart.quitReadThread();
        (void)gpsUart.join();
    }

    comDriver.stop();
    (void)comDriver.join();

    // Stop and join active component tasks.
    stopTasks(state);
    freeThreads(state);

    // Release resources after their users have stopped.
    cmdSeq.deallocateBuffer(mallocator);
    gpsBufferManager.cleanup();

    tearDownComponents(state);
    deinitComponents(state);
}
};  // namespace GPS_namespace
