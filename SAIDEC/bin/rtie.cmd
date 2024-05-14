setopt _conn_init_timeout 30
setopt _conn_socket_buffer_size 49152
setopt _lm_connect_timeout 30
setopt _lm_socket_buffer_size 1024
setopt _output_lines UNKNOWN
setopt _server_connect_timeout 30
setopt _socket_buffer_size 0
setopt data_source_type server
setopt default_rule_priority 10
setopt distribute_enable FALSE
setopt frame_interval 0.0
setopt immediate_chaining TRUE
setopt input_directory "../fichiers/bdc"
setopt knowledge_base migrazur
setopt output_directory "."
setopt priority_threshold 0
setopt queue_enable TRUE
setopt queue_mode depth_first
setopt rule_bindings_threshold 100
setopt unique_datagroup _sdci14379
setopt variable_filter UNKNOWN
setopt variable_table UNKNOWN
setopt warn_on_bad_indirect_slots FALSE
setopt enable_print_file FALSE
compile
run
