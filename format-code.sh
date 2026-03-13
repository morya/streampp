#!/bin/bash

# 代码格式化脚本
# 自动格式化项目中的C++/Qt代码

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 检查命令是否存在
check_command() {
    if ! command -v "$1" &> /dev/null; then
        print_error "命令 '$1' 未找到，请先安装"
        return 1
    fi
    return 0
}

# 格式化C++文件
format_cpp_files() {
    print_info "开始格式化C++文件..."
    
    # 查找所有C++源文件和头文件
    local cpp_files=$(find src -name "*.cpp" -o -name "*.h" | grep -v build | grep -v ".git")
    
    if [ -z "$cpp_files" ]; then
        print_warning "未找到C++文件"
        return 0
    fi
    
    print_info "找到 $(echo "$cpp_files" | wc -l) 个C++文件"
    
    # 使用clang-format格式化
    if check_command "clang-format"; then
        print_info "使用clang-format格式化..."
        echo "$cpp_files" | xargs -I {} clang-format -i -style=file {}
        print_success "C++文件格式化完成"
    else
        print_warning "clang-format未安装，跳过C++文件格式化"
    fi
}

# 格式化CMake文件
format_cmake_files() {
    print_info "开始格式化CMake文件..."
    
    local cmake_files=$(find . -name "CMakeLists.txt" -o -name "*.cmake" | grep -v build | grep -v ".git")
    
    if [ -z "$cmake_files" ]; then
        print_warning "未找到CMake文件"
        return 0
    fi
    
    print_info "找到 $(echo "$cmake_files" | wc -l) 个CMake文件"
    
    # 使用cmake-format格式化
    if check_command "cmake-format"; then
        print_info "使用cmake-format格式化..."
        echo "$cmake_files" | xargs -I {} cmake-format -i {}
        print_success "CMake文件格式化完成"
    else
        print_warning "cmake-format未安装，跳过CMake文件格式化"
    fi
}

# 检查代码格式
check_format() {
    print_info "检查代码格式..."
    
    local has_errors=0
    
    # 检查C++文件格式
    if check_command "clang-format"; then
        local cpp_files=$(find src -name "*.cpp" -o -name "*.h" | grep -v build | grep -v ".git")
        
        if [ -n "$cpp_files" ]; then
            print_info "检查C++文件格式..."
            local output=$(echo "$cpp_files" | xargs -I {} clang-format -style=file -output-replacements-xml {} | grep -c "replacement ")
            
            if [ "$output" -gt 0 ]; then
                print_error "发现 $output 个C++文件需要格式化"
                has_errors=1
            else
                print_success "所有C++文件格式正确"
            fi
        fi
    fi
    
    return $has_errors
}

# 显示帮助信息
show_help() {
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  -h, --help     显示此帮助信息"
    echo "  -f, --format   格式化所有代码文件"
    echo "  -c, --check    检查代码格式但不修改"
    echo "  -a, --all      格式化所有支持的文件类型"
    echo ""
    echo "示例:"
    echo "  $0 --format    格式化所有C++文件"
    echo "  $0 --check     检查代码格式"
    echo "  $0 --all       格式化所有支持的文件类型"
}

# 主函数
main() {
    local action="format"
    
    # 解析命令行参数
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -f|--format)
                action="format"
                shift
                ;;
            -c|--check)
                action="check"
                shift
                ;;
            -a|--all)
                action="all"
                shift
                ;;
            *)
                print_error "未知选项: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    print_info "项目根目录: $(pwd)"
    
    case $action in
        format)
            format_cpp_files
            ;;
        check)
            check_format
            ;;
        all)
            format_cpp_files
            format_cmake_files
            ;;
    esac
    
    print_success "操作完成"
}

# 运行主函数
main "$@"