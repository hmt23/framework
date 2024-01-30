set(ARCANE_SOURCES
  ArcaneStdRegisterer.cc
  ArcaneStdRegisterer.h
  ArcaneBasicVerifierService.cc
  BasicCheckpointService.cc
  BasicGenericReader.cc
  BasicGenericWriter.cc
  BasicReader.cc
  BasicWriter.cc
  BasicReaderWriter.cc
  BasicReaderWriterDatabase.cc
  ParallelDataReader.cc
  ParallelDataReader.h
  ParallelDataWriter.cc
  ParallelDataWriter.h
  TextReader.cc
  TextReader.h
  TextWriter.cc
  TextWriter.h
  Hdf5Utils.h
  Hdf5VariableInfoBase.h
  Hdf5ReaderWriter.h
  Hdf5MpiReaderWriter.h
  HiredisAdapter.cc
  HoneyCombMeshGenerator.cc
  HoneyCombMeshGenerator.h
  ArcaneCurveWriter.cc
  ArcaneLoadBalanceModule.cc
  DumpW.cc
  DumpW.h
  DumpWEnsight7.cc
  DumpWUCD.cc
  FileHashDatabase.cc
  RedisHashDatabase.cc
  HashAlgorithmServices.cc
  JsonMessagePassingProfilingService.h
  JsonMessagePassingProfilingService.cc
  MeshGeneratorService.cc
  SodMeshGenerator.cc
  SodMeshGenerator.h
  SimpleMeshGenerator.cc
  SimpleMeshGenerator.h
  CartesianMeshGenerator.cc
  CartesianMeshGenerator.h
  ArcanePostProcessingModule.cc
  ArcaneCheckpointModule.cc
  ArcaneDirectExecution.cc
  ArcaneCasePartitioner.cc
  ArcaneMeshConverter.cc
  VtkMeshIOService.cc
  VoronoiMeshIOService.cc
  MeshPartitionerBase.cc
  MeshPartitionerBase.h
  PapiPerformanceService.h
  ProfilingInfo.cc
  ProfilingInfo.h
  MasterModule.cc
  UnitTestModule.cc
  ArcaneStdRegisterer.h
  ParallelDataReader.h
  ParallelDataWriter.h
  TextReader.h
  TextWriter.h
  Hdf5Utils.h
  Hdf5VariableInfoBase.h
  Hdf5ReaderWriter.h
  Hdf5MpiReaderWriter.h
  DumpW.h
  SodMeshGenerator.h
  SimpleMeshGenerator.h
  CartesianMeshGenerator.h
  MeshPartitionerBase.h
  PapiPerformanceService.h
  ProfilingInfo.h
  ArcaneSession.h
  Hdf5VariableInfoBase.h
  PartitionConverter.h
  GraphDistributor.h
  IMeshGenerator.h
  SodMeshGenerator.h
  SimpleMeshGenerator.h
  CartesianMeshGenerator.h

  PDESRandomNumberGeneratorService.cc
  PDESRandomNumberGeneratorService.h

  SimpleCsvComparatorService.cc
  SimpleCsvComparatorService.h

  SimpleCsvOutputService.cc
  SimpleCsvOutputService.h

  SimpleCsvReaderWriter.cc
  SimpleCsvReaderWriter.h

  SimpleTableInternalComparator.cc
  SimpleTableInternalComparator.h

  SimpleTableInternalMng.cc
  SimpleTableInternalMng.h

  SimpleTableWriterHelper.cc
  SimpleTableWriterHelper.h

  VariableDataInfo.cc

  MshMeshReader.cc
  MshParallelMeshReader.cc
  MshMeshWriter.cc

  internal/IosFile.cc
  internal/IosFile.h
  internal/IosGmsh.h
  internal/VtkCellTypes.h
  internal/VtkCellTypes.cc

  internal/SodStandardGroupsBuilder.h
  internal/SodStandardGroupsBuilder.cc

  internal/IHashDatabase.h
  internal/IRedisContext.h
  internal/BasicReaderWriter.h
  internal/BasicReaderWriterDatabase.h
  internal/BasicReader.h
  internal/BasicWriter.h
  internal/VariableDataInfo.h
)

set(AXL_FILES
  EnsightHdfPostProcessor
  ArcaneBasicCheckpoint
  ArcaneLoadBalance
  Ensight7PostProcessor
  ArcanePostProcessing
  ArcaneCheckpoint
  ArcaneDirectExecution
  ArcaneCasePartitioner
  ArcaneMeshConverter
  MetisMeshPartitioner
  ZoltanMeshPartitioner
  PTScotchMeshPartitioner
  Master
  UnitTest
  Cartesian2DMeshGenerator
  Cartesian3DMeshGenerator
  Sod3DMeshGenerator
  HoneyComb2DMeshGenerator
  HoneyComb3DMeshGenerator
  PDESRandomNumberGenerator
  SimpleCsvOutput
  SimpleCsvComparator
  VtkHdfPostProcessor
  VtkHdfV2PostProcessor
  VtkPolyhedralMeshIO
  )
