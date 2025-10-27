# ===== COOP Benchmarks =====
project(coop_benchmarks)


set(BenchSources
    "coop_bench_queue.c"
   
)

add_executable(coop_benchmarks_short ${BenchSources})
target_link_libraries(coop_benchmarks_short PRIVATE COOP)


if(TARGET COOP_testing)
    add_executable(coop_benchmarks_int ${BenchSources})
    target_link_libraries(coop_benchmarks_int PRIVATE COOP_testing)
endif()



