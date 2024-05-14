/* --------------------------------------------- */
/* rtmigrazur.cmd -- defaults for RTseClient Migrazur */
/* --------------------------------------------- */

/* ----------------------------------------------------------------- */
/* This file is loaded BEFORE RTClient run 			      */
/* ----------------------------------------------------------------- */

/* ------------------------- */
/* RTserver-specific options */
/* ------------------------- */
setopt socket_connect_timeout  	40.0 
setopt server_keep_alive_timeout	80.0
setopt server_read_timeout	80.0
setopt server_write_timeout	40.0
setopt server_start_timeout  	40.0 
setopt udp_broadcast_timeout   	20.0
setopt server_start_max_tries   10.0
setopt server_auto_flush_size  	0 
setopt default_protocols	tcp
/*setopt log_in_data		/tmp/log_in_client.tmp*/
