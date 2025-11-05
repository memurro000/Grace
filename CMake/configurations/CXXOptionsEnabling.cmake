
option(GRACE_ENABLE_CXX_OPTIONS_SAN "Use sanitizer compile and link options \
    (requires working sanitizer support)" OFF)
option(GRACE_ENABLE_CXX_OPTIONS_NOLIB_UB_SAN "Enable UB sanitizer without library dependencies - \
    provides detection only producing crash without pretty diagnostics" OFF)
if(GRACE_ENABLE_CXX_OPTIONS_SAN AND GRACE_ENABLE_CXX_OPTIONS_NOLIB_UB_SAN)
    message(
        WARNING "Grace: "
        "Both ENABLE_CXX_OPTIONS_SAN and ENABLE_CXX_OPTIONS_NOLIB_UB_SAN are enabled. This may cause conflicts."
    )
endif()