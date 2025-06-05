if (NOT TARGET amulet_level)
    message(STATUS "Finding amulet_level")

    find_package(amulet_game CONFIG REQUIRED)

    set(amulet_level_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../..")
    find_library(amulet_level_LIBRARY NAMES amulet_level PATHS "${CMAKE_CURRENT_LIST_DIR}")
    message(STATUS "amulet_level_LIBRARY: ${amulet_level_LIBRARY}")

    add_library(amulet_level SHARED IMPORTED)
    set_target_properties(amulet_level PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${amulet_level_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES amulet_game
        IMPORTED_IMPLIB "${amulet_level_LIBRARY}"
    )
endif()
