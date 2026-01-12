# CheckAtomic.cmake - 检查原子操作支持并配置 libatomic 链接
#
# 此模块用于检测目标架构是否需要 libatomic 库来支持小尺寸原子操作
# 主要针对 RISC-V 64/32 位、ARM 32 位等缺乏硬件原子操作支持的架构

include(CheckCXXSourceCompiles)

# 测试代码：检查小尺寸原子操作是否可用
set(ATOMIC_TEST_CODE "
#include <atomic>
#include <cstdint>
int main() {
    std::atomic<uint8_t> a{0};
    std::atomic<uint16_t> b{0};
    a.store(1);
    b.store(1);
    return a.load() + b.load();
}
")

# 检查是否需要 libatomic
function(check_atomic_support)
    # 首先尝试编译测试代码
    check_cxx_source_compiles("${ATOMIC_TEST_CODE}" ATOMIC_WORKS_WITHOUT_LIBATOMIC)

    if(NOT ATOMIC_WORKS_WITHOUT_LIBATOMIC)
        # 原子操作测试失败，尝试查找 libatomic 库
        find_library(ATOMIC_LIBRARY atomic)

        if(ATOMIC_LIBRARY)
            set(NEED_LIBATOMIC TRUE CACHE INTERNAL "Whether libatomic is needed")
            set(ATOMIC_LIBRARY_PATH ${ATOMIC_LIBRARY} CACHE INTERNAL "Path to libatomic library")
            message(STATUS "Found libatomic: ${ATOMIC_LIBRARY} (required for atomic operations on this architecture)")
        else()
            message(WARNING "libatomic not found. Atomic operations may not work correctly on this architecture.")
            message(STATUS "  Consider installing libatomic (usually part of gcc runtime libraries)")
            message(STATUS "  On Ubuntu/Debian: sudo apt install libatomic1")
            message(STATUS "  On CentOS/RHEL: sudo yum install libatomic")
            message(STATUS "  On Arch Linux: sudo pacman -S gcc-libs")
            set(NEED_LIBATOMIC FALSE CACHE INTERNAL "Whether libatomic is needed")
        endif()
    else()
        message(STATUS "Native atomic operations available (no libatomic needed)")
        set(NEED_LIBATOMIC FALSE CACHE INTERNAL "Whether libatomic is needed")
    endif()
endfunction()

# 配置目标的原子操作链接
function(configure_atomic_linking target_name)
    if(NEED_LIBATOMIC)
        target_link_libraries(${target_name} PRIVATE ${ATOMIC_LIBRARY_PATH})
        message(STATUS "Linking ${target_name} with libatomic for atomic operation support")
    endif()
endfunction()

# 自动检查并配置（便捷函数）
function(check_and_configure_atomic target_name)
    check_atomic_support()
    configure_atomic_linking(${target_name})
endfunction()
