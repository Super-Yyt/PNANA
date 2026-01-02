# FindTreeSitterLanguages.cmake
# 查找 Tree-sitter 语言库的 CMake 模块
#
# 此模块查找常用的 Tree-sitter 语言库
# 如果某个语言库未找到，对应的语言将使用原生语法高亮器
#
# 使用方式:
#   include(FindTreeSitterLanguages)
#
# 定义变量:
#   对于每个语言，定义以下变量:
#   TreeSitter_<LANG>_FOUND     - 如果找到语言库则为 TRUE
#   TreeSitter_<LANG>_LIBRARY   - 库文件路径
#
# 支持的语言:
#   CPP, C, PYTHON, JAVASCRIPT, TYPESCRIPT, JSON, MARKDOWN, BASH, RUST, GO, JAVA,
#   CMAKE, TCL, FORTRAN, HASKELL, LUA

# 查找函数
function(find_tree_sitter_language lang_name lib_names display_name)
    find_library(TreeSitter_${lang_name}_LIBRARY
        NAMES ${lib_names}
        PATHS
            /usr/lib
            /usr/local/lib
            /usr/lib/x86_64-linux-gnu
            /opt/local/lib
            ${CMAKE_SOURCE_DIR}/third-party
            ~/.local/lib
            ${CMAKE_INSTALL_PREFIX}/lib
        PATH_SUFFIXES
            lib
            lib64
    )
    
    if(TreeSitter_${lang_name}_LIBRARY)
        set(TreeSitter_${lang_name}_FOUND TRUE PARENT_SCOPE)
        set(TreeSitter_${lang_name}_LIBRARY ${TreeSitter_${lang_name}_LIBRARY} PARENT_SCOPE)
        if(NOT TreeSitterLanguages_FIND_QUIETLY)
            message(STATUS "  ✓ Found Tree-sitter ${display_name} language library: ${TreeSitter_${lang_name}_LIBRARY}")
        endif()
    else()
        set(TreeSitter_${lang_name}_FOUND FALSE PARENT_SCOPE)
        if(NOT TreeSitterLanguages_FIND_QUIETLY)
            message(STATUS "  ✗ Tree-sitter ${display_name} language library not found")
        endif()
    endif()
endfunction()

# 查找各个语言库
if(NOT TreeSitterLanguages_FIND_QUIETLY)
    message(STATUS "Searching for Tree-sitter language libraries...")
endif()

find_tree_sitter_language(CPP "tree-sitter-cpp;libtree-sitter-cpp" "C++")
find_tree_sitter_language(C "tree-sitter-c;libtree-sitter-c" "C")
find_tree_sitter_language(PYTHON "tree-sitter-python;libtree-sitter-python" "Python")
find_tree_sitter_language(JAVASCRIPT "tree-sitter-javascript;libtree-sitter-javascript" "JavaScript")
find_tree_sitter_language(TYPESCRIPT "tree-sitter-typescript;libtree-sitter-typescript" "TypeScript")
find_tree_sitter_language(JSON "tree-sitter-json;libtree-sitter-json" "JSON")
find_tree_sitter_language(MARKDOWN "tree-sitter-markdown;libtree-sitter-markdown" "Markdown")
find_tree_sitter_language(BASH "tree-sitter-bash;libtree-sitter-bash" "Bash")
find_tree_sitter_language(RUST "tree-sitter-rust;libtree-sitter-rust" "Rust")
find_tree_sitter_language(GO "tree-sitter-go;libtree-sitter-go" "Go")
find_tree_sitter_language(JAVA "tree-sitter-java;libtree-sitter-java" "Java")
find_tree_sitter_language(CMAKE "tree-sitter-cmake;libtree-sitter-cmake" "CMake")
find_tree_sitter_language(TCL "tree-sitter-tcl;libtree-sitter-tcl" "TCL")
find_tree_sitter_language(FORTRAN "tree-sitter-fortran;libtree-sitter-fortran" "Fortran")
find_tree_sitter_language(HASKELL "tree-sitter-haskell;libtree-sitter-haskell" "Haskell")
find_tree_sitter_language(LUA "tree-sitter-lua;libtree-sitter-lua" "Lua")

# 标记为高级变量
mark_as_advanced(
    TreeSitter_CPP_LIBRARY
    TreeSitter_C_LIBRARY
    TreeSitter_PYTHON_LIBRARY
    TreeSitter_JAVASCRIPT_LIBRARY
    TreeSitter_TYPESCRIPT_LIBRARY
    TreeSitter_JSON_LIBRARY
    TreeSitter_MARKDOWN_LIBRARY
    TreeSitter_BASH_LIBRARY
    TreeSitter_RUST_LIBRARY
    TreeSitter_GO_LIBRARY
    TreeSitter_JAVA_LIBRARY
    TreeSitter_CMAKE_LIBRARY
    TreeSitter_TCL_LIBRARY
    TreeSitter_FORTRAN_LIBRARY
    TreeSitter_HASKELL_LIBRARY
    TreeSitter_LUA_LIBRARY
)

