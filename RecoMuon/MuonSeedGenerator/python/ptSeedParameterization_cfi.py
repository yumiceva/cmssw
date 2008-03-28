import FWCore.ParameterSet.Config as cms

# These are the {p0, p1, p2, ep0, ep1, ep2} parameters for the various
# segment combinations
ptSeedParameterization = cms.PSet(
    dt1030 = cms.vdouble(0.3419, 0.0659, -0.1345, 0.1402, -0.1069, 0.1753),
    csc2040 = cms.vdouble(0.0157, 0.0156, -0.0015, 1.3456, 0.0, -0.056),
    over13 = cms.vdouble(0.6929, -0.4361, 0.1091, 0.1757),
    over12 = cms.vdouble(1.065, -0.8274, 0.3208, -0.1192),
    # Overlap  
    over11 = cms.vdouble(1.065, -0.8274, 0.3208, -0.1192),
    csc1131 = cms.vdouble(1.0537, -0.5768, 0.0855, -0.1875, 0.2202, 0.0222),
    csc1132 = cms.vdouble(-0.6416, 0.9726, -0.2973, 2.0256, -2.0803, 0.6333),
    # CSC
    csc1112 = cms.vdouble(0.8348, -0.4091, 0.0, 0.25, 0.0, 0.0),
    csc1232 = cms.vdouble(-0.6416, 0.9726, -0.2973, 2.0256, -2.0803, 0.6333),
    csc1231 = cms.vdouble(1.0537, -0.5768, 0.0855, -0.1875, 0.2202, 0.0222),
    dt2030 = cms.vdouble(0.1398, 0.0286, -0.068, 0.1908, -0.0914, 0.1851),
    csc2030 = cms.vdouble(-0.0795, 0.114, -0.0288, 6.4577, -6.0346, 1.5801),
    csc1141 = cms.vdouble(1.0419, -0.5709, 0.0876, 1.1362, -1.0226, 0.3227),
    dt3040 = cms.vdouble(0.047, 0.01, -0.0242, 0.5455, -0.1407, 0.3828),
    csc1122 = cms.vdouble(-0.5474, 0.862, -0.2794, 3.4666, -4.3546, 1.4666),
    csc1121 = cms.vdouble(0.7782, 0.3524, 0.0337, 1.778, -1.7289, 0.4915),
    dt1040 = cms.vdouble(0.3892, 0.0502, -0.118, 0.1712, -0.01, -0.0658),
    # DT
    dt1020 = cms.vdouble(0.1997, 0.0569, -0.0928, 0.1195, -0.0424, 0.0838),
    csc3040 = cms.vdouble(0.0519, -0.0537, 0.0156, 23.241, -15.425, 2.6788),
    csc1221 = cms.vdouble(0.7782, 0.3524, 0.0337, 1.778, -1.7289, 0.4915),
    csc1222 = cms.vdouble(-0.5474, 0.862, -0.2794, 3.4666, -4.3546, 1.4666),
    csc1241 = cms.vdouble(1.0419, -0.5709, 0.0876, 1.1362, -1.0226, 0.3227),
    dt2040 = cms.vdouble(0.1864, 0.0356, -0.0801, 0.2709, -0.1385, 0.2557),
    over21 = cms.vdouble(0.6283, -0.546, 0.5499, -0.2569)
)

