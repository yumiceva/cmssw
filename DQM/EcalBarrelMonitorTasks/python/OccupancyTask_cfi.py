import FWCore.ParameterSet.Config as cms

tpThreshold = 4.
recHitThreshold = 0.5

ecalOccupancyTask = cms.untracked.PSet(
    recHitThreshold = cms.untracked.double(recHitThreshold),
    tpThreshold = cms.untracked.double(tpThreshold),
    MEs = cms.untracked.PSet(
        TrendNTPDigi = cms.untracked.PSet(
            path = cms.untracked.string('Ecal/Trends/OccupancyTask %(prefix)s number of filtered TP digis'),
            kind = cms.untracked.string('TProfile'),
            otype = cms.untracked.string('Ecal2P'),
            btype = cms.untracked.string('Trend'),
            description = cms.untracked.string('Trend of the per-event number of TP digis with Et > ' + str(tpThreshold) + ' GeV.')
        ),
        TPDigiThrProjPhi = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT TP digi thr occupancy%(suffix)s projection phi'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjPhi'),
            description = cms.untracked.string('Projection of the occupancy of TP digis with Et > ' + str(tpThreshold) + ' GeV.')
        ),
        DigiDCC = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT digi occupancy by DCC'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal2P'),
            btype = cms.untracked.string('DCC'),
            description = cms.untracked.string('DCC digi occupancy.')
        ),
        DigiProjPhi = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT digi occupancy%(suffix)s projection phi'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjPhi'),
            description = cms.untracked.string('Projection of digi occupancy.')
        ),
        TPDigiProjEta = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT TP digi occupancy%(suffix)s projection eta'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjEta'),
            description = cms.untracked.string('Projection of TP digi occupancy.')
        ),
        Digi = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT digi occupancy %(sm)s'),
            kind = cms.untracked.string('TH2F'),
            otype = cms.untracked.string('SM'),
            btype = cms.untracked.string('Crystal'),
            description = cms.untracked.string('Digi occupancy.')
        ),
        DigiProjEta = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT digi occupancy%(suffix)s projection eta'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjEta'),
            description = cms.untracked.string('Projection of digi occupancy.')
        ),
        TrendNRecHitThr = cms.untracked.PSet(
            path = cms.untracked.string('Ecal/Trends/OccupancyTask %(prefix)s number of filtered recHits'),
            kind = cms.untracked.string('TProfile'),
            otype = cms.untracked.string('Ecal2P'),
            btype = cms.untracked.string('Trend'),
            description = cms.untracked.string('Trend of the per-event number of rec hits with GOOD reconstruction flag and E > ' + str(recHitThreshold) + ' GeV.')
        ),
        TPDigiAll = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT TP digi occupancy%(suffix)s'),
            kind = cms.untracked.string('TH2F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('TriggerTower'),
            description = cms.untracked.string('TP digi occupancy.')
        ),
        TPDigiThrProjEta = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT TP digi thr occupancy%(suffix)s projection eta'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjEta'),
            description = cms.untracked.string('Projection of the occupancy of TP digis with Et > ' + str(tpThreshold) + ' GeV.')
        ),
        TrendNDigi = cms.untracked.PSet(
            path = cms.untracked.string('Ecal/Trends/OccupancyTask %(prefix)s number of digis'),
            kind = cms.untracked.string('TProfile'),
            otype = cms.untracked.string('Ecal2P'),
            btype = cms.untracked.string('Trend'),
            description = cms.untracked.string('Trend of the per-event number of digis.')
        ),
        RecHitThr1D = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT number of filtered rec hits in event'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal2P'),
            xaxis = cms.untracked.PSet(
                high = cms.untracked.double(500.0),
                nbins = cms.untracked.int32(200),
                low = cms.untracked.double(0.0)
            ),
            btype = cms.untracked.string('User'),
            description = cms.untracked.string('Occupancy of rec hits with GOOD reconstruction flag and E > ' + str(recHitThreshold) + ' GeV.')
        ),
        Digi1D = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT number of digis in event'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal2P'),
            xaxis = cms.untracked.PSet(
                high = cms.untracked.double(3000.0),
                nbins = cms.untracked.int32(200),
                low = cms.untracked.double(0.0)
            ),
            btype = cms.untracked.string('User'),
            description = cms.untracked.string('Distribution of the number of digis per event.')
        ),
        DigiAll = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT digi occupancy%(suffix)s'),
            kind = cms.untracked.string('TH2F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('SuperCrystal'),
            description = cms.untracked.string('Digi occupancy.')
        ),
        RecHitThrProjEta = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT rec hit thr occupancy%(suffix)s projection eta'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjEta'),
            description = cms.untracked.string('Projection of the occupancy of rec hits with GOOD reconstruction flag and E > ' + str(recHitThreshold) + ' GeV.')
        ),
        RecHitProjEta = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT rec hit occupancy%(suffix)s projection eta'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjEta'),
            description = cms.untracked.string('Projection of the occupancy of all rec hits.')
        ),
        TPDigiThrAll = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT TP digi thr occupancy%(suffix)s'),
            kind = cms.untracked.string('TH2F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('TriggerTower'),
            description = cms.untracked.string('Occupancy for TP digis with Et > ' + str(tpThreshold) + ' GeV.')
        ),
        RecHitThrAll = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT rec hit thr occupancy%(suffix)s'),
            kind = cms.untracked.string('TH2F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('SuperCrystal'),
            description = cms.untracked.string('Occupancy for rec hits with GOOD reconstruction flag and E > ' + str(recHitThreshold) + ' GeV.')
        ),
        RecHitAll = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT rec hit occupancy%(suffix)s'),
            kind = cms.untracked.string('TH2F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('Crystal'),
            description = cms.untracked.string('Rec hit occupancy.')
        ),
        RecHitProjPhi = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT rec hit occupancy%(suffix)s projection phi'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjPhi'),
            description = cms.untracked.string('Projection of the rec hit occupancy.')
        ),
        RecHitThrProjPhi = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT rec hit thr occupancy%(suffix)s projection phi'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjPhi'),
            description = cms.untracked.string('Projection of the occupancy of rec hits with GOOD reconstruction flag and E > ' + str(recHitThreshold) + ' GeV.')
        ),
        TPDigiProjPhi = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sOccupancyTask/%(prefix)sOT TP digi occupancy%(suffix)s projection phi'),
            kind = cms.untracked.string('TH1F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('ProjPhi'),
            description = cms.untracked.string('Projection of TP digi occupancy.')
        )
    )
)

