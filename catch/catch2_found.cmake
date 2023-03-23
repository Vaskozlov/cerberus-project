  enable_testing()

  file(
          GLOB_RECURSE
          CCL_CATCH_OBJECTS
          "catch/*.cpp"
  )

  add_executable(
          ccl-catch
          ${CCL_CATCH_OBJECTS}
  )

  set_source_files_properties(
          catch/runner.cpp PRIVATE
          PROPERTIES SKIP_PRECOMPILE_HEADERS ON
  )

  target_link_libraries(
          ccl-catch
          ccl
          fmt::fmt
          Catch2::Catch2
          Catch2::Catch2WithMain
  )


  if (${PRECOMPILED_HEADER})
      target_precompile_headers(
              ccl-catch
              PUBLIC
              "pch.hpp"
      )
  endif ()

  add_test(ccl-catch-test ccl-catch)