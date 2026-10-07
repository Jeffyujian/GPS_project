telemetry packets GPS_deploymentPackets {

  packet CDH id 1 group 1 {
    CdhCore.cmdDisp.CommandsDispatched
    CdhCore.cmdDisp.CommandsDropped
    #CdhCore.tlmSend.SendLevel # uncomment when using TlmPacketizer

    FileHandling.fileUplink.FilesReceived
    FileHandling.fileUplink.PacketsReceived
    FileHandling.fileDownlink.FilesSent
    FileHandling.fileDownlink.PacketsSent
    FileHandling.fileManager.CommandsExecuted

    GPS_namespace.cmdSeq.CS_LoadCommands
    GPS_namespace.cmdSeq.CS_CancelCommands
    GPS_namespace.cmdSeq.CS_CommandsExecuted
    GPS_namespace.cmdSeq.CS_SequencesCompleted
    ComCcsds.comQueue.comQueueDepth
    ComCcsds.comQueue.buffQueueDepth
    ComCcsds.commsBufferManager.TotalBuffs
    ComCcsds.commsBufferManager.CurrBuffs
    ComCcsds.commsBufferManager.HiBuffs

    GPS_namespace.rateGroup_1Hz.RgMaxTime
    GPS_namespace.rateGroup_0_5Hz.RgMaxTime
    GPS_namespace.rateGroup_0_25Hz.RgMaxTime
  }

  packet CDHErrors id 2 group 1 {
    CdhCore.$health.PingLateWarnings

    FileHandling.fileUplink.Warnings
    FileHandling.fileDownlink.Warnings
    FileHandling.fileManager.Errors

    GPS_namespace.cmdSeq.CS_Errors
    ComCcsds.commsBufferManager.NoBuffs
    ComCcsds.commsBufferManager.EmptyBuffs

    GPS_namespace.rateGroup_1Hz.RgCycleSlips
    GPS_namespace.rateGroup_0_5Hz.RgCycleSlips
    GPS_namespace.rateGroup_0_25Hz.RgCycleSlips
  }

  packet SystemRes1 id 5 group 2 {
    GPS_namespace.systemResources.MEMORY_TOTAL
    GPS_namespace.systemResources.MEMORY_USED
    GPS_namespace.systemResources.NON_VOLATILE_TOTAL
    GPS_namespace.systemResources.NON_VOLATILE_FREE
  }

  packet SystemRes3 id 6 group 2 {
    GPS_namespace.systemResources.CPU
    GPS_namespace.systemResources.CPU_00
    GPS_namespace.systemResources.CPU_01
    GPS_namespace.systemResources.CPU_02
    GPS_namespace.systemResources.CPU_03
    GPS_namespace.systemResources.CPU_04
    GPS_namespace.systemResources.CPU_05
    GPS_namespace.systemResources.CPU_06
    GPS_namespace.systemResources.CPU_07
    GPS_namespace.systemResources.CPU_08
    GPS_namespace.systemResources.CPU_09
    GPS_namespace.systemResources.CPU_10
    GPS_namespace.systemResources.CPU_11
    GPS_namespace.systemResources.CPU_12
    GPS_namespace.systemResources.CPU_13
    GPS_namespace.systemResources.CPU_14
    GPS_namespace.systemResources.CPU_15
  }

  packet DataProducts id 21 group 3 {
    DataProducts.dpCat.CatalogDps
    DataProducts.dpCat.DpsSent

    DataProducts.dpMgr.NumSuccessfulAllocations
    DataProducts.dpMgr.NumFailedAllocations
    DataProducts.dpMgr.NumDataProducts
    DataProducts.dpMgr.NumBytes

    DataProducts.dpWriter.NumBuffersReceived
    DataProducts.dpWriter.NumBytesWritten
    DataProducts.dpWriter.NumSuccessfulWrites
    DataProducts.dpWriter.NumFailedWrites
    DataProducts.dpWriter.NumErrors

    DataProducts.dpBufferManager.TotalBuffs
    DataProducts.dpBufferManager.CurrBuffs
    DataProducts.dpBufferManager.HiBuffs
    DataProducts.dpBufferManager.NoBuffs
    DataProducts.dpBufferManager.EmptyBuffs
  }

  packet Version1 id 22 group 2 {
    CdhCore.version.FrameworkVersion
    CdhCore.version.ProjectVersion
  }

  packet Version_Library1 id 23 group 2 {
    CdhCore.version.LibraryVersion01
    CdhCore.version.LibraryVersion02
  }

  packet Version_Library2 id 24 group 2 {
    CdhCore.version.LibraryVersion03
    CdhCore.version.LibraryVersion04
  }

  packet Version_Library3 id 25 group 2 {
    CdhCore.version.LibraryVersion05
    CdhCore.version.LibraryVersion06
  }

  packet Version_Library4 id 26 group 2 {
    CdhCore.version.LibraryVersion07
    CdhCore.version.LibraryVersion08
  }

  packet Version_Library5 id 27 group 2 {
    CdhCore.version.LibraryVersion09
    CdhCore.version.LibraryVersion10
  }

  packet Version_Custom1 id 28 group 2 {
    CdhCore.version.CustomVersion01
  }

  packet Version_Custom2 id 29 group 2 {
    CdhCore.version.CustomVersion02
  }

  packet Version_Custom3 id 30 group 2 {
    CdhCore.version.CustomVersion03
  }

  packet Version_Custom4 id 31 group 2 {
    CdhCore.version.CustomVersion04
  }

  packet Version_Custom5 id 32 group 2 {
    CdhCore.version.CustomVersion05
  }

  packet Version_Custom6 id 33 group 2 {
    CdhCore.version.CustomVersion06
  }

  packet Version_Custom7 id 34 group 2 {
    CdhCore.version.CustomVersion07
  }

  packet Version_Custom8 id 35 group 2 {
    CdhCore.version.CustomVersion08
  }

  packet Version_Custom9 id 36 group 2 {
    CdhCore.version.CustomVersion09
  }

  packet Version_Custom10 id 37 group 2 {
    CdhCore.version.CustomVersion10
  }

  packet GpsInfo id 8 group 2 {
    GPS_namespace.gps.UTCTime
    GPS_namespace.gps.Latitude
    GPS_namespace.gps.Longitude
    GPS_namespace.gps.Altitude
  }

} omit {
  CdhCore.cmdDisp.CommandErrors

  GPS_namespace.gpsBufferManager.TotalBuffs
  GPS_namespace.gpsBufferManager.CurrBuffs
  GPS_namespace.gpsBufferManager.HiBuffs
  GPS_namespace.gpsBufferManager.NoBuffs
  GPS_namespace.gpsBufferManager.EmptyBuffs

  GPS_namespace.gpsUart.BytesSent
  GPS_namespace.gpsUart.BytesRecv

  CdhCore.tlmSend.GroupConfigs
  CdhCore.tlmSend.SectionEnabled
  GPS_namespace.cmdSeq.CS_CurrentSequence
  FileHandling.fileUplink.FilesReceivedFailed

  CdhCore.events.EventsDropped
  DataProducts.dpBufferAccumulator.BA_NumQueuedBuffers
}
