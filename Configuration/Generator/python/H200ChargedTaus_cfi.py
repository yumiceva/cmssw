# The following comments couldn't be translated into the new config version:

# This is a vector of ParameterSet names to be read, in this order

# "TAUO = 0 0              ! Registered by Alexandre.Nikitenko@cern.ch",

# higgs decays

import FWCore.ParameterSet.Config as cms

source = cms.Source("PythiaSource",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(1.0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 'MSTJ(22)=2     ! Decay those unstable particles', 'PARJ(71)=10 .  ! for which ctau  10 mm', 'MSTP(2)=1      ! which order running alphaS', 'MSTP(33)=0     ! no K factors in hard cross sections', 'MSTP(51)=7     ! structure function chosen', 'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 'MSTP(82)=4     ! Defines the multi-parton model', 'MSTU(21)=1     ! Check on possible errors during program execution', 'PARP(82)=1.9409   ! pt cutoff for multiparton interactions', 'PARP(89)=1960. ! sqrts for which PARP82 is set', 'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 'PARP(90)=0.16  ! Multiple interactions: rescaling power', 'PARP(67)=2.5    ! amount of initial-state radiation', 'PARP(85)=1.0  ! gluon prod. mechanism in MI', 'PARP(86)=1.0  ! gluon prod. mechanism in MI', 'PARP(62)=1.25   ! ', 'PARP(64)=0.2    ! ', 'MSTP(91)=1     !', 'PARP(91)=2.1   ! kt distribution', 'PARP(93)=15.0  ! '),
        parameterSets = cms.vstring('pythiaUESettings', 'processParameters', 'pythiaMSSMmhmax'),
        processParameters = cms.vstring('MSEL = 0                ! user control', 'MSUB(401) = 1           ! gg->tbH+ Registered by Alexandre.Nikitenko@cern.ch', 'MSUB(402) = 1           ! qq->tbH+ Registered by Alexandre.Nikitenko@cern.ch', 'IMSS(1)= 1              ! MSSM ', 'RMSS(5) = 30.           ! TANBETA', 'RMSS(19) = 200.         ! (D=850.) m_A', 'MDME(503,1)=0           !Higgs(H+) decay into dbar            u', 'MDME(504,1)=0           !Higgs(H+) decay into sbar            c', 'MDME(505,1)=0           !Higgs(H+) decay into bbar            t', 'MDME(506,1)=0           !Higgs(H+) decay into b bar           t', 'MDME(507,1)=0           !Higgs(H+) decay into e+              nu_e', 'MDME(508,1)=0           !Higgs(H+) decay into mu+             nu_mu', 'MDME(509,1)=1           !Higgs(H+) decay into tau+            nu_tau', 'MDME(510,1)=0           !Higgs(H+) decay into tau prime+           nu_tau', 'MDME(511,1)=0           !Higgs(H+) decay into W+              h0', 'MDME(512,1)=0           !Higgs(H+) decay into ~chi_10         ~chi_1+', 'MDME(513,1)=0           !Higgs(H+) decay into ~chi_10         ~chi_2+', 'MDME(514,1)=0           !Higgs(H+) decay into ~chi_20         ~chi_1+', 'MDME(515,1)=0           !Higgs(H+) decay into ~chi_20         ~chi_2+', 'MDME(516,1)=0           !Higgs(H+) decay into ~chi_30         ~chi_1+', 'MDME(517,1)=0           !Higgs(H+) decay into ~chi_30         ~chi_2+', 'MDME(518,1)=0           !Higgs(H+) decay into ~chi_40         ~chi_1+', 'MDME(519,1)=0           !Higgs(H+) decay into ~chi_40         ~chi_2+', 'MDME(520,1)=0           !Higgs(H+) decay into ~t_1            ~b_1bar', 'MDME(521,1)=0           !Higgs(H+) decay into ~t_2            ~b_1bar', 'MDME(522,1)=0           !Higgs(H+) decay into ~t_1            ~b_2bar', 'MDME(523,1)=0           !Higgs(H+) decay into ~t_2            ~b_2bar', 'MDME(524,1)=0           !Higgs(H+) decay into ~d_Lbar         ~u_L', 'MDME(525,1)=0           !Higgs(H+) decay into ~s_Lbar         ~c_L', 'MDME(526,1)=0           !Higgs(H+) decay into ~e_L+           ~nu_eL', 'MDME(527,1)=0           !Higgs(H+) decay into ~mu_L+          ~nu_muL', 'MDME(528,1)=0           !Higgs(H+) decay into ~tau_1+         ~nu_tauL', 'MDME(529,1)=0           !Higgs(H+) decay into ~tau_2+         ~nu_tauL', 'MDME(89,1) = 0          ! no tau->electron', 'MDME(90,1) = 0          ! no tau->muon'),
        pythiaMSSMmhmax = cms.vstring('RMSS(2)= 200.           ! SU(2) gaugino mass ', 'RMSS(3)= 800.           ! SU(3) (gluino) mass ', 'RMSS(4)= 200.           ! higgsino mass parameter', 'RMSS(6)= 1000.          ! left slepton mass', 'RMSS(7)= 1000.          ! right slepton mass', 'RMSS(8)= 1000.          ! right slepton mass', 'RMSS(9)= 1000.          ! right squark mass', 'RMSS(10)= 1000.         ! left sq mass for 3th gen/heaviest stop mass', 'RMSS(11)= 1000.         ! right sbottom mass/lightest sbotoom mass', 'RMSS(12)= 1000.         ! right stop mass/lightest stop mass', 'RMSS(13)= 1000.         ! left stau mass', 'RMSS(14)= 1000.         ! right stau mass', 'RMSS(15)= 2449.         ! Ab', 'RMSS(16)= 2449.         ! At', 'RMSS(17)= 2449.         ! Atau')
    )
)



