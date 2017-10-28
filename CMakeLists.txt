#--------------------------------------------------------
# The CMakeLists.txt for:                       paREST
# Author(s):                              
#--------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(XXD
  /usr/bin/xxd)

# This turns schema files into header files
function(add_schema_headers INFILE OUTFILE)
  add_custom_command(
    OUTPUT ${OUTFILE}
    COMMAND "${XXD}" -i "${INFILE}" "{OUTFILE}"
    DEPENDS "${INFILE}"
    MESSAGE(STATUS, "Running xxd")
    COMMENT "Generating header file from schema ${INFILE}")
endfunction(add_schema_headers)

# List of schema files
set(JSON_SCHEMA_FILES
  ${CMAKE_SOURCE_DIR}/schema/analogread_schema.json   
  ${CMAKE_SOURCE_DIR}/schema/digitalread_schema.json   
  ${CMAKE_SOURCE_DIR}/schema/function_schema.json   
  ${CMAKE_SOURCE_DIR}/schema/variable_schema.json
  ${CMAKE_SOURCE_DIR}/schema/analogwrite_schema.json  
  ${CMAKE_SOURCE_DIR}/schema/digitalwrite_schema.json  
  ${CMAKE_SOURCE_DIR}/schema/interface_schema.json)

# List of header files
set(JSON_HEADER_FILES
  ${CMAKE_SOURCE_DIR}/schema/analogread_schema.h   
  ${CMAKE_SOURCE_DIR}/schema/digitalread_schema.h   
  ${CMAKE_SOURCE_DIR}/schema/function_schema.h   
  ${CMAKE_SOURCE_DIR}/schema/variable_schema.h
  ${CMAKE_SOURCE_DIR}/schema/analogwrite_schema.h  
  ${CMAKE_SOURCE_DIR}/schema/digitalwrite_schema.h  
  ${CMAKE_SOURCE_DIR}/schema/interface_schema.h)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/../moos-ivp/include 
  ${CMAKE_CURRENT_SOURCE_DIR}/../moos-ivp/include/ivp)

SET(MOOS_LIBRARIES
  MOOS
  ${CMAKE_CURRENT_SOURCE_DIR}/../moos-ivp/lib/libapputil.a
  ${CMAKE_CURRENT_SOURCE_DIR}/../moos-ivp/lib/libmbutil.a)

add_schema_headers(${JSON_SCHEMA_FILES} ${JSON_HEADER_FILES})

SET(SRC
  aREST.cpp
  aREST_Info.cpp
  main.cpp
  ${JSON_HEADER_FILES}
)

ADD_EXECUTABLE(paREST ${SRC})

TARGET_LINK_LIBRARIES(paREST
   ${MOOS_LIBRARIES}
   m
   pthread)
