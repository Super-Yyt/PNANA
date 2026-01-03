#!/bin/bash

# Shell脚本示例 - 构建脚本
# 这个脚本演示了各种Shell语法特性

# 函数定义
print_info() {
    echo -e "\033[1;34m[INFO]\033[0m $1"
}

print_success() {
    echo -e "\033[1;32m[SUCCESS]\033[0m $1"
}

print_error() {
    echo -e "\033[1;31m[ERROR]\033[0m $1"
}

print_warning() {
    echo -e "\033[1;33m[WARNING]\033[0m $1"
}

# 变量定义
PROJECT_NAME="MyApplication"
VERSION="1.0.0"
BUILD_DIR="build"
SOURCE_DIR="src"
TEST_DIR="tests"

# 数组
DEPENDENCIES=("cmake" "gcc" "make" "git")
OPTIONAL_DEPS=("doxygen" "valgrind")

# 命令行参数处理
VERBOSE=false
CLEAN=false
TEST=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -t|--test)
            TEST=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -v, --verbose    Enable verbose output"
            echo "  -c, --clean      Clean build directory"
            echo "  -t, --test       Run tests after build"
            echo "  -h, --help       Show this help message"
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# 检查依赖
check_dependencies() {
    print_info "Checking dependencies..."

    local missing_deps=()

    for dep in "${DEPENDENCIES[@]}"; do
        if ! command -v "$dep" &> /dev/null; then
            missing_deps+=("$dep")
        fi
    done

    if [ ${#missing_deps[@]} -ne 0 ]; then
        print_error "Missing required dependencies: ${missing_deps[*]}"
        return 1
    fi

    # 检查可选依赖
    for dep in "${OPTIONAL_DEPS[@]}"; do
        if command -v "$dep" &> /dev/null; then
            print_info "Found optional dependency: $dep"
        else
            print_warning "Optional dependency not found: $dep"
        fi
    done

    return 0
}

# 清理构建目录
clean_build() {
    if [ "$CLEAN" = true ]; then
        print_info "Cleaning build directory..."
        rm -rf "$BUILD_DIR"
    fi

    if [ -d "$BUILD_DIR" ]; then
        print_info "Build directory exists, cleaning..."
        rm -rf "$BUILD_DIR"/*
    fi
}

# 创建构建目录
create_build_dir() {
    print_info "Creating build directory..."
    mkdir -p "$BUILD_DIR"

    if [ ! -d "$BUILD_DIR" ]; then
        print_error "Failed to create build directory"
        return 1
    fi

    return 0
}

# 配置项目
configure_project() {
    print_info "Configuring project..."

    cd "$BUILD_DIR" || return 1

    local cmake_args=()

    if [ "$VERBOSE" = true ]; then
        cmake_args+=("-DCMAKE_VERBOSE_MAKEFILE=ON")
    fi

    # 添加其他CMake参数
    cmake_args+=("-DCMAKE_BUILD_TYPE=Release")
    cmake_args+=("-DENABLE_TESTS=ON")

    if cmake "${cmake_args[@]}" ..; then
        print_success "CMake configuration completed"
        cd ..
        return 0
    else
        print_error "CMake configuration failed"
        cd ..
        return 1
    fi
}

# 构建项目
build_project() {
    print_info "Building project..."

    cd "$BUILD_DIR" || return 1

    local make_args=()

    if [ "$VERBOSE" = true ]; then
        make_args+=("VERBOSE=1")
    fi

    # 获取CPU核心数
    local cpu_count
    cpu_count=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

    if make -j"$cpu_count" "${make_args[@]}"; then
        print_success "Build completed successfully"
        cd ..
        return 0
    else
        print_error "Build failed"
        cd ..
        return 1
    fi
}

# 运行测试
run_tests() {
    if [ "$TEST" = false ]; then
        return 0
    fi

    print_info "Running tests..."

    cd "$BUILD_DIR" || return 1

    if [ -f "CTestTestfile.cmake" ]; then
        if ctest --output-on-failure; then
            print_success "All tests passed"
            cd ..
            return 0
        else
            print_error "Some tests failed"
            cd ..
            return 1
        fi
    else
        print_warning "No test configuration found"
        cd ..
    fi

    return 0
}

# 安装项目
install_project() {
    print_info "Installing project..."

    cd "$BUILD_DIR" || return 1

    if sudo make install; then
        print_success "Installation completed"
        cd ..
        return 0
    else
        print_error "Installation failed"
        cd ..
        return 1
    fi
}

# 主函数
main() {
    print_info "Starting build process..."
    print_info "Project root: $(pwd)"
    print_info "Build directory: $BUILD_DIR"

    # 检查依赖
    if ! check_dependencies; then
        exit 1
    fi

    # 清理
    clean_build

    # 创建构建目录
    if ! create_build_dir; then
        exit 1
    fi

    # 配置
    if ! configure_project; then
        exit 1
    fi

    # 构建
    if ! build_project; then
        exit 1
    fi

    # 测试
    if ! run_tests; then
        exit 1
    fi

    # 安装
    if ! install_project; then
        exit 1
    fi

    print_success "Build process completed successfully!"
    print_info "You can now run: $PROJECT_NAME --version"
}

# 脚本入口
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
