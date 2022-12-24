function (add_plugin name)
    file(GLOB Plugin_${name}_files ${CMAKE_CURRENT_SOURCE_DIR}/${name}/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/${name}/*.hxx)
    add_library(Bolt_${name} SHARED ${Plugin_${name}_files})
    install(TARGETS Bolt_${name}
        LIBRARY DESTINATION ${BOLT_PLUGIN_DIR})
endfunction (add_plugin)

add_plugin(ostree)
add_plugin(net)
add_plugin(unknown)