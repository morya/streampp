#!/usr/bin/env python3
"""
代码格式化脚本
支持C++/Qt项目的自动代码格式化
"""

import os
import sys
import subprocess
import argparse
from pathlib import Path
from typing import List, Tuple

# 颜色定义
class Colors:
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    NC = '\033[0m'  # No Color

def print_color(color: str, prefix: str, message: str):
    """打印带颜色的消息"""
    print(f"{color}[{prefix}]{Colors.NC} {message}")

def print_info(message: str):
    print_color(Colors.BLUE, "INFO", message)

def print_success(message: str):
    print_color(Colors.GREEN, "SUCCESS", message)

def print_warning(message: str):
    print_color(Colors.YELLOW, "WARNING", message)

def print_error(message: str):
    print_color(Colors.RED, "ERROR", message)

def check_command(command: str) -> bool:
    """检查命令是否存在"""
    try:
        subprocess.run([command, "--version"], capture_output=True, check=False)
        return True
    except (subprocess.SubprocessError, FileNotFoundError):
        return False

def find_files(patterns: List[str], exclude_dirs: List[str] = None) -> List[Path]:
    """查找匹配模式的文件"""
    if exclude_dirs is None:
        exclude_dirs = ["build", ".git", "__pycache__", "node_modules"]
    
    files = []
    project_root = Path.cwd()
    
    for pattern in patterns:
        for file_path in project_root.rglob(pattern):
            # 检查是否在排除目录中
            if any(exclude_dir in str(file_path) for exclude_dir in exclude_dirs):
                continue
            files.append(file_path)
    
    return files

def format_cpp_files(dry_run: bool = False) -> Tuple[int, int]:
    """格式化C++文件"""
    print_info("开始处理C++文件...")
    
    cpp_files = find_files(["*.cpp", "*.h", "*.hpp"])
    
    if not cpp_files:
        print_warning("未找到C++文件")
        return 0, 0
    
    print_info(f"找到 {len(cpp_files)} 个C++文件")
    
    if not check_command("clang-format"):
        print_warning("clang-format未安装，跳过C++文件格式化")
        print_info("安装命令: brew install clang-format (macOS) 或 apt-get install clang-format (Linux)")
        return 0, len(cpp_files)
    
    formatted_count = 0
    error_count = 0
    
    for file_path in cpp_files:
        try:
            if dry_run:
                # 检查格式但不修改
                result = subprocess.run(
                    ["clang-format", "--style=file", "--dry-run", "-Werror", str(file_path)],
                    capture_output=True,
                    text=True
                )
                if result.returncode != 0:
                    print_warning(f"需要格式化: {file_path}")
                    error_count += 1
            else:
                # 实际格式化
                subprocess.run(
                    ["clang-format", "-i", "--style=file", str(file_path)],
                    check=True,
                    capture_output=True
                )
                formatted_count += 1
        except subprocess.CalledProcessError as e:
            print_error(f"格式化失败: {file_path}")
            error_count += 1
    
    if dry_run:
        if error_count == 0:
            print_success("所有C++文件格式正确")
        else:
            print_warning(f"发现 {error_count} 个C++文件需要格式化")
    else:
        print_success(f"成功格式化 {formatted_count} 个C++文件")
    
    return formatted_count, error_count

def format_cmake_files(dry_run: bool = False) -> Tuple[int, int]:
    """格式化CMake文件"""
    print_info("开始处理CMake文件...")
    
    cmake_files = find_files(["CMakeLists.txt", "*.cmake"])
    
    if not cmake_files:
        print_warning("未找到CMake文件")
        return 0, 0
    
    print_info(f"找到 {len(cmake_files)} 个CMake文件")
    
    if not check_command("cmake-format"):
        print_warning("cmake-format未安装，跳过CMake文件格式化")
        print_info("安装命令: pip install cmake-format")
        return 0, len(cmake_files)
    
    formatted_count = 0
    error_count = 0
    
    for file_path in cmake_files:
        try:
            if dry_run:
                # 检查格式但不修改
                result = subprocess.run(
                    ["cmake-format", "--check", str(file_path)],
                    capture_output=True,
                    text=True
                )
                if result.returncode != 0:
                    print_warning(f"需要格式化: {file_path}")
                    error_count += 1
            else:
                # 实际格式化
                subprocess.run(
                    ["cmake-format", "-i", str(file_path)],
                    check=True,
                    capture_output=True
                )
                formatted_count += 1
        except subprocess.CalledProcessError as e:
            print_error(f"格式化失败: {file_path}")
            error_count += 1
    
    if dry_run:
        if error_count == 0:
            print_success("所有CMake文件格式正确")
        else:
            print_warning(f"发现 {error_count} 个CMake文件需要格式化")
    else:
        print_success(f"成功格式化 {formatted_count} 个CMake文件")
    
    return formatted_count, error_count

def create_clang_format_config() -> bool:
    """创建.clang-format配置文件"""
    config_path = Path(".clang-format")
    
    if config_path.exists():
        print_info(".clang-format配置文件已存在")
        return True
    
    config_content = """---
BasedOnStyle: LLVM
AccessModifierOffset: -4
AlignAfterOpenBracket: Align
AlignConsecutiveAssignments: false
AlignConsecutiveDeclarations: false
AlignEscapedNewlines: Left
AlignOperands: true
AlignTrailingComments: true
AllowAllArgumentsOnNextLine: false
AllowAllConstructorInitializersOnNextLine: false
AllowAllParametersOfDeclarationOnNextLine: false
AllowShortBlocksOnASingleLine: Never
AllowShortFunctionsOnASingleLine: None
AllowShortIfStatementsOnASingleLine: Never
AllowShortLambdasOnASingleLine: All
AllowShortLoopsOnASingleLine: false
AlwaysBreakAfterDefinitionReturnType: None
AlwaysBreakAfterReturnType: None
AlwaysBreakBeforeMultilineStrings: false
AlwaysBreakTemplateDeclarations: MultiLine
BinPackArguments: true
BinPackParameters: true
BraceWrapping:
  AfterCaseLabel: false
  AfterClass: false
  AfterControlStatement: Never
  AfterEnum: false
  AfterFunction: false
  AfterNamespace: false
  AfterObjCDeclaration: false
  AfterStruct: false
  AfterUnion: false
  AfterExternBlock: false
  BeforeCatch: false
  BeforeElse: false
  IndentBraces: false
  SplitEmptyFunction: false
  SplitEmptyRecord: false
  SplitEmptyNamespace: false
BreakBeforeBinaryOperators: None
BreakBeforeBraces: Attach
BreakBeforeInheritanceList: false
BreakBeforeTernaryOperators: true
BreakConstructorInitializers: BeforeColon
BreakStringLiterals: true
ColumnLimit: 120
CommentPragmas: '^ IWYU pragma:'
CompactNamespaces: false
ConstructorInitializerAllOnOneLineOrOnePerLine: false
ConstructorInitializerIndentWidth: 4
ContinuationIndentWidth: 4
Cpp11BracedListStyle: true
DeriveLineEnding: true
DerivePointerAlignment: false
DisableFormat: false
EmptyLineAfterAccessModifier: Never
EmptyLineBeforeAccessModifier: LogicalBlock
ExperimentalAutoDetectBinPacking: false
FixNamespaceComments: true
ForEachMacros:
  - foreach
  - Q_FOREACH
  - BOOST_FOREACH
IncludeBlocks: Regroup
IncludeCategories:
  - Regex:           '^<.*\.h>'
    Priority:        1
  - Regex:           '^<.*'
    Priority:        2
  - Regex:           '.*'
    Priority:        3
IncludeIsMainRegex: '(Test)?$'
IndentCaseLabels: true
IndentCaseBlocks: false
IndentGotoLabels: true
IndentPPDirectives: None
IndentExternBlock: AfterExternBlock
IndentWidth: 4
IndentWrappedFunctionNames: false
KeepEmptyLinesAtTheStartOfBlocks: false
MacroBlockBegin: ''
MacroBlockEnd: ''
MaxEmptyLinesToKeep: 1
NamespaceIndentation: None
ObjCBinPackProtocolList: Auto
ObjCBlockIndentWidth: 4
ObjCSpaceAfterProperty: false
ObjCSpaceBeforeProtocolList: true
PenaltyBreakAssignment: 2
PenaltyBreakBeforeFirstCallParameter: 19
PenaltyBreakComment: 300
PenaltyBreakFirstLessLess: 120
PenaltyBreakString: 1000
PenaltyBreakTemplateDeclaration: 10
PenaltyExcessCharacter: 1000000
PenaltyReturnTypeOnItsOwnLine: 60
PointerAlignment: Left
ReflowComments: true
SortIncludes: true
SortUsingDeclarations: true
SpaceAfterCStyleCast: false
SpaceAfterLogicalNot: false
SpaceAfterTemplateKeyword: true
SpaceBeforeAssignmentOperators: true
SpaceBeforeCpp11BracedList: false
SpaceBeforeCtorInitializerColon: true
SpaceBeforeInheritanceColon: true
SpaceBeforeParens: ControlStatements
SpaceBeforeRangeBasedForLoopColon: true
SpaceBeforeSquareBrackets: false
SpaceInEmptyBlock: false
SpaceInEmptyParentheses: false
SpacesBeforeTrailingComments: 1
SpacesInAngles: false
SpacesInConditionalStatement: false
SpacesInContainerLiterals: true
SpacesInCStyleCastParentheses: false
SpacesInParentheses: false
SpacesInSquareBrackets: false
Standard: Cpp17
TabWidth: 4
UseTab: Never
...
"""
    
    try:
        config_path.write_text(config_content)
        print_success("已创建.clang-format配置文件")
        return True
    except Exception as e:
        print_error(f"创建.clang-format配置文件失败: {e}")
        return False

def main():
    """主函数"""
    parser = argparse.ArgumentParser(description="代码格式化工具")
    parser.add_argument(
        "-c", "--check",
        action="store_true",
        help="检查代码格式但不修改"
    )
    parser.add_argument(
        "-f", "--format",
        action="store_true",
        help="格式化所有代码文件"
    )
    parser.add_argument(
        "-a", "--all",
        action="store_true",
        help="格式化所有支持的文件类型"
    )
    parser.add_argument(
        "--create-config",
        action="store_true",
        help="创建.clang-format配置文件"
    )
    
    args = parser.parse_args()
    
    # 默认行为是格式化
    if not any([args.check, args.format, args.all, args.create_config]):
        args.format = True
    
    print_info(f"项目根目录: {Path.cwd()}")
    
    # 创建配置文件
    if args.create_config:
        create_clang_format_config()
        return
    
    # 确保.clang-format配置文件存在
    if not Path(".clang-format").exists():
        print_warning(".clang-format配置文件不存在")
        if create_clang_format_config():
            print_info("已创建配置文件，继续执行...")
        else:
            print_error("无法创建配置文件，退出")
            sys.exit(1)
    
    dry_run = args.check
    
    if args.format or args.all:
        # 格式化C++文件
        cpp_formatted, cpp_errors = format_cpp_files(dry_run)
        
        if args.all:
            # 格式化CMake文件
            cmake_formatted, cmake_errors = format_cmake_files(dry_run)
        else:
            cmake_formatted, cmake_errors = 0, 0
        
        if dry_run:
            total_errors = cpp_errors + cmake_errors
            if total_errors == 0:
                print_success("✓ 所有文件格式正确")
            else:
                print_warning(f"⚠ 发现 {total_errors} 个文件需要格式化")
                sys.exit(1)
        else:
            total_formatted = cpp_formatted + cmake_formatted
            if total_formatted > 0:
                print_success(f"✓ 成功格式化 {total_formatted} 个文件")
            else:
                print_info("没有文件需要格式化")

if __name__ == "__main__":
    main()