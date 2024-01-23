import std_modules;

#include <benchmark/benchmark.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static constexpr std::size_t BUF_SIZE = 1 << 21;

static void LoadVector(benchmark::State& state) {
  for (auto _ : state) {
    std::ifstream ifs("./large_array.raw", std::ios::binary | std::ios::ate);

    ifs.seekg(0, std::ios::end);
    std::size_t size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<std::uint32_t> v(size / sizeof(std::uint32_t));
    ifs.read(reinterpret_cast<char*>(v.data()), size);
    benchmark::DoNotOptimize(v.data());
  }
}

BENCHMARK(LoadVector)->Iterations(10);

static void LoadRawArray(benchmark::State& state) {
  for (auto _ : state) {
    std::ifstream ifs("./large_array.raw", std::ios::binary | std::ios::ate);

    auto raw_buf = new char[BUF_SIZE];
    ifs.rdbuf()->pubsetbuf(raw_buf, BUF_SIZE);

    ifs.seekg(0, std::ios::end);
    std::size_t size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<std::uint32_t> v(size / sizeof(std::uint32_t));
    ifs.read(reinterpret_cast<char*>(v.data()), size);
    benchmark::DoNotOptimize(v.data());
    delete[] raw_buf;
  }
}

BENCHMARK(LoadRawArray)->Iterations(10);

static void LoadVectorWithChangeBufSize(benchmark::State& state) {
  for (auto _ : state) {
    std::ifstream ifs("./large_array.raw", std::ios::binary | std::ios::ate);

    ifs.seekg(0, std::ios::end);
    std::size_t size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    auto v = new std::uint32_t[size / sizeof(std::uint32_t)];
    ifs.read(reinterpret_cast<char*>(v), size);
    benchmark::DoNotOptimize(v);
    delete[] v;
  }
}

BENCHMARK(LoadVectorWithChangeBufSize)->Iterations(10);

static void LoadRawArrayWithChangeBufSize(benchmark::State& state) {
  for (auto _ : state) {
    std::ifstream ifs("./large_array.raw", std::ios::binary | std::ios::ate);

    auto raw_buf = new char[BUF_SIZE];
    ifs.rdbuf()->pubsetbuf(raw_buf, BUF_SIZE);

    ifs.seekg(0, std::ios::end);
    std::size_t size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    auto v = new std::uint32_t[size / sizeof(std::uint32_t)];
    ifs.read(reinterpret_cast<char*>(v), size);
    benchmark::DoNotOptimize(v);
    delete[] raw_buf;
    delete[] v;
  }
}

BENCHMARK(LoadRawArrayWithChangeBufSize)->Iterations(10);

static void LoadVectorUnix(benchmark::State& state) {
  for (auto _ : state) {
    int fd = open("./large_array.raw", O_RDONLY);

    off_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    std::vector<std::uint32_t> v(size / sizeof(std::uint32_t));
    read(fd, v.data(), size);
    benchmark::DoNotOptimize(v.data());
    close(fd);
  }
}

BENCHMARK(LoadVectorUnix)->Iterations(10);

static void LoadRawArrayUnix(benchmark::State& state) {
  for (auto _ : state) {
    int fd = open("./large_array.raw", O_RDONLY);

    off_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    auto v = new std::uint32_t[size / sizeof(std::uint32_t)];
    read(fd, v, size);
    benchmark::DoNotOptimize(v);
    close(fd);

    delete[] v;
  }
}

BENCHMARK(LoadRawArrayUnix)->Iterations(10);

static void LoadRawArrayUnixNoCache(benchmark::State& state) {
  for (auto _ : state) {
    int fd = open("./large_array.raw", O_RDONLY);
    fcntl(fd, F_NOCACHE, 1);

    off_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    auto v = new std::uint32_t[size / sizeof(std::uint32_t)];
    read(fd, v, size);
    benchmark::DoNotOptimize(v);
    close(fd);

    delete[] v;
  }
}

BENCHMARK(LoadRawArrayUnixNoCache)->Iterations(10);

static void LoadRawArrayMMap(benchmark::State& state) {
  for (auto _ : state) {
    int fd = open("./large_array.raw", O_RDONLY);
    struct stat st;
    fstat(fd, &st);

    void* addr = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    benchmark::DoNotOptimize(addr);

    munmap(addr, st.st_size);
  }
}

BENCHMARK(LoadRawArrayMMap)->Iterations(10);

BENCHMARK_MAIN();
