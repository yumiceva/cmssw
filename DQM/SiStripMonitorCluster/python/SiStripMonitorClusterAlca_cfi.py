import FWCore.ParameterSet.Config as cms

# SiStripMonitorCluster
SiStripCalZeroBiasMonitorCluster = cms.EDAnalyzer("SiStripMonitorCluster",
    # by default do not write out any file with histograms
    # can overwrite this in .cfg file with: replace SiStripMonitorCluster.OutputMEsInRootFile = true
    ClusterProducer = cms.string('calZeroBiasClusters'),
                                     
    OutputMEsInRootFile = cms.bool(False),
    OutputFileName = cms.string('SiStripMonitorCluster.root'),
                                     
    ResetMEsEachRun = cms.bool(False),

    StripQualityLabel = cms.string('unbiased'),

    SelectAllDetectors = cms.bool(True),
    ShowMechanicalStructureView = cms.bool(True),

    ClusterLabel = cms.string(''),

    TkHistoMap_On = cms.bool(False),
                                     
    TopFolderName = cms.string('AlcaReco/SiStrip'),
                                     
    CreateTrendMEs = cms.bool(False),
    Trending = cms.PSet(
        Nbins = cms.int32(600),
        xmin = cms.double(0.0),
        xmax = cms.double(1.0*60*60),
        ymin = cms.double(0.0),
        ymax = cms.double(100000.0)
    ),
    TH1ClusterNoise = cms.PSet(
        Nbinx          = cms.int32(20),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(9.5),
        layerswitchon  = cms.bool(False),
        moduleswitchon = cms.bool(False)
    ),

    TH1NrOfClusterizedStrips = cms.PSet(
        Nbinx          = cms.int32(20),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(99.5),
        layerswitchon  = cms.bool(False),
        moduleswitchon = cms.bool(False)
    ),
    TH1ClusterPos = cms.PSet(
        Nbinx          = cms.int32(768),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(767.5),
        layerswitchon  = cms.bool(False),
        moduleswitchon = cms.bool(False)
    ),
    TH1ClusterDigiPos = cms.PSet(
        Nbinx          = cms.int32(768),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(767.5),
        layerswitchon  = cms.bool(False),
        moduleswitchon = cms.bool(True)
    ),                                
    TH1ModuleLocalOccupancy = cms.PSet(
        Nbinx          = cms.int32(20),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(0.95),
        layerswitchon  = cms.bool(False),
        moduleswitchon = cms.bool(False)
    ),
    TH1nClusters = cms.PSet(
        Nbinx          = cms.int32(11),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(10.5),
        layerswitchon  = cms.bool(False),
        moduleswitchon = cms.bool(False)
    ),
    TH1ClusterStoN = cms.PSet(
        Nbinx          = cms.int32(100),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(299.5),
        layerswitchon  = cms.bool(False),
        moduleswitchon = cms.bool(False)
    ),
    TH1ClusterStoNVsPos = cms.PSet(
        Nbinx          = cms.int32(768),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(767.5),
        Nbiny          = cms.int32(100),
        ymin           = cms.double(-0.5),
        ymax           = cms.double(299.5),
        layerswitchon  = cms.bool(False),
        moduleswitchon = cms.bool(False)
    ),
    TH1ClusterCharge = cms.PSet(
        Nbinx          = cms.int32(200),
        xmin           = cms.double(-0.5),        
        xmax           = cms.double(799.5),
        layerswitchon  = cms.bool(False),
        moduleswitchon = cms.bool(False)
    ),
    TH1ClusterWidth = cms.PSet(
        Nbinx          = cms.int32(20),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(19.5),
        layerswitchon  = cms.bool(False),        
        moduleswitchon = cms.bool(False)
    ),

    TProfNumberOfCluster = cms.PSet(
        Nbinx            = cms.int32(100),
        xmin             = cms.double(-0.5),
        xmax             = cms.double(499.5),
        layerswitchon    = cms.bool(False),        
        moduleswitchon   = cms.bool(False)        
    ),
      
    TProfClusterWidth    = cms.PSet(
        Nbinx            = cms.int32(100),
        xmin             = cms.double(-0.5),
        xmax             = cms.double(499.5),
        layerswitchon    = cms.bool(False),        
        moduleswitchon   = cms.bool(False)        
    ),
                                     
    ClusterConditions = cms.PSet(
        minWidth   = cms.double(0.0),
        On         = cms.bool(True),
        maxStoN    = cms.double(10000.0),
        minStoN    = cms.double(0.0),
        maxWidth   = cms.double(10000.0)
    ),
    TProfTotalNumberOfClusters = cms.PSet(
        Nbins = cms.int32(600),
        xmin = cms.double(0.0),
        xmax = cms.double(1.0*60*60),
        ymin = cms.double(0.0),
        ymax = cms.double(0.0),
        subdetswitchon = cms.bool(False)
    ),

    TH1TotalNumberOfClusters = cms.PSet(
        Nbinx          = cms.int32(50),
        xmin           = cms.double(-0.5),
        xmax           = cms.double(299.5),
        subdetswitchon = cms.bool(True)
    ),
    TProfClustersApvCycle = cms.PSet(
        Nbins = cms.int32(70),
        xmin = cms.double(-0.5),
        xmax = cms.double(69.5),
        Nbinsy = cms.int32(200),
        ymin = cms.double(0.0),
        ymax = cms.double(0.0),
        subdetswitchon = cms.bool(False)
        ),

    TH2ClustersApvCycle = cms.PSet(
        Nbinsx = cms.int32(70),
        xmin = cms.double(-0.5),
        xmax = cms.double(69.5),
        Nbinsy = cms.int32(200),
        ymin = cms.double(0.0),
        yfactor = cms.double(0.0002),
        subdetswitchon = cms.bool(True)
    ),

    TProfClustersVsDBxCycle = cms.PSet(
        Nbins = cms.int32(800),
        xmin = cms.double(0.5),
        xmax = cms.double(800.5),
        ymin = cms.double(0.0),
        ymax = cms.double(0.0),
        subdetswitchon = cms.bool(False)
        ),
                                                                          
    TProf2ApvCycleVsDBx = cms.PSet(
        Nbinsx = cms.int32(70),
        xmin   = cms.double(-0.5),
        xmax   = cms.double(69.5),
        Nbinsy = cms.int32(800),
        ymin   = cms.double(0.5),
        ymax   = cms.double(800.5),
        zmin   = cms.double(0.0),
        zmax   = cms.double(0.0),
        subdetswitchon = cms.bool(False)
        ),
                                     
    TH2ApvCycleVsDBxGlobal = cms.PSet(
        Nbinsx = cms.int32(70),
        xmin   = cms.double(-0.5),
        xmax   = cms.double(69.5),
        Nbinsy = cms.int32(800),
        ymin   = cms.double(0.5),
        ymax   = cms.double(800.5),
        globalswitchon = cms.bool(False)
        ),

    Mod_On = cms.bool(True),

    HistoryProducer = cms.InputTag("consecutiveHEs"),
    ApvPhaseProducer = cms.InputTag("APVPhases"),

    ShowControlView = cms.bool(False),
    ShowReadoutView = cms.bool(False)
)
