# For Data Trigger
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

process = cms.Process("TagAndProbeData")

process.load("FWCore.MessageService.MessageLogger_cfi")

# Set the maximum number of events to be processed (-1 processes all events)
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(10000))

# Define files of dataset
files = FileUtils.loadListFromFile("data/CMS_Run2011B_SingleMu_AOD_12Oct2013-v1_10000_file_index.txt")

process.source = cms.Source(
    "PoolSource", fileNames=cms.untracked.vstring(*files))


goodJSON = 'data/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess.extend(myLumis)

# Number of events to be skipped (0 by default)
process.source.skipEvents = cms.untracked.uint32(0)

process.TFileService = cms.Service ('TFileService',
    fileName = cms.string (
    'TagAndProbeSingleMu2.root'
    )
)
  
process.tagandprobe = cms.EDAnalyzer('TagAndProbe2019',
	verbose = cms.bool(False),
	triggerflag = cms.bool(True),		      
	# Trigger
	TriggerResultsTag = cms.untracked.InputTag("TriggerResults", "", "HLT"),
	TriggerEventTag = cms.untracked.InputTag("hltTriggerSummaryAOD", "", "HLT"),
	PathName = cms.untracked.vstring(["HLT_IsoMu24_eta2p1"]),
	# RECO Labels
	primaryVertexProducer = cms.InputTag("offlinePrimaryVertices"),
	recoMuonsLabel = cms.InputTag("muons"), 
	# RECO Configs
	minResonanceMass = cms.double(70.),# in GeV
	maxResonanceMass = cms.double(120.)# in GeV 
)


process.p = cms.Path(process.tagandprobe)
