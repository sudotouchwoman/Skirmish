set(SOURCES include/PlayerEvent.h)

add_library(events INTERFACE)

target_link_libraries(events PUBLIC Boost::json)
target_include_directories(events PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(events INTERFACE include)