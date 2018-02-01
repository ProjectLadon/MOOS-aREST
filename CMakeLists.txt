#--------------------------------------------------------
# The CMakeLists.txt for:                       paREST
# Author(s):
#--------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(XXD
  /usr/bin/xxd)

# List of schema files
set(JSON_SCHEMA_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/schema/analogread_schema.json
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/digitalread_schema.json
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/function_schema.json
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/variable_schema.json
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/analogwrite_schema.json
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/digitalwrite_schema.json
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/interface_schema.json)

# List of schema files
set(JSON_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/analogread_schema.h
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/digitalread_schema.h
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/function_schema.h
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/variable_schema.h
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/analogwrite_schema.h
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/digitalwrite_schema.h
  ${CMAKE_CURRENT_SOURCE_DIR}/schema/interface_schema.h)

FOREACH(LOOP_SCHEMA ${JSON_SCHEMA_FILES})
	get_filename_component(LOOP_DIR ${LOOP_SCHEMA} DIRECTORY)
	get_filename_component(LOOP_FILE_WE ${LOOP_SCHEMA} NAME_WE)
	SET(LOOP_HEADER ${LOOP_DIR}/${LOOP_FILE_WE}.h)
	add_custom_command(
		OUTPUT ${LOOP_HEADER}
		COMMAND	cd ${LOOP_DIR} && ${XXD} -i ${LOOP_FILE_WE}.json ${LOOP_FILE_WE}.h
		DEPENDS ${LOOP_SCHEMA})
ENDFOREACH(LOOP_SCHEMA)

SET(SRC
  aREST.cpp
  aREST_Info.cpp
  main.cpp
  "${JSON_HEADER_FILES}"
)

ADD_EXECUTABLE(paREST ${SRC})

TARGET_LINK_LIBRARIES(paREST
  ${MOOS_LIBRARIES}
   m
   mbutil
   apputil
   pthread
   curl)
