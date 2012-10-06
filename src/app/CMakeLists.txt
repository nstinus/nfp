add_executable(nfpshm
        nfpshm.cpp
        $<TARGET_OBJECTS:core_obj>
        $<TARGET_OBJECTS:algos_obj>
)

target_link_libraries(nfpshm
        ${GLOG_LIBRARY}
)

# install(TARGETS nfpshm DESTINATION bin)

if (${BUILD_TESTS})
   set(ENV{NFP_SHM_DIR} "/tmp")
   set(movie_id 0000100)
   add_test(nfpshm_load nfpshm load ${movie_id})
   add_test(nfpshm_infos nfpshm infos ${movie_id})
   add_test(nfpshm_ratings nfpshm ratings ${movie_id})
   add_test(nfpshm_users nfpshm users ${movie_id})
   add_test(nfpshm_remove nfpshm remove)
   set_property(TEST nfpshm_load PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
   set_property(TEST nfpshm_infos PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
   set_property(TEST nfpshm_ratings PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
   set_property(TEST nfpshm_users PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
   set_property(TEST nfpshm_remove PROPERTY ENVIRONMENT "NFP_SHM_FILES=/tmp")
endif (${BUILD_TESTS})
