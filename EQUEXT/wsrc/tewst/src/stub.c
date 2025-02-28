/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "util.h"
#include <axutil_utils.h>
#include <axutil_types.h>
#include <axiom_soap.h>
#include <axis2_client.h>
#include <axutil_file_handler.h>
#include <axutil_thread.h>
#include <axis2_conf_init.h>
#include <axis2_conf.h>
#include <axutil_param.h>
#include <axis2_transport_receiver.h>
#include <axis2_http_transport.h>
#include <axis2_transport_in_desc.h>
#include <axiom_children_qname_iterator.h>
#include <axis2_http_transport.h>
#include <axis2_http_header.h>
#include <axis2_addr.h>
#include <axiom.h>
#ifdef WSF_RAMPART_ENABLED
#include <rampart_context.h>
#include "rampart_constants.h"
#endif
#include <neethi_options.h>
#include <neethi_util.h>
#include <neethi_policy.h>

#include "constants.h"
#include "option.h"
#include "stub.h"
#include "error.h"

/*
 * RT Server required includes
 */
#include <xdc.h>
#include <xdy.h>
#include <xzst.h>
#include <xzsc.h>

static int is_soap_enabled = 0;
static int is_action = 0;
static int is_soap_out = 0;
static int is_soap_dump = 0;
static int is_get_enabled = 0;
static int is_wsa_enabled = 1;
static int is_mtom_enabled = 1;
static int is_send_only = 0;
static int is_reply_to = 0;
static int is_in_reply_to = 0;
static int is_to = 0;
static int is_from = 0;
static int is_fault_to = 0;
static int is_username = 0;
static int is_password = 0;
static int is_password_file = 0;
static int is_digest = 0;
static int is_xop_in = 0;
static int is_xop_out = 0;
static axis2_char_t *password_buffer;
static axis2_char_t *username_value;
static axis2_char_t *password_file;
static axis2_char_t *private_key_file;
static axis2_char_t *certificate_file;
static axis2_char_t *recipient_certificate_file;
static axis2_char_t *xop_out_dir;
static axis2_char_t *content_type;
static int is_contenty_type = 0;
static axutil_array_list_t *header_list;
static int is_output_http_headers = 0;
static axis2_char_t *output_filename;
static axis2_char_t *server_cert;
static int is_server_cert = 0;
static axis2_bool_t enable_rampart;
static axis2_bool_t enable_signature;
static axis2_bool_t enable_encryption;
static neethi_options_t *neethi_options;
static axis2_char_t *policy_file;

int isComplete = 0;
long timeout_in_milli_seconds = 2000;

extern tewst_cmd_options_t cmd_options_data[];
extern int array_size;


axiom_node_t *
create_policy_node(const axutil_env_t *env,
                   axiom_node_t *node );

axiom_node_t *
create_recipient_token(const axutil_env_t *env,
                       axiom_node_t *parent_om_node);

axiom_node_t *
create_initiator_token(const axutil_env_t *env,
                       axiom_node_t *parent_om_node);

axiom_node_t *
create_username_token(const axutil_env_t *env,
                      axiom_node_t *parent_node);

/* my on_complete callback function */
axis2_status_t AXIS2_CALL
echo_callback_on_complete (struct axis2_callback* callback,
						   const axutil_env_t* 	  env);

/* my on_error callback function */
axis2_status_t AXIS2_CALL
echo_callback_on_error (struct axis2_callback* callback,
						const axutil_env_t*    env,
						int 				   exception);


void* tewst_sct = NULL;
AXIS2_EXTERN void* AXIS2_CALL
tewst_obtain_sct_default(
    const axutil_env_t *env, 
    axis2_bool_t is_encryption, 
    axis2_msg_ctx_t* msg_ctx, 
    axis2_char_t *sct_id, 
    int sct_id_type,
    void* user_params)
{
    
    return tewst_sct;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
tewst_store_sct_default(
    const axutil_env_t *env, 
    axis2_msg_ctx_t* msg_ctx, 
    axis2_char_t *sct_global_id, 
    axis2_char_t *sct_local_id, 
    void *sct, 
    void *user_params)
{
	tewst_sct = sct;
	return AXIS2_SUCCESS;

}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
tewst_delete_sct_default(
    const axutil_env_t *env, 
    axis2_msg_ctx_t* msg_ctx, 
    axis2_char_t *sct_id, 
    int sct_id_type,
    void* user_params)
{
    /* delete method is not implemented, because we are still not supporting sct cancel function */

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[rampart]Using default sct provider delete function.");

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
tewst_validate_sct_default(
    const axutil_env_t *env, 
    axiom_node_t *sct_node, 
    axis2_msg_ctx_t *msg_ctx,
    void *user_params)
{
    /* default implementation does not need to validate anything. We haven't extended the 
     * functionality of sct */

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[rampart]Using default sct provider validate function.");

    return AXIS2_SUCCESS;
}


static axis2_options_t *
tewst_svc_option (axis2_svc_client_t *svc_client,
                     const axutil_env_t *env,
					 axutil_array_list_t *array_list,
					 axiom_node_t *payload)
{
	axis2_options_t *options;
	axis2_endpoint_ref_t *to = NULL;
	axis2_endpoint_ref_t *from = NULL;
	axis2_endpoint_ref_t *reply_to = NULL;
	axis2_endpoint_ref_t *fault_to = NULL;
	axis2_endpoint_ref_t *in_reply_to = NULL;
	axis2_relates_to_t *relates_to = NULL;
	axutil_property_t *dump_property ;
    axutil_property_t *rest_property;

	tewst_options_t *tewst_options = NULL;
	int size;
	int i;
	int ii;
	size = axutil_array_list_size (array_list, env);
    options = axis2_options_create(env);
    neethi_options = neethi_options_create(env);
	header_list = axutil_array_list_create (env, 2);
	for (i = 0; i < size; i++)
	{
	    tewst_options = (tewst_options_t *)
			axutil_array_list_get (array_list, env, i);
	    if (tewst_options)
	    {
			register char *opt = NULL;
			register int  opt_short = 0;
			opt = tewst_options->opt_name;
			opt_short = tewst_options->short_name;

			for (ii = 0; ii < array_size; ii++)
			{
				if (opt)
				{
					if (!strcmp (opt, cmd_options_data[ii].long_name))
						break;
				}
				
				if (opt_short)
				{
					if (cmd_options_data[ii].short_name == opt_short)
						break;
				}
			}

			if (opt || opt_short)
			{
				switch (ii)
				{

					case GET:
					{
						is_get_enabled = 1;
						is_soap_enabled = 0;
						is_wsa_enabled = 0;
						is_mtom_enabled = 0;
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] processed get option ");
					}
					break;
					case CONTENT_TYPE:
					{
						is_contenty_type = 1;
						if (tewst_options->value)
						{			      
							content_type = (char *)tewst_options->value;
						}
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] processed content type ");
					}
					break;
					case SOAP:
					{
						is_soap_enabled = 1;
						/* Set the default soap version of 1.2 */
						axis2_options_set_soap_version(options, env, AXIOM_SOAP12);
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] soap 1.2 selected");
					}
					break;
					case SOAP_OLD:
					{
						is_soap_enabled = 1;
						axis2_options_set_soap_version(options, env, AXIOM_SOAP11);
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] soap 1.1 selected");
					}
					break;
					case SOAP_DUMP:
					{
						is_soap_dump = 1;
						if (!is_soap_enabled)
						{
							is_soap_enabled = 1;
						}
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] soap dumping  ");
					}
					break;
					case SOAP_OUT:
					{
						is_soap_out = 1;
						if (!is_soap_enabled)
						{
							is_soap_enabled = 1;
							axis2_options_set_soap_version(
								options, env, AXIOM_SOAP12);
						} 
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] soap out block");
					}
					break;
					case SOAP_HEADER:
					{
	                    axiom_node_t *soap_header = NULL;
						char *header = (char *) tewst_options->value;
	                    soap_header = tewst_soap_header (env, header);
                        if(soap_header)
                        {
                            if(AXIS2_SUCCESS != axis2_svc_client_add_header(
                                svc_client, env, soap_header))
                            {
                                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                                    "[tewst]Adding soap header failed");
                            }
                        }
                        else
                        {
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                                "[tewst]Creating soap header axiom element"\
                                " for string %s failed", header);
                        }
					}
					break;
					case ACTION:
					{
						is_soap_enabled = 1;
						is_action = 1;
						if (tewst_options->value)
						{
							axis2_options_set_action(
								options,
								env, 
								(char *)tewst_options->value);
						}
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] wsa action block %a", 
										(char *)tewst_options->value);
					}
					break;
					case SEND_ONLY:
					{
						is_send_only = 1;
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] send only block ");
					}
					break;
					case NO_WSA:
					{
						is_wsa_enabled = 0;
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] addressing disabled ");
					}
					break;
					case TO:
					{
						is_to = 1;
						if (tewst_options->value)
						{
							to = axis2_endpoint_ref_create (env, 
															(char *)tewst_options->value);
						}
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] wsa to  %s ", 
										(char *)tewst_options->value);
					}
					break;
					case FROM:
					{
						is_from = 1;
						if (tewst_options->value)
						{
							from = axis2_endpoint_ref_create(env, 
															 (char *)tewst_options->value);
						}
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] wsa from  %s ", 
										(char *)tewst_options->value);
					}
					break;
					case REPLY_TO:
					{
						is_reply_to = 1;
						if (tewst_options->value)
						{
							reply_to = axis2_endpoint_ref_create(env, 
																 (char *)tewst_options->value);
						}
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] wsa reply to  %s ", 
										(char *)tewst_options->value);
					}
					break;
					case IN_REPLY_TO:
					{
						is_in_reply_to = 1;
						if (tewst_options->value)
						{
							relates_to = axis2_relates_to_create (env, 
																  (char *)tewst_options->value,
																  "http://www.w3.org/2005/08/addressing/reply");
						}
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] wsa in-reply-to  %s ", 
										(char *)tewst_options->value);
					}
					break;
					case FAULT_TO:
					{
						is_fault_to = 1;
						if (tewst_options->value)
						{
							fault_to = axis2_endpoint_ref_create (env, 
																  (char *)tewst_options->value);
						}
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] wsa fault-to  %s ", 
										(char *)tewst_options->value);
					}
					break;
					case NO_MTOM:
					{
						is_mtom_enabled = 0;
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] mtom disabled  ");
					}
					break;
					case USER:
					{
						is_soap_enabled = 1;
                        /*is_username = 1;*/
                        enable_rampart = AXIS2_TRUE;
                        neethi_options_set_is_username_token(neethi_options, env, AXIS2_TRUE);
						username_value = (axis2_char_t *)tewst_options->value;
					}
					break;
					case PASSWORD:
					{
						is_password = 1;
						is_soap_enabled = 1;
                        password_buffer = axutil_strdup(env,  tewst_options->value);
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] security password block ");
					}
					break;
					case TIMESTAMP:
					{
                        enable_rampart = AXIS2_TRUE;
                        neethi_options_set_include_timestamp(neethi_options, env, AXIS2_TRUE);
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] security timestamp block ");
					}
                    break;
                    case ENCRYPT_PAYLOAD:
                    {
                        enable_rampart = AXIS2_TRUE;
                        enable_encryption = AXIS2_TRUE;
                        neethi_options_set_encrypt_body(neethi_options, env, AXIS2_TRUE);
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[tewst] Set encrypt body ");
                    }
					break;
                    case SIGN_BODY:
                    {
                        enable_rampart = AXIS2_TRUE;
                        enable_signature = AXIS2_TRUE;
                        neethi_options_set_sign_body(neethi_options, env, AXIS2_TRUE);
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[tewst] Set sign body ");
                    }
                    break;
                    case ENCRYPT_BEFORE_SIGNING:
                    {
                        enable_rampart = AXIS2_TRUE;
                        neethi_options_set_encrypt_before_sign(neethi_options, env, AXIS2_TRUE);
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[tewst] Set Encrypt Before signing ");
                    }
                    break;
                    case ENCRYPT_SIGNATURE:
                    {
                        enable_rampart = AXIS2_TRUE;
                        neethi_options_set_signature_protection(neethi_options, env, AXIS2_TRUE);
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[tewst] Set Signature Protection ");
                    }
                    break;
                    case KEY:
                    {
                        enable_rampart = AXIS2_TRUE;
                        private_key_file = (axis2_char_t *)tewst_options->value;
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[tewst] Set Private Key ");
                    }
                    break;
                    case CERTIFICATE:
                    {
                        enable_rampart = AXIS2_TRUE;
                        certificate_file = (axis2_char_t *)tewst_options->value;
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[tewst] Set Certificate ");
                    }
                    break;
                    case RECIPIENT_CERTIFICATE:
                    {
                        enable_rampart = AXIS2_TRUE;
                        recipient_certificate_file = (axis2_char_t *)tewst_options->value;
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[tewst] Set receiver Certificate ");
                    }
                    break;
                    case ALGORITHMSUITE:
                    {
                        enable_rampart = AXIS2_TRUE;
                        neethi_options_set_algorithmsuite(neethi_options, env, 
                                    (axis2_char_t *)tewst_options->value);
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[tewst] Set Algorithmsuite ");
                    }
                    break;
					case PASSWORD_FILE:
					{
						is_password_file = 1;
						is_password = 1;
                        is_username = 1;
						is_soap_enabled = 1;
						password_file = (char *) tewst_options->value;
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] security password file ");
					}
					break;
					case DIGEST:
					{
						is_digest = 1;
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] security digest block ");
					}
					break;
                    case POLICY_FILE:
                    {
                        enable_rampart = AXIS2_TRUE;
                        policy_file = (axis2_char_t *)tewst_options->value;
                        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[tewst] Set policy file:%s", policy_file);
                    }
                    break;
					case XOP_IN:
					{
						axis2_char_t *dir = NULL;
						is_xop_in = 1;
						dir = (axis2_char_t *)tewst_options->value;
						tewst_set_attachment (env, payload, dir, is_mtom_enabled);
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] xop in block");
					}
					break;
					case XOP_OUT:
					{
						is_xop_out = 1;
						xop_out_dir = (axis2_char_t *)tewst_options->value;
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] xop out block");
					}
					break;
					case HTTP_HEADER:
					{
						axis2_http_header_t *axis2_header;
						axutil_property_t *header_property;
						const char *header = (char *) tewst_options->value;
						axis2_header = axis2_http_header_create_by_str (env, header);
						axutil_array_list_add (header_list, env, (void *)axis2_header);
						
						header_property = axutil_property_create(env);
						if (header_property && header_list)
						{
							axutil_property_set_value(header_property, env, 
                                                      header_list);
							axutil_property_set_free_func (header_property, env, axutil_array_list_free_void_arg);
							axis2_options_set_property(options, env, AXIS2_TRANSPORT_HEADER_PROPERTY,
													   header_property);
						}
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] add http header");
					}
					break;
					case OUTPUT_HTTP_HEADERS:
					{
						is_output_http_headers = 1;
						output_filename = (char *) tewst_options->value;
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] output http header %s", 
										(char *) tewst_options->value);
					}
					break;
					case SERVER_CERT:
					{
						axutil_property_t *server_cert_property;
						server_cert = (char *)tewst_options->value;
						server_cert_property = axutil_property_create (env);
						if (server_cert_property && server_cert)
						{
							axutil_property_set_value(server_cert_property, env,
                                                      (void *)axutil_strdup (env, server_cert));
							axis2_options_set_property(options, env, AXIS2_SSL_SERVER_CERT,
													   server_cert_property);
						}
						is_server_cert = 1;
						AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
										"[tewst] server certificate path  %s",
										(char *) tewst_options->value);
					}
					break;
				};
			}
	    }
	}

	if (!is_soap_enabled)
	{
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
						"[tewst] soap not enabled");
		is_wsa_enabled = 0;
 		is_mtom_enabled = 0;

		rest_property = axutil_property_create(env);
		axutil_property_set_value(rest_property, env, 
                                  (void *)axutil_strdup (env, AXIS2_VALUE_TRUE));
		axis2_options_set_property(options, env, AXIS2_ENABLE_REST,
								   rest_property);
		if (AXIS2_TRUE == is_get_enabled)
		{
            axis2_options_set_http_method(options, env, AXIS2_HTTP_GET);
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] set get property ");
		}

		if (is_contenty_type)
		{
			axutil_hash_t *content_type_hash;
			axutil_property_t *content_type_property;
			content_type_hash = axutil_hash_make (env);
			if(content_type)
			{
				axutil_hash_set (content_type_hash, 
                                 AXIS2_HTTP_HEADER_CONTENT_TYPE, 
                                 AXIS2_HASH_KEY_STRING, 
                                 tewst_content_type (env, content_type));
			}
			content_type_property = axutil_property_create(env);
			axutil_property_set_value(content_type_property, env, 
                                      content_type_hash);
/* 			AXUTIL_PROPERTY_SET_FREE_FUNC (property, env, axutil_hash_free); */
			axis2_options_set_property(options, env, AXIS2_USER_DEFINED_HTTP_HEADER_CONTENT_TYPE,
									   content_type_property);
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] set contenty type");
		}
	}

	if (is_wsa_enabled)
	{
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
						"[tewst] addressing  enabled");
	    if (is_send_only)
	    {
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] addressing  enabled sendonly ");
			if(!is_reply_to)
			{
				reply_to = axis2_endpoint_ref_create(env,
													 "http://www.w3.org/2005/08/addressing/none");
				axis2_options_set_reply_to(options, env, reply_to);
			}
	    }

	    if (is_to)
	    {
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] adding \"to\" to options");
			if (to)
			{
				axis2_options_set_to(options, env, to);
			}
	    }
	    
	    if (is_from)
	    {
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
							"[tewst] adding from to options");
			if (from)
			{
				axis2_options_set_from (options, env, from);
			}
	    }

	    if (is_reply_to)
	    {
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
							"[tewst] adding reply-to to options");
			if (reply_to)
			{
				axis2_options_set_reply_to (options, env, reply_to);
			}
	    }

	    if (is_in_reply_to)
	    {
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
							"[tewst] adding in-reply-to to options");
			if (in_reply_to)
			{
			
				axis2_options_set_relates_to (options, env, relates_to);
			}
	    }

	    if (is_fault_to)
	    {
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] adding fault-to to options");
			if (fault_to)
			{
				axis2_options_set_fault_to (options, env, fault_to);
			}
	    }
	}

	if (is_soap_dump)
	{
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
						"[tewst] setting soap-dump property to options");
	    dump_property = axutil_property_create (env);
	    axutil_property_set_value (dump_property,
                                   env,
                                   (void *)axutil_strdup (env, AXIS2_VALUE_TRUE));
	    axis2_options_set_property (options, env, "dump", dump_property);
	}

	return options;
}


axis2_status_t AXIS2_CALL
tewst_stub_invoke(
	const axutil_env_t *env,
	axutil_array_list_t *array_list,
	axis2_char_t *dest_uri,
	axis2_char_t *input,
	int fire_and_forget)
{

	const axis2_char_t *address = NULL;
	axis2_endpoint_ref_t* endpoint_ref = NULL;
	axis2_options_t *options = NULL;
	const axis2_char_t *client_home = NULL;
	axis2_svc_client_t* svc_client = NULL;
	axiom_node_t *payload = NULL;
	axiom_node_t *ret_node = NULL;
	axis2_status_t status = AXIS2_FAILURE;

    enable_rampart = AXIS2_FALSE;
    enable_encryption = AXIS2_FALSE;
    enable_signature = AXIS2_FALSE;    

	AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
					"[tewst] payload %s \n", input);
	payload = tewst_payload (env, input);

	client_home = AXIS2_GETENV("WSFC_HOME");
	if (!client_home)
	{
#ifdef WSFC_PREFIX    
        client_home = WSFC_PREFIX;
        if (!client_home)
        {
#endif        
    		printf ("WSFC_HOME environment variable not set properly. Please recheck.\n");
		    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
		        "[tewst] WSFC_HOME is empty");
            return TEWST_FAILURE;
#ifdef WSFC_PREFIX    
        }
        else
    		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                "WSFC_HOME environment variable not set properly. Using %s as WSFC_HOME.\n", client_home);
#endif        
	}
    
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
        "[tewst] WSFC_HOME %s", client_home);

	if (dest_uri)
	{
	    address = dest_uri;
	}
else
	{
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
						"[tewst] destination uri not available");
	    return TEWST_ERROR_NO_DESTURI;
	}

	if (address)
	    endpoint_ref = axis2_endpoint_ref_create(env, address);
	else
	    return TEWST_FAILURE;

	svc_client = axis2_svc_client_create (env, client_home);
	AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
					"[tewst] svc client created");

	if (!svc_client)
	{
	    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
						" %d :: %s", env->error->error_number,
						AXIS2_ERROR_GET_MESSAGE(env->error));
	    return -1 /* TEWST_ERROR_SENDING_MSG */;
	}

	options = tewst_svc_option (svc_client, env, array_list, payload);
	AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
					"[tewst] svc client created");

	if (endpoint_ref)
	    axis2_options_set_to(options, env, endpoint_ref);
	else
	    return TEWST_FAILURE;

	if (is_soap_enabled)
	{
		if (is_wsa_enabled)
		{
			if (!is_action)
				axis2_options_set_action (options, env, "http://ws.apache.org/axis2/c");
			axis2_svc_client_engage_module(svc_client, env, AXIS2_MODULE_ADDRESSING);
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] addressing module engaged");
		}

        if(enable_rampart)
		{
#ifdef WSF_RAMPART_ENABLED
		    axiom_node_t *root_om_node = NULL;
            rampart_context_t *rampart_context = NULL;
            axis2_svc_ctx_t *svc_ctx = NULL;
            axis2_conf_ctx_t *conf_ctx = NULL;
            axis2_conf_t *conf = NULL;
            axutil_param_t *security_param = NULL;
            int defualt_ttl = 300;
            neethi_policy_t *policy = NULL;

            rampart_context = rampart_context_create(env);

        	axis2_svc_client_engage_module (svc_client, env, "rampart");
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] rampart module engaged");

            if (!is_action)
                axis2_options_set_action (options, env, "http://example.com/ws/2004/09/policy/Test/EchoRequest"/*"http://ws.apache.org/axis2/c"*/);
            
            axis2_svc_client_engage_module(svc_client, env, AXIS2_MODULE_ADDRESSING);
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "[tewst] addressing module engaged");

            /*Create the policy, from file*/
            policy = neethi_util_create_policy_from_file(env, policy_file);
            if(policy)
            {
                if(!policy)
                {
                    printf("\nPolicy creation failed from the file. %s\n", policy_file);
                }

                status = axis2_svc_client_set_policy(svc_client, env, policy);
                printf("\nPolicy set for the file. %s\n", policy_file);

                if(status == AXIS2_FAILURE)
                {
                    printf("Policy setting failed\n");
                }
            }
            else
            {
                root_om_node = neethi_options_get_root_node(neethi_options, env);

                if(!root_om_node)
                {
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                               "[tewst] Policy Creation failed");
                    return TEWST_FAILURE;
                }

                status = axis2_svc_client_set_policy_from_om(svc_client, env, root_om_node);            
                if(status != AXIS2_SUCCESS)
                {
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                               "[tewst] Policy Creation failed");
                    return TEWST_FAILURE;
                }
            }
 
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewst] setting creating policy node ");

            if (rampart_context_set_user(rampart_context, env,
                                     (axis2_char_t *)username_value) == AXIS2_SUCCESS)
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewst] setting username ");

            /*We set the default ttl now. This needs to be changed when we process the user given
            ttl*/

            rampart_context_set_ttl(rampart_context, env, defualt_ttl);            

            if (is_password_file)
            {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "[tewst] processing password file");
                if (username_value)
                {
                    password_buffer = tewst_get_password (env, username_value, password_file);
                    if (password_buffer)
                    {
                        is_password = 1;
                        is_soap_enabled = 1;
                    }
                }
            }

            if (is_digest)
            {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "[tewst] adding digest password property");
                if(rampart_context_set_password_type(rampart_context, env,
                                                 (axis2_char_t *)"Digest") == AXIS2_SUCCESS)
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewst] setting passwordType ");
            }

            if (password_buffer)
            {
                if(rampart_context_set_password(rampart_context, env,
                                            (axis2_char_t *)password_buffer) == AXIS2_SUCCESS)
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewst] setting password ");

            }

            if(private_key_file)
            {
                rampart_context_set_private_key_file(
                            rampart_context, env, private_key_file);   
            }
            else
            {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewst] Private key file not specified. ");
                return TEWST_FAILURE;
            }
            if(certificate_file)
            {
                rampart_context_set_certificate_file(
                            rampart_context, env, certificate_file);
            }
            else
            {    
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewst] Certificate file not specified. ");
                return TEWST_FAILURE;
            }
            if(recipient_certificate_file)
            {
                rampart_context_set_receiver_certificate_file(
                            rampart_context, env, recipient_certificate_file);
            }
            else
            {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewst] Reciever Certificate not specified. ");
                return TEWST_FAILURE;
            }

			rampart_context_set_obtain_security_context_token_fn(
                rampart_context, env, tewst_obtain_sct_default);
            rampart_context_set_store_security_context_token_fn(
                rampart_context, env, tewst_store_sct_default);
            rampart_context_set_delete_security_context_token_fn(
                rampart_context, env, tewst_delete_sct_default);
            rampart_context_set_validate_security_context_token_fn(
                rampart_context, env, tewst_validate_sct_default);


            svc_ctx = axis2_svc_client_get_svc_ctx(svc_client, env);
            conf_ctx = axis2_svc_ctx_get_conf_ctx(svc_ctx, env);
            conf = axis2_conf_ctx_get_conf(conf_ctx, env);

            security_param = axutil_param_create(env, RAMPART_CONFIGURATION, (void *)rampart_context);
            axis2_conf_add_param(conf, env, security_param);
#endif
        }

		if (is_mtom_enabled)
		{
			axis2_options_set_enable_mtom (options, env, AXIS2_TRUE);
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] mtom option enabled");
		}
	}

	status = axis2_options_set_timeout_in_milli_seconds(options, env, timeout_in_milli_seconds);


	if (svc_client && options)
	    axis2_svc_client_set_options(svc_client, env, options);
	else
	    return TEWST_FAILURE;


	if (payload && svc_client)
	{
	    if (dest_uri)
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Sending request to: %s \n", dest_uri);

#ifdef AXIS2_SSL_ENABLED
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
						"[tewst] compiled with ssl enabled");
		if (strstr (dest_uri, "https"))
		{
			if (is_server_cert)
				tewst_set_https_transport_parameretes (env, svc_client);
			else
			{
				printf ("server certificate not found \n");
				AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
								"[tewst] server certificate not found");
				return TEWST_FAILURE;
			}
		}
#endif
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
						"[tewst] prepareing for svc client send receive ");

		if (fire_and_forget)
		{
			/* printf("Fire and forget\n"); */

		    axis2_callback_t *callback = NULL;

		    /* Create the callback object with default on_complete and on_error
		       callback functions */
		    callback = axis2_callback_create (env);

		    /* Set our on_complete fucntion pointer to the callback object */
		    axis2_callback_set_on_complete (callback, echo_callback_on_complete);

		    /* Set our on_error function pointer to the callback object */
		    axis2_callback_set_on_error (callback, echo_callback_on_error);

		    /* Send request */
		   axis2_svc_client_send_receive_non_blocking (svc_client, env,
		                                                payload, callback);

		   sleep(2);

			/* printf("End fire and forget\n"); */

		    return TEWST_SUCCESS;

		}
		else if (!is_send_only)
			ret_node = axis2_svc_client_send_receive(svc_client, env, payload);
		else
			status = axis2_svc_client_send_robust(svc_client, env, payload);

		if (is_soap_dump)
		{
			tewst_soap_out (env, svc_client, 0);
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] soap dump successful");
			return TEWST_SUCCESS;
		}

		if (is_send_only && (status == AXIS2_SUCCESS))
		{
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] soap send successful");
			return TEWST_SUCCESS;
		}
	}
	else
	    return TEWST_FAILURE;

	if (ret_node)
	{
		axiom_element_t *element; 
		axis2_char_t *node_name; 
		axis2_char_t *node_value;
        axis2_char_t *om_str = NULL;
        char AlcReturnCode[20];

		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
						"[tewst] response return node found");
	    if(is_send_only)
	    {
			element = axiom_node_get_data_element(ret_node,
												  env);
			node_name = axiom_element_get_localname(element,
													env);
			if(0 != axutil_strcasecmp(node_name, "fault"))
			{
				AXIS2_FREE (env->allocator, node_name);
				return TEWST_ERROR_RECEIVING_RESPONSE;
			}
			AXIS2_FREE (env->allocator, node_name);
	    }

		if (is_output_http_headers)
			tewst_output_http_headers (env, svc_client, output_filename);

		if (is_xop_out)
		{
			tewst_get_attachment (env, ret_node, xop_out_dir, is_mtom_enabled);
			tewst_soap_out (env, svc_client, 1);
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] xop out successful");
		}
		else if (is_soap_out)
	    {
			tewst_soap_out (env, svc_client, 1);
			AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
							"[tewst] soap out SUCCESSFUL");
	    }
	    else
	    {
			om_str = axiom_node_to_string(ret_node, env);
			if (om_str)
            {
				/*
				 * printf("Réponse : %s\n", om_str);
				 */
				/* printf("Réponse : %s\n", om_str); */

				/* DONE : Analyse de la réponse ici et récup de la session ID */
				/* TODO : parser proprement pour trouver le noeud fwSessionId dans login Response */
				axiom_node_t *scan_node = NULL;
				axiom_node_t *parent_node = NULL;
				scan_node = axiom_node_get_first_child(ret_node, env);
				scan_node = axiom_node_get_first_child(scan_node, env);
				element = axiom_node_get_data_element(scan_node, env);
				node_name = axiom_element_get_localname(element, env);

				if ( ! strcmp(node_name, "loginResponse") )
				{
					parent_node = scan_node;

					scan_node = axiom_node_get_first_child(parent_node, env);
					element = axiom_node_get_data_element(scan_node, env);
					node_name = axiom_element_get_localname(element, env);
					node_value= axiom_element_get_text(element, env,scan_node);

					if ( ! strcmp(node_name, "resultCode") )
					{
						if ( ! strcmp(node_value, "SUCCESS") )
						{
							scan_node = axiom_node_get_last_child(parent_node, env);
							element = axiom_node_get_data_element(scan_node, env);
							node_name = axiom_element_get_localname(element, env);
							node_value= axiom_element_get_text(element, env,scan_node);

							if ( ! strcmp(node_name, "fwSessionId") )
							{
								/*
								 * AlcSessionId = pointeur de chaine
								 * +
								 * AlcSessionId = malloc ( strlen(node_value)*sizeof(char*))
								 *
								 * ou
								 *
								 * tronquage :
								 *
								 * strncpy(AlcSessionId,node_value,sizeof(AlcSessionId)/sizeof(char*));
								 */
								strcpy(AlcSessionId,node_value);
								return TEWST_SUCCESS;
							}
							else
							{
								/* La réponse est vérolée */
								printf ("Bad answer - Node name = %s (null)\n", node_name);
								strcpy(AlcSessionId,"0");
								printf("Réponse : %s\n", om_str);
								return TEWST_FAILURE;
							}

						}
						else
						{
							/* Le login a été refusé */
							printf ("Bad answer - Node name = %s (null)\n", node_name);
							strcpy(AlcSessionId,"0");
							printf("Réponse : %s\n", om_str);
							return TEWST_FAILURE;
						}

					}
					else
					{
						/* On a reçu autre chose qu'un result code : c'est pas bon */
						printf ("Bad answer - Node name = %s (null)\n", node_name);
						strcpy(AlcSessionId,"0");
						printf("Réponse : %s\n", om_str);
						return TEWST_FAILURE;
					}

				}
				else if ( ! strcmp(node_name, "makeCallResponse" ) )
				{
					/* On a recu autre chose qu'une login response */

					parent_node = scan_node;

					scan_node = axiom_node_get_first_child(parent_node, env);
					element = axiom_node_get_data_element(scan_node, env);
					node_name = axiom_element_get_localname(element, env);
					node_value= axiom_element_get_text(element, env,scan_node);

					if ( ! strcmp(node_name, "resultCode") )
					{
						if ( ! strcmp(node_value, "SUCCESS") )
						{
							strcpy(AlcReturnCode,"SUCCESS");
							return TEWST_SUCCESS;
						}
						else
						{
							strcpy(AlcReturnCode,node_value);
							printf("Réponse : %s\n", om_str);
							return TEWST_FAILURE;
						}
					}
					else
					{
						/* On a reçu autre chose qu'un result code : c'est pas bon */
						printf("Réponse : %s\n", om_str);
						return TEWST_FAILURE;
					}


				}
				else if ( ! strcmp(node_name, "getCurrentCallInfoResponse" ) )
				{
					/* On a recu autre chose qu'une login response
					 *
					 * <resultCode>SUCCESS</resultCode>
					 * <comment></comment>
					 * <callPresent>true</callPresent>
					 * <initialCalledNumber></initialCalledNumber>
					 * <name></name>
					 * <number>15088</number>
					 * <redirectedCall>false</redirectedCall>
					 * <state>RELEASING</state>
					 */
					parent_node = scan_node;

					scan_node = axiom_node_get_last_child(parent_node, env);
					element = axiom_node_get_data_element(scan_node, env);
					node_name = axiom_element_get_localname(element, env);
					node_value= axiom_element_get_text(element, env,scan_node);

					if ( ! strcmp(node_name, "state") )
					{
						if ( ! strcmp ( node_value, "INIT" ) )
						{
							AlcCallPresent=1;
							AlcCallState=XDC_CONNECT;
						}
						else if ( ! strcmp ( node_value, "RINGING_OUTGOING" ) )
						{
							AlcCallPresent=1;
							AlcCallState=XDC_RING;
						}
						else if ( ! strcmp ( node_value, "ACTIVE" ) )
						{
							AlcCallPresent=1;
							AlcCallState=XDC_OK;
						}
						else if ( ! strcmp ( node_value, "BUSY_TONE" ) )
						{
							AlcCallPresent=0;
							AlcCallState=XDC_BUSY;
						}
						else if ( ! strcmp ( node_value, "UNKNOWN" ) )
						{
							AlcCallPresent=0;
							AlcCallState=XDC_LIBRE;
						}
						else
						{
							AlcCallPresent=0;
							AlcCallState=XDC_ERROR;
						}

						return TEWST_SUCCESS;
					}
					else
					{
						/* La réponse est vérolée */
						printf("Réponse : %s\n", om_str);
						return TEWST_FAILURE;
					}

				}
				else if ( ! strcmp(node_name, "getTelephonicStateResponse" ) )
				{
					/* On a recu autre chose qu'une login response
					 * <getTelephonicStateResponse>
					 * <resultCode>SUCCESS</resultCode>
					 * <comment/>
					 * −
					 * <telephonicState>
					 * −
					 * <ns1:capabilities>
					 * <ns1:associateModificationAllowed>false</ns1:associateModificationAllowed>
					 * <ns1:businessCallAllowed>false</ns1:businessCallAllowed>
					 * <ns1:callBackRequestAllowed>false</ns1:callBackRequestAllowed>
					 * <ns1:callPickupAllowed>false</ns1:callPickupAllowed>
					 * <ns1:campOnAllowed>false</ns1:campOnAllowed>
					 * <ns1:cancelCallBackAllowed>false</ns1:cancelCallBackAllowed>
					 * <ns1:cancelForwardAllowed>true</ns1:cancelForwardAllowed>
					 * <ns1:cancelOverflowAllowed>false</ns1:cancelOverflowAllowed>
					 * <ns1:doNotDisturbAllowed>true</ns1:doNotDisturbAllowed>
					 * <ns1:forwardOnBusyAllowed>true</ns1:forwardOnBusyAllowed>
					 * <ns1:forwardOnBusyOrNoAnswerAllowed>true</ns1:forwardOnBusyOrNoAnswerAllowed>
					 * <ns1:forwardOnNoAnswerAllowed>true</ns1:forwardOnNoAnswerAllowed>
					 * <ns1:forwardToExternalNumberAllowed>false</ns1:forwardToExternalNumberAllowed>
					 * <ns1:groupCallPickupAllowed>true</ns1:groupCallPickupAllowed>
					 * <ns1:immediateForwardAllowed>true</ns1:immediateForwardAllowed>
					 * <ns1:lineProgrammable>true</ns1:lineProgrammable>
					 * <ns1:lockPhoneSetAllowed>true</ns1:lockPhoneSetAllowed>
					 * <ns1:makeCallAllowed>true</ns1:makeCallAllowed>
					 * <ns1:overflowOnBusyAllowed>false</ns1:overflowOnBusyAllowed>
					 * <ns1:overflowOnBusyOrNoAnswerAllowed>false</ns1:overflowOnBusyOrNoAnswerAllowed>
					 * <ns1:overflowOnNoAnswerAllowed>false</ns1:overflowOnNoAnswerAllowed>
					 * <ns1:secretCodeModificationAllowed>true</ns1:secretCodeModificationAllowed>
					 * <ns1:secretIdentityAllowed>true</ns1:secretIdentityAllowed>
					 * <ns1:unparkCallAllowed>true</ns1:unparkCallAllowed>
					 * </ns1:capabilities>
					 * <ns2:currentCallRef>-1</ns2:currentCallRef>
					 * </telephonicState>
					 </getTelephonicStateResponse>
					 */

					parent_node = scan_node;

					scan_node = axiom_node_get_first_child(parent_node, env);
					element = axiom_node_get_data_element(scan_node, env);
					node_name = axiom_element_get_localname(element, env);
					node_value= axiom_element_get_text(element, env,scan_node);

					if ( ! strcmp(node_name, "resultCode") )
					{
						if ( ! strcmp(node_value, "SUCCESS") )
						{
							scan_node = axiom_node_get_last_child(parent_node, env);
							element = axiom_node_get_data_element(scan_node, env);
							node_name = axiom_element_get_localname(element, env);
							node_value= axiom_element_get_text(element, env,scan_node);


							if ( ! strcmp(node_name, "telephonicState") )
							{
								scan_node = axiom_node_get_last_child(scan_node, env);
								element = axiom_node_get_data_element(scan_node, env);
								node_name = axiom_element_get_localname(element, env);
								node_value= axiom_element_get_text(element, env,scan_node);


								if ( ! strcmp(node_name, "currentCallRef") )
								{
									if ( ! strcmp ( node_value, "-1" ) )
									{
										AlcCallPresent=0;
										AlcCallState=XDC_LIBRE;
									}
									else
									{
										AlcCallPresent=0;
										strcpy(AlcCurrentCallRef,node_value);
										strcpy(AlcReturnCode,"SUCCESS");
									}

									return TEWST_SUCCESS;
								}
								else
								{
									/* La réponse est vérolée */
									printf("Réponse : %s\n", om_str);
									return TEWST_FAILURE;
								}

							}
							else
							{
								/* La réponse est vérolée */
								printf("Réponse : %s\n", om_str);
								return TEWST_FAILURE;
							}


						}
						else
						{
							strcpy(AlcReturnCode,node_value);
							printf("Réponse : %s\n", om_str);
							return TEWST_FAILURE;
						}
					}
					else
					{
						/* On a reçu autre chose qu'un result code : c'est pas bon */
						printf("Réponse : %s\n", om_str);
						return TEWST_FAILURE;
					}

				}
				else if ( ! strcmp(node_name, "holdCallResponse" ) )
				{
					parent_node = scan_node;

					scan_node = axiom_node_get_first_child(parent_node, env);
					element = axiom_node_get_data_element(scan_node, env);
					node_name = axiom_element_get_localname(element, env);
					node_value= axiom_element_get_text(element, env,scan_node);

					if ( ! strcmp(node_name, "resultCode") )
					{
						if ( ! strcmp(node_value, "SUCCESS") )
						{
							return TEWST_SUCCESS;
						}
						else
						{
							strcpy(AlcReturnCode,node_value);
							printf("Réponse : %s\n", om_str);
							return TEWST_FAILURE;
						}
					}
					else
					{
						/* On a reçu autre chose qu'un result code : c'est pas bon */
						printf("Réponse : %s\n", om_str);
						return TEWST_FAILURE;
					}

				}
				else if ( ! strcmp(node_name, "takeCallResponse" ) )
				{
					parent_node = scan_node;

					scan_node = axiom_node_get_first_child(parent_node, env);
					element = axiom_node_get_data_element(scan_node, env);
					node_name = axiom_element_get_localname(element, env);
					node_value= axiom_element_get_text(element, env,scan_node);

					if ( ! strcmp(node_name, "resultCode") )
					{
						if ( ! strcmp(node_value, "SUCCESS") )
						{
							return TEWST_SUCCESS;
						}
						else
						{
							strcpy(AlcReturnCode,node_value);
							printf("Réponse : %s\n", om_str);
							return TEWST_FAILURE;
						}
					}
					else
					{
						/* On a reçu autre chose qu'un result code : c'est pas bon */
						printf("Réponse : %s\n", om_str);
						return TEWST_FAILURE;
					}
				}
				else
				{
					/* On a recu autre chose qu'une response "connue" */
					printf("Réponse : %s\n", om_str);
					return TEWST_FAILURE;
				}

			    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
					"[tewst] response retrieval successful %s", om_str);
            }
			ret_node = NULL;
	    }
	}
	else 
	{
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
						"[tewst] invoke response retrival failed");
		return TEWST_FAILURE;
	}
	if (password_buffer)
	    AXIS2_FREE (env->allocator, password_buffer);
	if (svc_client)
		axis2_svc_client_free (svc_client, env);

    if(neethi_options)
    {
        neethi_options_free(neethi_options, env);
        neethi_options = NULL;
    }    

	return TEWST_SUCCESS;
}

axiom_node_t *
create_policy_node(const axutil_env_t *env,
                   axiom_node_t *parent_om_node) 
{
    axiom_node_t *policy_om_node = NULL;
    axiom_element_t *policy_om_ele = NULL;
    axiom_namespace_t *wsp_ns = NULL;

    wsp_ns = axiom_namespace_create(env, WS_POLICY_NAMESPACE_URI, WS_POLICY_NAMESPACE);
    policy_om_ele = axiom_element_create(env, parent_om_node, WS_POLICY, wsp_ns, &policy_om_node);

    return policy_om_node;


}

axiom_node_t *
create_initiator_token(const axutil_env_t *env,
                       axiom_node_t *parent_om_node)
{
    axiom_node_t *in_token_om_node = NULL;
    axiom_node_t *policy_om_node1 = NULL;
    axiom_node_t *x509_om_node = NULL;
    axiom_node_t *policy_om_node2 = NULL;
    axiom_node_t *token_id_om_node = NULL;
    axiom_node_t *tmp_node = NULL; /* if wrong option is found earlier
                                    * node should be given back */

    axiom_element_t *in_token_om_ele = NULL;
    axiom_element_t *x509_om_ele = NULL;

    axiom_attribute_t *attr = NULL;

    axiom_namespace_t *sp_ns = NULL;
    
    tmp_node = parent_om_node;
    sp_ns = axiom_namespace_create(env,WS_SEC_POLICY_NAMESPACE_URI,  WS_SEC_POLICY_NAMESPACE);
    in_token_om_ele = axiom_element_create(env, parent_om_node, WS_INITIATOR_VAL, sp_ns, &in_token_om_node);
    policy_om_node1 = create_policy_node(env, in_token_om_node);

    x509_om_ele = axiom_element_create(env, policy_om_node1, WS_X509_TOKEN_VAL, sp_ns, &x509_om_node);

    attr = axiom_attribute_create(env,WS_INCLUDE_TOKEN,  WS_INCLUDE_TOKEN_URI,
                                  sp_ns);
    axiom_element_add_attribute(x509_om_ele, env, attr, x509_om_node);
    policy_om_node2 = create_policy_node(env, x509_om_node);
    axiom_element_create(env, policy_om_node2, "WssX509V3Token10", sp_ns, &token_id_om_node);
    return token_id_om_node;
}


/** for encryption part of the policy file */

axiom_node_t *
create_recipient_token(const axutil_env_t *env,
                       axiom_node_t *parent_om_node)

{
    axiom_node_t *rec_token_om_node = NULL;
    axiom_node_t *policy_om_node1 = NULL;
    axiom_node_t *x509_om_node = NULL;
    axiom_node_t *policy_om_node2 = NULL;
    axiom_node_t *token_id_om_node = NULL;
    axiom_node_t *tmp_node = NULL;

    axiom_element_t *rec_token_om_ele = NULL;
    axiom_element_t *x509_om_ele = NULL;

    axiom_attribute_t *attr = NULL;

    axiom_namespace_t *sp_ns = NULL;
    
    tmp_node = parent_om_node;


    sp_ns = axiom_namespace_create(env,WS_SEC_POLICY_NAMESPACE_URI,  WS_SEC_POLICY_NAMESPACE);

    rec_token_om_ele = axiom_element_create(env, parent_om_node,  WS_RECIPIENT_TOKEN, sp_ns, &rec_token_om_node);
    policy_om_node1 = create_policy_node(env, rec_token_om_node);
    x509_om_ele = axiom_element_create(env, policy_om_node1, WS_X509_TOKEN_VAL, sp_ns, &x509_om_node);

    /* Here uri may change according to the options difined in policy spec
     * e.g - 'Always' may be changed as 'Never'*/
    attr = axiom_attribute_create(env,  WS_INCLUDE_TOKEN ,  WS_INCLUDE_TOKEN_URI,
                                  sp_ns);
    axiom_element_add_attribute(x509_om_ele, env, attr, x509_om_node);
    policy_om_node2 = create_policy_node(env, x509_om_node);
    axiom_element_create(env, policy_om_node2, "WssX509V3Token10", sp_ns, &token_id_om_node);
    return token_id_om_node;
}

axiom_node_t *
create_username_token(const axutil_env_t *env,
                      axiom_node_t *parent_node)
{
    axiom_node_t *signsupport_om_node = NULL;
    axiom_node_t *policy_om_node = NULL;
    axiom_node_t *ut_om_node = NULL;

    axiom_element_t *ele = NULL;
    axiom_attribute_t *attr = NULL;
    axiom_namespace_t *sp_ns = NULL;

    sp_ns = axiom_namespace_create(env, WS_SEC_POLICY_NAMESPACE_URI, WS_SEC_POLICY_NAMESPACE);
    axiom_element_create(env, parent_node, WS_SIGNSUPPORTING_TOKEN, sp_ns, &signsupport_om_node);
    policy_om_node = create_policy_node(env, signsupport_om_node);
    ele = axiom_element_create(env, policy_om_node, WS_USERNAMETOKEN, sp_ns, &ut_om_node);
    attr = axiom_attribute_create(env,  WS_INCLUDE_TOKEN ,  WS_INCLUDE_TOKEN_URI,
                                  sp_ns);
    axiom_element_add_attribute(ele, env, attr, ut_om_node);

    return ut_om_node;

}

axis2_status_t AXIS2_CALL
echo_callback_on_complete(struct axis2_callback* callback,
						  const axutil_env_t* 	 env)
{
	isComplete = 1;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
echo_callback_on_error (struct axis2_callback* callback,
						const axutil_env_t*    env,
						int 				   exception)
{
    isComplete = 1;
	return AXIS2_SUCCESS;
}

