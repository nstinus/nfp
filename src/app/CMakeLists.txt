add_executable(nfpshm
        nfpshm.cpp
        $<TARGET_OBJECTS:core_obj>
        $<TARGET_OBJECTS:algos_obj>
)

target_link_libraries(nfpshm
        ${GLOG_LIBRARY}
)

install(TARGETS nfpshm DESTINATION bin)

if (${BUILD_TESTING})
   set(ENV{NFP_SHM_DIR} "/tmp")
   set(movie_id 0000100)
   add_test("app/nfpshm::load" nfpshm load ${movie_id})
   add_test("app/nfpshm::infos" nfpshm infos ${movie_id})
   add_test("app/nfpshm::ratings" nfpshm ratings ${movie_id})
   add_test("app/nfpshm::users" nfpshm users ${movie_id})
   add_test("app/nfpshm::remove" nfpshm remove)
   set_property(TEST "app/nfpshm::load" PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
   set_property(TEST "app/nfpshm::infos" PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
   set_property(TEST "app/nfpshm::ratings" PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
   set_property(TEST "app/nfpshm::users" PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
   set_property(TEST "app/nfpshm::remove" PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
endif()
