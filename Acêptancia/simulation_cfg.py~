import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
process = cms.Process("AOD2NanoAOD")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = "WARNING"
process.MessageLogger.categories.append("AOD2NanoAOD")
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit=cms.untracked.int32(10))
process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

# Set the maximum number of events to be processed (-1 processes all events)
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(1000))

# Define files of dataset
files = FileUtils.loadListFromFile("data/CMS_MonteCarlo2012_Summer12_DR53X_DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_AODSIM_PU_RD1_START53_V7N-v1_20000_file_index.txt")
#files.extend(FileUtils.loadListFromFile("data/CMS_MonteCarlo2012_Summer12_DR53X_TTbar_8TeV-Madspin_aMCatNLO-herwig_AODSIM_PU_S10_START53_V19-v2_20000_file_index.txt"))

process.source = cms.Source(
    "PoolSource", fileNames=cms.untracked.vstring(*files))

# Set global tag
# We don't have set the global tag for the educational samples. This simplifies running the code since we don't have to access the database.
#process.GlobalTag.globaltag = "START53_LV6A1::All"

# Number of events to be skipped (0 by default)
process.source.skipEvents = cms.untracked.uint32(0)

# Register fileservice for output file
process.aod2nanoaod = cms.EDAnalyzer("AOD2NanoAOD", isData = cms.bool(False))
process.TFileService = cms.Service(
    "TFileService", fileName=cms.string("testeTagAndProg.root"))

process.p = cms.Path(process.aod2nanoaod)
