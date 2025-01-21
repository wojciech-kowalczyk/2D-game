cmake_minimum_required(VERSION 3.26)
project(Golf VERSION 0.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6Widgets REQUIRED)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
	set(DEBUG_BUILD_TYPE True)
else ()
	set(ADD_WIN32_RELEASE WIN32)
endif ()

configure_file(Others/config.h.in include/config.h)

add_executable(${PROJECT_NAME} ${ADD_WIN32_RELEASE} main.cpp ${PROJECT_BINARY_DIR}/include/config.h
               Libraries/List.h
               Libraries/Range.h
               Libraries/geometry.h
               Libraries/utility.h
               Graphics/renderFunctions.h Graphics/renderFunctions.cpp
               Graphics/Window.h Graphics/Window.cpp
               Graphics/Shader.h Graphics/Shader.cpp
               Graphics/Texture.h Graphics/Texture.cpp
               Graphics/Color.h
               Graphics/Vertex.h
               Graphics/Drawable.h
               Xml/Base.h Xml/Base.cpp
               Xml/Settings.h Xml/Settings.cpp
               Xml/Level.h Xml/Level.cpp
               Math/Point.h
               Math/Vector.h
               Math/LineFunction.h
               Math/CircleFunction.h
               Libraries/Clock.h Libraries/Clock.cpp
               Others/Axis.h
               Others/Status.h
#              Others/Camera.h Others/Camera.cpp
               Shapes/Sub/Base.h Shapes/Sub/Base.cpp
               Shapes/Sub/Velocity.h Shapes/Sub/Velocity.cpp
               Shapes/Sub/Drag.h Shapes/Sub/Drag.cpp
               Shapes/Circle.h Shapes/Circle.cpp
#              Shapes/Complex/Ball.h Shapes/Complex/Ball.cpp
#              Shapes/Complex/Arrow.h Shapes/Complex/Arrow.cpp
#              Shapes/Complex/Background.h Shapes/Complex/Background.cpp
               Shapes/Specialized.h
#              Shapes/Entities.h Shapes/Entities.cpp
               Shapes/EntitiesTemp.h Shapes/EntitiesTemp.cpp
               Ui/ResizableQWidget.h Ui/ResizableQWidget.cpp
               Ui/MainMenu.h Ui/MainMenu.cpp
               Ui/Settings.h Ui/Settings.cpp
               Ui/LevelLauncher.h Ui/LevelLauncher.cpp
               Ui/Level.h Ui/Level.cpp
               Ui/LevelCreatorLauncher.h Ui/LevelCreatorLauncher.cpp
#              Ui/LevelCreator.h Ui/LevelCreator.cpp
               Ui/WidgetPosition.h Ui/WidgetPosition.cpp
               Shapes/Sub/Springiness.cpp
               Shapes/Sub/Springiness.h
               Shapes/Rectangle.cpp
               Shapes/Rectangle.h)

if (DEBUG_BUILD_TYPE)
	target_sources(${PROJECT_NAME} PRIVATE Others/Error.h Others/Error.cpp)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 Qt6::Widgets D:/libs/pugiXml/lib/pugixml.a)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/include D:/libs/pugiXml/include D:/libs/image/include)
