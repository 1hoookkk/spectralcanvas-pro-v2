# Pluginval integration for continuous validation
# Pattern #7: Continuous Verification with pluginval

# Find or download pluginval
if(WIN32)
    set(PLUGINVAL_URL "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_Windows.zip")
    set(PLUGINVAL_EXECUTABLE "${CMAKE_BINARY_DIR}/pluginval.exe")
elseif(APPLE)
    set(PLUGINVAL_URL "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_macOS.zip")
    set(PLUGINVAL_EXECUTABLE "${CMAKE_BINARY_DIR}/pluginval.app/Contents/MacOS/pluginval")
else()
    set(PLUGINVAL_URL "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_Linux.zip")
    set(PLUGINVAL_EXECUTABLE "${CMAKE_BINARY_DIR}/pluginval")
endif()

# Download pluginval if not present
if(NOT EXISTS "${PLUGINVAL_EXECUTABLE}")
    message(STATUS "Downloading pluginval...")
    file(DOWNLOAD "${PLUGINVAL_URL}" "${CMAKE_BINARY_DIR}/pluginval.zip" SHOW_PROGRESS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf "${CMAKE_BINARY_DIR}/pluginval.zip"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    file(REMOVE "${CMAKE_BINARY_DIR}/pluginval.zip")
endif()

# Create pluginval test target
add_custom_target(pluginval_test
    COMMAND ${CMAKE_COMMAND} -E echo "Running pluginval with strict validation..."
    COMMAND "${PLUGINVAL_EXECUTABLE}" 
        --strictness-level 8
        --timeout-ms 300000
        --output-dir "${CMAKE_BINARY_DIR}/pluginval_results"
        --validate-in-process
        --verbose
        --skip-gui-tests  # Skip GUI tests for CI
        $<TARGET_FILE:SpectralCanvasPro_VST3>
    DEPENDS SpectralCanvasPro
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Running pluginval validation on SpectralCanvas Pro"
)

# Create a faster validation for development
add_custom_target(pluginval_quick
    COMMAND ${CMAKE_COMMAND} -E echo "Running quick pluginval validation..."
    COMMAND "${PLUGINVAL_EXECUTABLE}"
        --strictness-level 5
        --timeout-ms 60000
        --validate-in-process
        --skip-gui-tests
        $<TARGET_FILE:SpectralCanvasPro_VST3>
    DEPENDS SpectralCanvasPro
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Running quick pluginval validation"
)

# Create RT-safety specific test
add_custom_target(pluginval_rt_safety
    COMMAND ${CMAKE_COMMAND} -E echo "Running RT-safety focused validation..."
    COMMAND "${PLUGINVAL_EXECUTABLE}"
        --strictness-level 8
        --timeout-ms 180000
        --validate-in-process
        --enable-allocation-tracker
        --random-parameters
        --skip-gui-tests
        $<TARGET_FILE:SpectralCanvasPro_VST3>
    DEPENDS SpectralCanvasPro
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Running RT-safety focused pluginval validation"
)

# Create pluginval results parser (simple script) - optional
# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/scripts/parse_pluginval_results.py.in"
#     "${CMAKE_BINARY_DIR}/parse_pluginval_results.py"
#     @ONLY
# )

# Integration with CTest if available
if(BUILD_TESTING)
    enable_testing()
    
    add_test(
        NAME PluginvalValidation
        COMMAND "${PLUGINVAL_EXECUTABLE}"
            --strictness-level 7
            --timeout-ms 240000
            --validate-in-process
            --skip-gui-tests
            $<TARGET_FILE:SpectralCanvasPro_VST3>
    )
    
    add_test(
        NAME PluginvalRTSafety
        COMMAND "${PLUGINVAL_EXECUTABLE}"
            --strictness-level 8
            --timeout-ms 120000
            --validate-in-process
            --enable-allocation-tracker
            --skip-gui-tests
            $<TARGET_FILE:SpectralCanvasPro_VST3>
    )
    
    # Set test properties
    set_tests_properties(PluginvalValidation PluginvalRTSafety PROPERTIES
        TIMEOUT 300
        LABELS "pluginval;validation"
    )
endif()

# Helper function to add custom validation tests
function(add_pluginval_test NAME)
    set(options QUICK RT_SAFETY_ONLY)
    set(oneValueArgs STRICTNESS TIMEOUT_MS)
    set(multiValueArgs EXTRA_ARGS)
    cmake_parse_arguments(TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # Set defaults
    if(NOT TEST_STRICTNESS)
        set(TEST_STRICTNESS 8)
    endif()
    
    if(NOT TEST_TIMEOUT_MS)
        set(TEST_TIMEOUT_MS 300000)
    endif()
    
    # Build command
    set(PLUGINVAL_CMD "${PLUGINVAL_EXECUTABLE}")
    list(APPEND PLUGINVAL_CMD --strictness-level ${TEST_STRICTNESS})
    list(APPEND PLUGINVAL_CMD --timeout-ms ${TEST_TIMEOUT_MS})
    list(APPEND PLUGINVAL_CMD --validate-in-process)
    
    if(TEST_RT_SAFETY_ONLY)
        list(APPEND PLUGINVAL_CMD --enable-allocation-tracker)
    endif()
    
    if(TEST_QUICK)
        list(APPEND PLUGINVAL_CMD --skip-gui-tests)
    endif()
    
    list(APPEND PLUGINVAL_CMD ${TEST_EXTRA_ARGS})
    list(APPEND PLUGINVAL_CMD $<TARGET_FILE:SpectralCanvasPro_VST3>)
    
    add_custom_target(${NAME}
        COMMAND ${PLUGINVAL_CMD}
        DEPENDS SpectralCanvasPro
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Running custom pluginval test: ${NAME}"
    )
endfunction()

# Example custom tests
add_pluginval_test(pluginval_automation
    STRICTNESS 8
    EXTRA_ARGS --random-parameters --parameter-stress
)

add_pluginval_test(pluginval_threading
    STRICTNESS 9
    RT_SAFETY_ONLY
    EXTRA_ARGS --multithreaded-test
)

# Print pluginval setup summary
message(STATUS "Pluginval integration configured:")
message(STATUS "  Executable: ${PLUGINVAL_EXECUTABLE}")
message(STATUS "  Available targets:")
message(STATUS "    - pluginval_test (full validation)")
message(STATUS "    - pluginval_quick (development)")
message(STATUS "    - pluginval_rt_safety (RT-safety focus)")
message(STATUS "    - pluginval_automation (parameter testing)")
message(STATUS "    - pluginval_threading (threading validation)")
if(BUILD_TESTING)
    message(STATUS "  CTest integration: enabled")
endif()