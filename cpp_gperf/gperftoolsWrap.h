#ifndef __GPERFTOOLSWRAP_H__
#define __GPERFTOOLSWRAP_H__

#include <chrono>
#include <iostream>
// #include <map>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

#include <gperftools/heap-checker.h>
#include <gperftools/malloc_extension.h>
#include <gperftools/profiler.h>

namespace kanon {

bool WriteProfFile(const char* filePath, std::string&& content);

std::string GetNowTimeStamp();

std::string GetHeapName();

std::string GetCPUProfilerName();

std::string GetHeapGrowProfilerName();

void DumpTcmallocProperty();

void DumpTcmallocEnvVar();

std::string GetEnvVar(const std::string& key);

class gperftoolsWrapper {
  public:
    // delete copy and move
    gperftoolsWrapper(const gperftoolsWrapper& rhs) = delete;
    gperftoolsWrapper& operator=(const gperftoolsWrapper& rhs) = delete;
    gperftoolsWrapper(gperftoolsWrapper&& rhs)                 = delete;
    gperftoolsWrapper& operator=(gperftoolsWrapper&& rhs) = delete;

    static gperftoolsWrapper& instance();

    // interface for public
    std::string GetEnvVar(const std::string& key);
    size_t      GetPropertyValue(const std::string& key);
    void        SetPropertyVlaue(const std::string& key, size_t value);

    // basic interface for env var
    int    GetSampleParameter();
    double GetTcmallocReleaseRate();
    void   SetTcmallocReleaseRate(double rate);
    void   ReleaseFreeMemory();

    // write content to file
    bool WriteProfFile(const char* filePath, std::string&& content);

    // Dump information
    void                                         DumpTcmallocEnvVar();
    std::unordered_map<std::string, std::string> DumpTcmallocEnvVar_string();
    void                                         DumpTcmallocProperty();
    std::unordered_map<std::string, size_t>      DumpTcmallocPorperty_string();

    // Heap Simple
    void HeapSample();

    // Heap Growth Stacks
    void HeapGrowthStacks();

    // Heap StatS
    std::string HeapStats();

    // CPU Perfcpu
    void StartCpuProfiler();

    void EndCpuProfiler();

    // Logger
    void LogError(const std::string& log_msg);
    void LogInfo(const std::string& log_msg);

  private:
    // construct function
    gperftoolsWrapper();
    ~gperftoolsWrapper() = default;

    // create fileName
    std::string GetNowTimeStamp();
    std::string GetHeapName();
    std::string GetCPUProfilerName();
    std::string GetHeapGrowProfilerName();

    // static member
    static std::vector<std::string> TcmallocProperty;
    static std::vector<std::string> TcmallocEnvVar;

    // store var information
    std::unordered_map<std::string, std::string> EnvVar_Map;
    std::unordered_map<std::string, size_t>      Property_Map;

    // cpu and heap profile name
    std::string CPUProfileName     = "";
    std::string HeapProfileName    = "";
    std::string HeapGrowthfileName = "";
};

};  // namespace kanon

#endif