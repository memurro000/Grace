
# TODO reconsider file name, extension and location

add_library(Grace_build_flags INTERFACE)

set_target_properties(Grace_build_flags PROPERTIES EXPORT_PROPERTIES "EXCLUDE_FROM_ALL")

message(STATUS "Grace: " "MSVC build options are subject to test")
message(STATUS "Grace: " "MSVC sanitizer build options are disabled now and are subject to change")

target_compile_options(
    Grace_build_flags
    INTERFACE
        # GNU, Clang, AppleClang
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleCLang>:
            # Common
                # Warnings
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wnon-virtual-dtor
                # -Wnull-dereference # TODO reconsider
                # -Wdouble-promotion # TODO reconsider

                # Version-specific warnings
                # $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,8>:-Wduplicated-cond>     # TODO reconsider
                # $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,6>:-Wduplicated-branches> # TODO reconsider

            # Debug
            $<$<CONFIG:Debug>:
                -O0
                -g3
#                -D_GLIBCXX_DEBUG # TODO reconsider (doesn't work with GTest)
                -D_GLIBCXX_ASSERTIONS
                -fno-inline
                -fno-omit-frame-pointer
                -fstack-protector-strong
#                -fstack-clash-protection # TODO reconsider (reported to be unused)

                # Sanitizer
                $<$<BOOL:${GRACE_ENABLE_CXX_OPTIONS_SAN}>:
                    -fno-sanitize-recover=undefined
                    -fsanitize-address-use-after-scope
                    -fsanitize=address,undefined
                    $<$<NOT:$<PLATFORM_ID:Windows,Darwin>>:
                        -fsanitize=leak
                    >
                >
                $<$<BOOL:${GRACE_ENABLE_CXX_OPTIONS_NOLIB_UB_SAN}>:
                    -fsanitize=undefined
                    -fsanitize-undefined-trap-on-error
                >
            >

            # Release
            $<$<CONFIG:Release>:
                -O3
                -DNDEBUG

                # Performance
                -funroll-loops
                -fomit-frame-pointer
#                -flto # TODO reconsider (file not recognized: file format not recognized)
                -march=native
                -mtune=native

                # TODO benchmark
                # Security
                -D_FORTIFY_SOURCE=2
                -fstack-protector-strong
            >

            # RelWithDebInfo
            $<$<CONFIG:RelWithDebInfo>:
                -O2
                -g
                -DNDEBUG
                -funroll-loops
                -fomit-frame-pointer
#                -flto # TODO reconsider (file not recognized: file format not recognized)
            >

            # MinSizeRel
            $<$<CONFIG:MinSizeRel>:
                -Os
                -DNDEBUG
                -fomit-frame-pointer
#                -flto # TODO reconsider (file not recognized: file format not recognized)
            >

        >

        # MSVC
        $<$<CXX_COMPILER_ID:MSVC>:
            # Common
            /W4
            /permissive-

            # Debug
            $<$<CONFIG:Debug>:
                /Od
                /Zi
                /RTC1
                /MDd
            >

            # Release
            $<$<CONFIG:Release>:
                /O2
                /Ob2
                /Gy
                /DNDEBUG
                /GL
                /MD
            >

            # RelWithDebInfo
            $<$<CONFIG:RelWithDebInfo>:
                /O2
                /Ob1
                /Zi
                /DNDEBUG
                /MD
            >

        >

)

target_link_options(
    Grace_build_flags
    INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
            $<$<CONFIG:Debug>:
                $<$<BOOL:${GRACE_ENABLE_CXX_OPTIONS_SAN}>:
                    -fno-sanitize-recover=undefined
                    -fsanitize-address-use-after-scope
                    -fsanitize=address,undefined
                    $<$<NOT:$<PLATFORM_ID:Windows,Darwin>>:
                        -fsanitize=leak
                    >
                >
                $<$<BOOL:${GRACE_ENABLE_CXX_OPTIONS_NOLIB_UB_SAN}>:
                    -fsanitize=undefined
                    -fsanitize-undefined-trap-on-error
                >
            >
        >
)