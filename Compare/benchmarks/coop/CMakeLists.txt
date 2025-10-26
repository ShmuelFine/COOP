# ===== COOP Benchmarks =====
project(coop_benchmarks)


set(BenchSources
    #"coop_bench_queue.c"
    #"bench_list_ops.c"
    "coop_bench_binary_tree.c"
)

add_executable(${PROJECT_NAME} ${BenchSources})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/COOP/COOP
)

target_link_libraries(${PROJECT_NAME} PRIVATE COOP_testing)



