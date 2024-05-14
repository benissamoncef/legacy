/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_TEWST_STUB_H
#define AXIS2_TEWST_STUB_H

#include <platforms/axutil_platform_auto_sense.h>
#include <axutil_utils.h>
#include <axutil_utils_defines.h>
#include <axutil_env.h>
#include <axutil_array_list.h>
#include <axutil_hash.h>
#include <axiom_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * RT Server required includes
 */
#include <xdc.h>
#include <xdy.h>
#include <xzst.h>
#include <xzsc.h>

/* elements de Retour du WS Alcatel */
char AlcSessionId[100];
T_INT4 AlcCallPresent;
T_INT4 AlcCallState;
char AlcCurrentCallRef[500];

axis2_status_t AXIS2_CALL
tewst_stub_invoke(
    const axutil_env_t *env,
    axutil_array_list_t *all_opt_list,
    axis2_char_t *dest_uri,
    axis2_char_t *input,
    int fire_and_forget,
    int actionType);

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_TEWST_STUB_H */
