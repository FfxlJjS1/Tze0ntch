cmake_minimum_required(VERSION 3.8)

project("Tze0ntch")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
include_directories(SYSTEM "./include/")

# find_package(Boost 1.40.0 REQUIRED system)
set (BOOST_ROOT "C:/Boost/boost_1_82_0/") # Путь к библиотеке Boost

set (CMAKE_CXX_STANDARD 20)
set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

find_package (Boost REQUIRED COMPONENTS program_options REQUIRED)

if (Boost_FOUND)
    include_directories (SYSTEM ${Boost_INCLUDE_DIR}) # подключаем заголовочные файлы
#    add_executable (Boost_test "include/main.h") # Создаем исполняемый файл для нашего проекта
#    target_link_libraries (Boost_test ${Boost_LIBRARIES}) # подключаем библиотеку
endif ()

set(include_dir include)
set(src_dir src)

##########################################################################
# ini configurator module
set(INI_CONFIGURATOR_header_dir ${include_dir})
set(INI_CONFIGURATOR_srt_dir ${src_dir})

set(INI_CONFIGURATOR_HEADER_FILES 
		${INI_CONFIGURATOR_header_dir}/IniConfigure.h)
		
set(INI_CONFIGURATOR_SRC_FILES 
		${INI_CONFIGURATOR_srt_dir}/IniConfigure.cpp)


##########################################################################
# db controller module
set(DBMS_header_dir ${include_dir}/DBMS)
set(DBMS_src_dir ${src_dir}/DBMS)

set(DBMS_HEADER_FILES 
		${DBMS_header_dir}/DBMS.h
		${DBMS_header_dir}/DbControllers/DbController.h
		${DBMS_header_dir}/DbControllers/System/SystemDbController.h
		${DBMS_header_dir}/DbControllers/Module/Module.h
		${DBMS_header_dir}/DbControllers/Module/Representing.h
		${DBMS_header_dir}/DbControllers/Module/ModuleDbController.h
		${DBMS_header_dir}/DbControllers/Customizable/CustomizableDbController.h
		${DBMS_header_dir}/DbControllers/SemanticWebWithIndexing/Vertex.h
		${DBMS_header_dir}/DbControllers/SemanticWebWithIndexing/SubNode.h
		${DBMS_header_dir}/DbControllers/SemanticWebWithIndexing/Node.h
		${DBMS_header_dir}/DbControllers/SemanticWebWithIndexing/SemanticWeb.h
		${DBMS_header_dir}/DbControllers/SemanticWebWithIndexing/SemanticWebWithIndexingDbController.h)

set(DBMS_SRC_FILES
		${DBMS_src_dir}/DBMS.cpp
		${DBMS_src_dir}/DbControllers/DbController.cpp
		${DBMS_src_dir}/DbControllers/System/SystemDbController.cpp
		${DBMS_src_dir}/DbControllers/Module/Module.cpp
		${DBMS_src_dir}/DbControllers/Module/Representing.cpp
		${DBMS_src_dir}/DbControllers/Module/ModuleDbController.cpp
		${DBMS_src_dir}/DbControllers/Customizable/CustomizableDbController.cpp
		${DBMS_src_dir}/DbControllers/SemanticWebWithIndexing/Vertex.cpp
		${DBMS_src_dir}/DbControllers/SemanticWebWithIndexing/SubNode.cpp
		${DBMS_src_dir}/DbControllers/SemanticWebWithIndexing/Node.cpp
		${DBMS_src_dir}/DbControllers/SemanticWebWithIndexing/SemanticWeb.cpp
		${DBMS_src_dir}/DbControllers/SemanticWebWithIndexing/SemanticWebWithIndexingDbController.cpp)
		

##########################################################################
# interpreter module
set(Interpreter_header_dir ${include_dir}/Interpretator)
set(Interpreter_src_dir ${src_dir}/Interpretator)

set(INTERPRETER_HEADER_FILES
		${Interpreter_header_dir}/Interpretator.h
		${Interpreter_header_dir}/InterpretatorStages/Interpretator.h
		${Interpreter_header_dir}/InterpretatorStages/RecursionParser.h
		${Interpreter_header_dir}/InterpretatorStages/InterpretatorHeaders.h
		${Interpreter_header_dir}/InterpretatorStages/GlobalIISMutex.h
		${Interpreter_header_dir}/OperatorActions/LineAction.h
		${Interpreter_header_dir}/OperatorActions/AssertAction.h
		${Interpreter_header_dir}/OperatorActions/DefineAction.h
		${Interpreter_header_dir}/OperatorActions/GoToMethodEmptyAction.h
		${Interpreter_header_dir}/OperatorActions/RemoveSpecificVariable.h
		${Interpreter_header_dir}/OperatorActions/ReturnFromMethodAction.h
		${Interpreter_header_dir}/OperatorActions/SkipRemoveMethLocalVariable.h
		${Interpreter_header_dir}/OperatorActions/LineActionClasses.h
		${Interpreter_header_dir}/OperatorActions/GotoActions/GotoAction.h
		${Interpreter_header_dir}/OperatorActions/GotoActions/GotoActionWithIf.h
		${Interpreter_header_dir}/OperatorArgumentTypes/Variable.h
		${Interpreter_header_dir}/OperatorArgumentTypes/OnlyVariableClasses.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableClasses.h
		${Interpreter_header_dir}/OperatorArgumentTypes/OperationTypes/CastAction.h
		${Interpreter_header_dir}/OperatorArgumentTypes/OperationTypes/ComparibleAction.h
		${Interpreter_header_dir}/OperatorArgumentTypes/OperationTypes/CreateComplexVar.h
		${Interpreter_header_dir}/OperatorArgumentTypes/OperationTypes/GoToMethod.h
		${Interpreter_header_dir}/OperatorArgumentTypes/OperationTypes/LogicalAction.h
		${Interpreter_header_dir}/OperatorArgumentTypes/OperationTypes/LogicalNOT.h
		${Interpreter_header_dir}/OperatorArgumentTypes/OperationTypes/LogicalTwoArgs.h
		${Interpreter_header_dir}/OperatorArgumentTypes/OperationTypes/MathAction.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/MethodVariable.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/NamedObject.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/LinkToVariable.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/ComplexVariableTypes/Array.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/ComplexVariableTypes/String_t.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/ComplexVariableTypes/Struct.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/ComplexVariableTypes/Tree.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/ComplexVariableTypes/VariablePath/ArrayIndex.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/ComplexVariableTypes/VariablePath/VariablePath.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SimpleVariableTypes/Void.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SimpleVariableTypes/Bool.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SimpleVariableTypes/Byte.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SimpleVariableTypes/Double.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SimpleVariableTypes/Int.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SimpleVariableTypes/Long.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SimpleVariableTypes/UByte.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SimpleVariableTypes/UInt.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SimpleVariableTypes/ULong.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SpecificVariableTypes/Any.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SpecificVariableTypes/ArrayTemplate.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SpecificVariableTypes/NullObject.h
		${Interpreter_header_dir}/OperatorArgumentTypes/VariableTypes/SpecificVariableTypes/StackElement.h
		${Interpreter_header_dir}/Tables/ContainerElementTable.h
		${Interpreter_header_dir}/Tables/ContainerTable.h
		${Interpreter_header_dir}/Tables/ModuleTable.h
		${Interpreter_header_dir}/Tables/MethodTable.h
		${Interpreter_header_dir}/Tables/StructTable.h
		${Interpreter_header_dir}/Tables/NamedVariable.h)

set(INTERPRETER_SRC_FILES
		${Interpreter_src_dir}/Interpretator.cpp
		${Interpreter_src_dir}/InterpretatorStages/Interpretator.cpp
		${Interpreter_src_dir}/InterpretatorStages/RecursionParser.cpp)


##########################################################################
# network module
set(Network_header_dir ${include_dir}/NetworkInterface)
set(Network_src_dir ${src_dir}/NetworkInterface)

set(INTERPRETER_HEADER_FILES
		${Network_header_dir}/NetworkInterface.h)

set(INTERPRETER_SRC_FILES
		${Network_src_dir}/NetworkInterface.cpp)


##########################################################################


add_executable(Tze0ntch 
	${INI_CONFIGURATOR_HEADER_FILES} ${DBMS_HEADER_FILES} ${INTERPRETER_HEADER_FILES} ${NETWORK_HEADER_FILES}
	${INI_CONFIGURATOR_SRC_FILES} ${DBMS_SRC_FILES} ${INTERPRETER_SRC_FILES} ${NETWORK_SRC_FILES})

# target_compile_options(Tze0ntch PRIVATE -Wall -Wextra)
