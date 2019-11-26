add_executable(d2tb_calo D2TB_Calo.cc)
target_link_libraries(d2tb_calo LINK_PUBLIC d2tb)
install(TARGETS d2tb_calo RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
