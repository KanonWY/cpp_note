#include "gperftoolsWrap.h"

namespace kanon {

// gperftoolsWrapper

// Init static var
std::vector<std::string> gperftoolsWrapper::TcmallocProperty{
    "generic.current_allocated_bytes",
    "generic.heap_size",
    "tcmalloc.pageheap_free_bytes",
    "tcmalloc.pageheap_unmapped_bytes",
    "tcmalloc.slack_bytes",
    "tcmalloc.max_total_thread_cache_bytes",
    "tcmalloc.current_total_thread_cache_bytes"};

std::vector<std::string> gperftoolsWrapper::TcmallocEnvVar{
    "TCMALLOC_SAMPLE_PARAMETER", "TCMALLOC_RELEASE_RATE",
    "TCMALLOC_LARGE_ALLOC_REPORT_THRESHOLD",
    "TCMALLOC_MAX_TOTAL_THREAD_CACHE_BYTES"};

// construct function
gperftoolsWrapper::gperftoolsWrapper()
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext != nullptr) {
        // 1. EenVar;
        for (auto item : TcmallocEnvVar) {
            char* env_val = getenv(item.c_str());
            if (env_val != nullptr) {
                EnvVar_Map[item] = env_val;
            }
            else {
                EnvVar_Map[item] = "";
            }
        }
        // 2.Property;
        for (auto item : TcmallocProperty) {
            size_t PropertyNumber = 0;
            malloc_ext->GetNumericProperty(item.c_str(), &PropertyNumber);
            Property_Map[item] = PropertyNumber;
        }
    }
    else {
        std::cerr << "MallocExtension::instance() error" << std::endl;
    }
}

gperftoolsWrapper& gperftoolsWrapper::instance()
{
    static gperftoolsWrapper s_instance;
    return s_instance;
}

std::string gperftoolsWrapper::GetEnvVar(const std::string& key)
{
    return EnvVar_Map[key];
}
size_t gperftoolsWrapper::GetPropertyValue(const std::string& key)
{
    return Property_Map[key];
}
void gperftoolsWrapper::SetPropertyVlaue(const std::string& key, size_t value)
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext != nullptr) {
        malloc_ext->SetNumericProperty(key.c_str(), value);
        Property_Map[key] = value;
    }
    else {
        std::cerr << "MallocExtension::instance() error" << std::endl;
    }
}

// basic interface for env var
int gperftoolsWrapper::GetSampleParameter()
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext != nullptr) {
        int Number = 0;
        malloc_ext->ReadStackTraces(&Number);
        return Number;
    }
    else {
        LogError("MallocExtension::instance() error");
        return -1;
    }
}

double gperftoolsWrapper::GetTcmallocReleaseRate()
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext != nullptr) {
        double rate = malloc_ext->GetMemoryReleaseRate();
        return rate;
    }
    else {
        LogError("MallocExtension::instance() error");
        return -1;
    }
}

void gperftoolsWrapper::SetTcmallocReleaseRate(double rate)
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext != nullptr) {
        malloc_ext->SetMemoryReleaseRate(rate);
    }
    else {
        LogError("MallocExtension::instance() error");
    }
}

void gperftoolsWrapper::ReleaseFreeMemory()
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext != nullptr) {
        malloc_ext->ReleaseFreeMemory();
    }
    else {
        LogError("MallocExtension::instance() error");
    }
}

bool gperftoolsWrapper::WriteProfFile(const char*   filePath,
                                      std::string&& content)
{
    FILE* file = fopen(filePath, "w+");
    if (!file) {
        std::cerr << "fopen error" << std::endl;
        return false;
    }
    bool ret = true;
    if (fwrite(content.data(), content.size(), 1UL, file) != 1UL) {
        std::cerr << "fwrite error " << std::endl;
        ret = false;
    }
    fclose(file);
    return ret;
}

std::string gperftoolsWrapper::GetNowTimeStamp()
{
    auto now_time = std::chrono::system_clock::now();
    auto time_s   = std::chrono::system_clock::to_time_t(now_time);
    return std::to_string(time_s);
}
std::string gperftoolsWrapper::GetHeapName()
{
    std::string TS = GetNowTimeStamp();
    TS += "_heap.prof";
    return TS;
}
std::string gperftoolsWrapper::GetCPUProfilerName()
{
    std::string TS = GetNowTimeStamp();
    TS += "_cpu.prof";
    return TS;
}
std::string gperftoolsWrapper::GetHeapGrowProfilerName()
{
    std::string TS = GetNowTimeStamp();
    TS += "_GrowHeap.prof";
    return TS;
}

// Dump information
void gperftoolsWrapper::DumpTcmallocEnvVar()
{
    printf("=============================================[EnvVar]=========="
           "============================================\n");
    for (auto item : EnvVar_Map) {
        printf("%45s %s\n", item.first.c_str(), item.second.c_str());
    }
    printf("==============================================================="
           "============================================\n");
}

std::unordered_map<std::string, std::string>
gperftoolsWrapper::DumpTcmallocEnvVar_string()
{
    return EnvVar_Map;
}

void gperftoolsWrapper::DumpTcmallocProperty()
{
    printf("=============================================[Property_Map]===="
           "============================================\n");
    for (auto it : Property_Map) {
        printf("%45s %ld\n", it.first.c_str(), it.second);
    }
    printf("==============================================================="
           "============================================\n");
}

std::unordered_map<std::string, size_t>
gperftoolsWrapper::DumpTcmallocPorperty_string()
{
    return Property_Map;
}

// logInfo

void gperftoolsWrapper::LogError(const std::string& log_msg)
{
    std::cerr << "error malloc_ext is empty " << std::endl;
}

void gperftoolsWrapper::LogInfo(const std::string& log_msg)
{
    std::cout << log_msg.c_str() << std::endl;
}

// Heap Sample
void gperftoolsWrapper::HeapSample()
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext == nullptr) {
        LogError("error malloc_ext is empty");
        return;
    }
    else {
        std::string StoreInformation;
        malloc_ext->GetHeapSample(&StoreInformation);
        if (!WriteProfFile(GetHeapName().c_str(),
                           std::move(StoreInformation))) {
            LogError("WriteProfFile Error" + std::string(__FUNCTION__));
        }
    }
}

// Heap Growth Stacks
void gperftoolsWrapper::HeapGrowthStacks()
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext == nullptr) {
        LogError("error malloc_ext is empty");
        return;
    }
    else {
        std::string StoreInformation;
        malloc_ext->GetHeapGrowthStacks(&StoreInformation);
        if (!WriteProfFile(GetHeapGrowProfilerName().c_str(),
                           std::move(StoreInformation))) {
            LogError("WriteProf error" + std::string(__FUNCTION__));
        }
    }
}

std::string gperftoolsWrapper::HeapStats()
{
    MallocExtension* malloc_ext = MallocExtension::instance();
    if (malloc_ext == nullptr) {
        LogError("error malloc_ext is empty");
        return "";
    }
    else {
        char* str = new char[4096];
        malloc_ext->GetStats(str, 4096);
        std::string out(str);
        delete[] str;
        return out;
    }
}

// CPU Perfcpu
void gperftoolsWrapper::StartCpuProfiler()
{
    std::string CPUProfileFile = GetCPUProfilerName();
    ProfilerStart(CPUProfileFile.c_str());
    LogInfo(CPUProfileFile.c_str());
}

void gperftoolsWrapper::EndCpuProfiler()
{
    ProfilerStop();
}

};  // namespace kanon