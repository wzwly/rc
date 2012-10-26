# 创建预编译头
# Target是用来生成预编译头的项目Target；PrecompiledHeader和PrecompiledSource分别是头文件的路径1�7  
MACRO(ADD_PRECOMPILED_HEADER Target PrecompiledHeader PrecompiledSource)
 
IF(MSVC)
    ADD_MSVC_PRECOMPILED_HEADER(${PrecompiledHeader}
      ${PrecompiledSource})
ENDIF(MSVC)
 
IF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_GCC_PRECOMPILED_HEADER(${Target} ${PrecompiledHeader})
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ENDMACRO(ADD_PRECOMPILED_HEADER)
 
# 在生成一个预编译头以后就可以调用这个宏来指定源文件来使用宄1�7 
# SourcesVar是一个包含了要使用该预编译头的源文件列表
MACRO(USE_PRECOMPILED_HEADER SourcesVar)
  IF(MSVC)
    USE_MSVC_PRECOMPILED_HEADER(${SourcesVar})
  ENDIF(MSVC)
 
  IF(CMAKE_COMPILER_IS_GNUCXX)
    USE_GCC_PRECOMPILED_HEADER(${SourcesVar})
  ENDIF()
ENDMACRO(USE_PRECOMPILED_HEADER)
 
# 用于为visual studio生成pch 
MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource)
  GET_FILENAME_COMPONENT(PrecompiledBasename
    ${PrecompiledHeader} NAME_WE)
 
  # 得到pch文件的文件名 
  SET(PrecompiledBinary
    "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
 
  SET(PCH_BIN_PATH
    ${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch
    CACHE INTERNAL "the path of the precompiled binary")
  SET(PCH_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/${PrecompiledHeader}
    CACHE INTERNAL "the path of the precompiled header")
 
  # 为visual studio设置用于生成pch的编译器参数 
  SET_SOURCE_FILES_PROPERTIES(
    ${PrecompiledSource}
    PROPERTIES COMPILE_FLAGS
    "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
    OBJECT_OUTPUTS "${PrecompiledBinary}")
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)
 
MACRO(USE_MSVC_PRECOMPILED_HEADER SourcesVar)
  SET(Sources ${${SourcesVar}})
 
  message("using pch: ${PCH_BIN_PATH} for ${${SourcesVar}}")
 
  set(PrecompiledBinary ${PCH_BIN_PATH})
 
  # 对需要使用pch的源文件设置编译器参敄1�7
    SET_SOURCE_FILES_PROPERTIES(${Sources}
    PROPERTIES COMPILE_FLAGS
    "/Yu\"${PrecompiledBinary}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
    OBJECT_DEPENDS "${PrecompiledBinary}")
ENDMACRO(USE_MSVC_PRECOMPILED_HEADER)
 
# 改函数用于判断gcc是否支持预编译头 
IF(CMAKE_COMPILER_IS_GNUCXX)
  EXEC_PROGRAM(
    ${CMAKE_CXX_COMPILER}
    ARGS                    --version
    OUTPUT_VARIABLE _compiler_output)
  STRING(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1"
    gcc_compiler_version ${_compiler_output})
  #MESSAGE("GCC Version: ${gcc_compiler_version}")
  IF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    SET(PCHSupport_FOUND TRUE)
  ELSE(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    IF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
      SET(PCHSupport_FOUND TRUE)
    ENDIF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
  ENDIF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
 
MACRO(USE_GCC_PRECOMPILED_HEADER SourcesVar)
  SET(Sources ${${SourcesVar}})
 
  # 通过-include参数让所有源文件强制包含预编译头
  FOREACH(source ${_sourcesVar})
    SET_SOURCE_FILES_PROPERTIES(${source}
      PROPERTIES
      COMPILE_FLAGS "-include ${_name} -Winvalid-pch"
      OBJECT_DEPENDS "${PCH_PATH}")
  ENDFOREACH(source)
ENDMACRO(USE_GCC_PRECOMPILED_HEADER)
 
MACRO(ADD_GCC_PRECOMPILED_HEADER TargetName PrecompileHeader)
  SET(_compile_FLAGS ${CMAKE_CXX_FLAGS})
 
  SET(_input "${CMAKE_CURRENT_SOURCE_DIR}/${PrecompileHeader}")
  MESSAGE("creating pch: ${_input}")
  GET_FILENAME_COMPONENT(_name ${_input} NAME)
  GET_FILENAME_COMPONENT(_path ${_input} PATH)
 
  # 根据不同的编译配置生成预编译头二进制文件的文件名，比如debug.c++, release.c++
  SET(_outdir "${CMAKE_CURRENT_BINARY_DIR}/${_name}.gch")
  IF(CMAKE_BUILD_TYPE)
    SET(_output "${_outdir}/${CMAKE_BUILD_TYPE}.c++")
    LIST(APPEND _compile_FLAGS ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}})
  ELSE(CMAKE_BUILD_TYPE)
    SET(_output "${_outdir}/default.c++")
  ENDIF(CMAKE_BUILD_TYPE)
 
  IF(${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
    LIST(APPEND _compile_FLAGS "-DQT_DEBUG")
  ENDIF()
 
  # 自动创建文件处1�7  
    ADD_CUSTOM_COMMAND(
    OUTPUT ${_outdir}
    COMMAND mkdir ${_outdir} # TODO: {CMAKE_COMMAND} -E ...
    )
 
  GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
 
  # 确保生成预编译头的文件夹在所有包含目录的朄1�7前面
  SET(_CMAKE_CURRENT_BINARY_DIR_included_before_path FALSE)
  FOREACH(item ${_directory_flags})
    IF(${item}  STREQUAL ${_path} AND NOT
        _CMAKE_CURRENT_BINARY_DIR_included_before_path )
      MESSAGE(FATAL_ERROR
        "This is the ADD_PRECOMPILED_HEADER macro. "
        "CMAKE_CURREN_BINARY_DIR has to mentioned at INCLUDE_DIRECTORIES's argument list before ${_path}, where ${_name} is located"
        )
    ENDIF(${item}  STREQUAL ${_path} AND NOT
      _CMAKE_CURRENT_BINARY_DIR_included_before_path )
 
    IF(${item}  STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
      SET(_CMAKE_CURRENT_BINARY_DIR_included_before_path TRUE)
    ENDIF(${item}  STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
 
    LIST(APPEND _compile_FLAGS "-I${item}")
  ENDFOREACH(item)
 
  GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
  #LIST(APPEND _compile_FLAGS "-fPIC")
  LIST(APPEND _compile_FLAGS ${_directory_flags})
 
  SEPARATE_ARGUMENTS(_compile_FLAGS)
  #MESSAGE("_compiler_FLAGS: ${_compile_FLAGS}")
 
  # 拷贝头文件到binary目录
  ADD_CUSTOM_COMMAND(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${_name}
    COMMAND ${CMAKE_COMMAND} -E copy  ${_input}
    ${CMAKE_CURRENT_BINARY_DIR}/${_name}
    )
 
  SET(PCH_PATH ${CMAKE_CURRENT_BINARY_DIR}/${_name} CACHE INTERNAL
    "the path of the precompiled header")
 
  #MESSAGE("command : ${CMAKE_COMMAND} -E copy  ${_input}
  #  ${CMAKE_CURRENT_BINARY_DIR}/${_name}")
 
  # 添加用于生成预编译头的命仄1�7  
    ADD_CUSTOM_COMMAND(
    OUTPUT ${_output}
    COMMAND ${CMAKE_CXX_COMPILER}
    ${_compile_FLAGS}
    -x c++-header
    -o ${_output}
    ${_input}
    DEPENDS ${_input} ${_outdir} ${CMAKE_CURRENT_BINARY_DIR}/${_name}
    )
  ADD_CUSTOM_TARGET(${TargetName}_gch
    DEPENDS ${_output}
    )
  ADD_DEPENDENCIES(${TargetName} ${TargetName}_gch )
ENDMACRO(ADD_GCC_PRECOMPILED_HEADER)


#=============================================================#
#=============================================================#
PROJECT(stripchart)

aux_source_directory(. SRC_DIR)
aux_source_directory(./Core/ SUB_SRC_DIR)

include_directories(./ )

ADD_EXECUTABLE(stripchart ${SRC_DIR} ${SUB_SRC_DIR})

ADD_PRECOMPILED_HEADER(stripchart sefafx.h  sefafx.cpp)

USE_PRECOMPILED_HEADER(SRC_DIR SUB_SRC_DIR)

