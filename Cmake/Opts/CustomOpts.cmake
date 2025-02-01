cmake_minimum_required(VERSION 3.22)

function(target_post_build TargetName)
    set(AllTargets "${AllTargets}" CACHE INTERNAL "My list of items" FORCE)
    add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${TargetName} ${CMAKE_BINARY_DIR}/${TargetName}.bin
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${TargetName} ${CMAKE_BINARY_DIR}/${TargetName}.hex
        COMMAND ${CMAKE_OBJDUMP} -S ${TargetName} > ${CMAKE_BINARY_DIR}/${TargetName}.S
        COMMAND ${CMAKE_NM} -a -l -S -s ${TargetName} > ${CMAKE_BINARY_DIR}/${TargetName}.map
        COMMAND ${CMAKE_SIZE} -A ${TargetName}
        COMMAND ${CMAKE_SIZE} ${TargetName}
        COMMAND echo "Build - success"
        COMMAND echo "CMAKE_BUILD_TYPE::${CMAKE_BUILD_TYPE}"
    )
    install(TARGETS ${TargetName} DESTINATION bin/${TargetName})
    install(FILES ${CMAKE_BINARY_DIR}/${TargetName}.bin DESTINATION bin/${TargetName})
    install(FILES ${CMAKE_BINARY_DIR}/${TargetName}.hex DESTINATION bin/${TargetName})
endfunction(target_post_build TargetName)

function(target_install_binary TargetName)
    add_custom_target(Install${TargetName}
        COMMAND CPack -G ZIP
        DEPENDS ${PROJECT_NAME}
        COMMENT "Installing ${PROJECT_NAME}"
    )
endfunction(target_install_binary TargetName)
