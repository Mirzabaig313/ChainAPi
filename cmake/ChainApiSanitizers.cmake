# ChainApiSanitizers.cmake
# Reusable function: chainapi_apply_sanitizers(<target>)
# Honors CHAINAPI_ENABLE_ASAN and CHAINAPI_ENABLE_UBSAN options.
# No-op on MSVC for ASan to avoid runtime weirdness; CI matrix runs ASan on
# clang/gcc only per the project layout doc.

function(chainapi_apply_sanitizers target)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "chainapi_apply_sanitizers: '${target}' is not a target")
    endif()

    if(MSVC)
        return()
    endif()

    set(_san_flags "")
    if(CHAINAPI_ENABLE_ASAN)
        list(APPEND _san_flags address)
    endif()
    if(CHAINAPI_ENABLE_UBSAN)
        list(APPEND _san_flags undefined)
    endif()

    if(_san_flags)
        string(REPLACE ";" "," _san_csv "${_san_flags}")
        target_compile_options(${target} PRIVATE
            -fsanitize=${_san_csv}
            -fno-omit-frame-pointer
            -g
        )
        target_link_options(${target} PRIVATE
            -fsanitize=${_san_csv}
        )
    endif()
endfunction()
