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

/*
 * Standard includes
 */
#include "constants.h"
#include "error.h"
#include "stub.h"
#include "option.h"
#include <axiom.h>

#include <axis2_http_transport.h>

/*
 * minINI include
 * For INI file parameters management
 */
#include "minIni.h"

/*
 * RT Server required includes
 */
#include <xdc.h>
#include <xdy.h>
#include <xzst.h>
#include <xzsc.h>
#include <xzamc.h>

/*
 * Defines
 */
#define BUFF_SIZE 1024

/*
 * minIni defines
 */
#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

/*
 * Prototypes
 */
static void
print_help();

static void
print_version();

static void tewsr_sortir();

static void tewsr_autorad_process();

/*
 * Global variables
 */
axutil_env_t *env = NULL;
axutil_array_list_t *array_list;

/*
 * Ini params
 */
const char inifile[] = "/produits/migrazur/appliSD/fichiers/deq/tewsr.ini";
char log_level_string[100];
char log_path_string[250];
char ws_rad_baseuri[250];
char nomMachineLocale[250];
int numeroEqt;
long n;

char myIP[16] = "10.16.12.154";
char myPort[6] = "9090";
char myServiceUri[100] = "/axis2/services/echoSAE";

int rt_error = 0;

/*
 * main
 */
int main (int argc, char **argv)
{
	/*
	int ii = 0;
	axis2_char_t *name = NULL;
	*/
	axis2_char_t *log_path = "/produits/migrazur/appliSD/traces/tewsr.log";
	axutil_log_levels_t log_level = AXIS2_LOG_LEVEL_INFO;

	/*
	 * Migrazur Framework vars
	 */
	int vl_cr = XDC_OK;
	XDY_Datagroup vl_dg;
	T_IPC_MSG pl_Msg;
	int vl_IndPre = 0;

	char numeroEqtString[20];

	/*
	 * Args parsing if needed
	 */
	if (argc > 1) {
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
			print_help();
		else if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
			print_version();
	}

	/*
	 * TODO : A voir... paramétrage du log façon AXIS2 ? Ou façon Migrazur ?
	 */
	env = axutil_env_create_all(log_path, log_level);

	/*
	 * NOTICE : Simplifier (ne sert qu'à initialiser array_list et récupérer l'option --soap)
	 */
	array_list = tewsr_process_options(argc, argv, env);

	/*
	 * Log  path
	 */
	n = ini_gets("tewsr", "log-path", ".", log_path_string,
			sizearray(log_path_string), inifile);

	/*
	 * My Service Url Params
	 */
	n = ini_gets("tewsr", "my-ip", "10.16.12.154", myIP,
			sizearray(myIP), inifile);

	n = ini_gets("tewsr", "my-port", "9090", myPort,
			sizearray(myPort), inifile);

	n = ini_gets("tewsr", "my-service-uri", "/axis2/services/echoSAE", myServiceUri,
			sizearray(myServiceUri), inifile);


	/*
	 * DONE : vérifier s'il manque pas un free sur log_path
	 */
	log_path = malloc(sizeof(axis2_char_t) * (strlen(log_path_string) + 1));
	strncpy(log_path, log_path_string, strlen(log_path_string));
	log_path[sizeof(log_path) - 1] = '\0';

	/*
	 * Log  level
	 */
	n = ini_gets("tewsr", "log-level", "info", log_level_string,
			sizearray(log_level_string), inifile);

	printf("Log level : %s \n", log_level_string);

	if (!axutil_strcmp(log_level_string, "user")) {
		log_level = AXIS2_LOG_LEVEL_USER;
	} else if (!axutil_strcmp(log_level_string, "debug")) {
		log_level = AXIS2_LOG_LEVEL_DEBUG;
	} else if (!axutil_strcmp(log_level_string, "info")) {
		log_level = AXIS2_LOG_LEVEL_INFO;
	} else if (!axutil_strcmp(log_level_string, "warning")) {
		log_level = AXIS2_LOG_LEVEL_WARNING;
	} else if (!axutil_strcmp(log_level_string, "error")) {
		log_level = AXIS2_LOG_LEVEL_ERROR;
	} else if (!axutil_strcmp(log_level_string, "critical")) {
		log_level = AXIS2_LOG_LEVEL_CRITICAL;
	}

	/*
	 * WS Tel URI
	 */
	n = ini_gets("tewsr", "ws-rad-base-uri",
			"http://10.16.12.10:20051/soap", ws_rad_baseuri,
			sizearray(ws_rad_baseuri), inifile);

	printf("WS Tel Base URI : %s \n", ws_rad_baseuri);
	
	/*
	 * Gestion Alarmes
	 */
	n = ini_gets("tewsr", "machine-locale",
			"wsGatewayDefault", nomMachineLocale,
			sizearray(nomMachineLocale), inifile);

	n = ini_gets("tewsr", "numero-eqt",
			"wsGatewayDefault", numeroEqtString,
			sizearray(numeroEqtString), inifile);

	sscanf(numeroEqtString, "%d",&numeroEqt);

	/*
	 * DONE : Abonnement Cnx au WS Radio
	 */

	/*
	 * Init connection RT
	 */

	/*
	 * TODO : Régler le pb de la sortie standard avec JMG ( 03/09/2010 )
	 *
	 */
	printf("XZSC_01InitCnxSup Application = %s Task = %s", "migrazur",
			"tewsr.x");

	vl_cr = XZSC_01InitCnxSup(argv[1], argv[0], XZSCC_INTERDIRE /* mise a jour horodate*/
	, XZSCC_AUTORISER /* systeme trace */
	, XZSCC_INTERDIRE /* synchro tsetat */
	, tewsr_sortir /* fontion de sortie */
	, NULL /* redirection sortie standard *//* TODO : JMG a remplacé "/dev/null" par NULL pour que ça marche */
	);

	if (vl_cr != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur XZSC_01InitCnxSup cr = %d",
				vl_cr);
		exit(0);
	}

	strcpy(vl_dg, XDG_ECMD_AUTORAD);

	if (XDG_JoindreDG(vl_dg) != T_TRUE)
		XZST_03EcritureTrace(XZSTC_FATAL,
				"probleme lors de l'abonnement au datagroup %s", vl_dg);

	/*
	 * Armement du Callback pour XDM_ECMD_AUTORAD
	 */
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ECMD_AUTORAD),
			(T_IPC_CONN_PROCESS_CB_FUNC) tewsr_autorad_process,
			(T_CB_ARG) NULL) == NULL)
		XZST_03EcritureTrace(XZSTC_FATAL,
				"probleme lors de la creation du callback de reception de XDM_ECMD_AUTORAD");

	/*
	 AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,"avant while");
	 */
	/*
	 * printf("[DEBUG] avant while\n");
	 */

	while (1) {
		/*
		 AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,"apres while");
		 */

		/*
		 * printf("[DEBUG] dans while\n");
		 */

		pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

		/*
		 * printf("[RT] message reçu\n");
		 */

		if (pl_Msg != NULL) {

			/*
			 AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,"[RT] arrivee message");
			 */
			/*
			 * printf("[RT] traitement message\n");
			 */

			TipcSrvMsgProcess(pl_Msg);
			TipcMsgDestroy(pl_Msg);

			/*
			 * printf("[RT] message traité\n");
			 */

			/* Memorisation Test Presence RTserver */
			vl_IndPre = 0;

		} else {
			/* Test de Presence RTserver */
			if (vl_IndPre == 0) {
				XZST_03EcritureTrace(XZSTC_WARNING,
						"Erreur RTWorks num: %d -- errno %d \n",
						TutErrNumGet(), errno);
			} else if (vl_IndPre >= XDC_EXIT_RTCLIENT) {
				XZST_03EcritureTrace(XZSTC_WARNING,
						"Bouclage et arret tache tewsr.x %d \n", vl_IndPre);

				/* Arret de la tache */
				tewsr_sortir();
			}

			/* Indication absence RTserver */
			vl_IndPre++;
		}
	}

	free(log_path);
	axutil_env_free((void *) env);
}

/*
 * Functions
 */
static void print_version() {
	static const char
			*version[] =
					{
							("\
\n"),
			("\
             tewsr 1.0     \n"),
			("\
\n"),
			("\
  Licensed to the Apache Software Foundation (ASF) under one or more\n"),
			("\
 contributor license agreements.  See the NOTICE file distributed with\n"),
			("\
 this work for additional information regarding copyright ownership.\n"),
			("\
 The ASF licenses this file to You under the Apache License, Version 2.0\n"),
			("\
 (the \"License\"); you may not use this file except in compliance with\n"),
			("\
 the License.  You may obtain a copy of the License at\n"),
			("\
\n"),
			("\
      http://www.apache.org/licenses/LICENSE-2.0\n"),
			("\
\n"),
			("\
 Unless required by applicable law or agreed to in writing, software\n"),
			("\
 distributed under the License is distributed on an \"AS IS\" BASIS,\n"),
			("\
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"),
			("\
 See the License for the specific language governing permissions and\n"),
			("\
 limitations under the License.\n"),
			("\
\n"),
		};
	int i = 0;
	for (i = 0; i < array_sizeof(version); i++)
		fputs(version[i], stdout);
	exit(0);
}

static void print_help() {
	static const char
			*help[] =
					{
							("\
tewsr 1.0, command line tool to consume SEE Radio webservices.\n"),
							("\
Usage: tewsr \n\n") };

	int i = 0;
	for (i = 0; i < array_sizeof(help); i++)
		fputs(help[i], stdout);
	exit(0);
}

void tewsr_sortir() {
	exit(0);
}

void tewsr_autorad_process(va_Cnx, pa_Data, pa_Arg)
	T_IPC_CONN va_Cnx;T_IPC_CONN_PROCESS_CB_DATA pa_Data;T_CB_ARG pa_Arg; {
	char input[1000];
	char dest_uri[300];
	axis2_status_t status;

	T_INT4 va_typeAppel;
	T_STR va_numeroGroupeRadio;
	T_STR va_numeroCibleAppel;
	T_INT2 va_NumPO;
	T_STR va_nomSite;

	char str_typeAppel[5];

	XDY_Datagroup pl_DG;
	XDY_Datagroup pl_DG_Error;
	double vl_horodate;
	
	XDG_EncodeDG2 ( pl_DG, XDG_EETAT_AUTOTEL, va_nomSite);
	XDG_EncodeDG2 ( pl_DG_Error, XDG_ETAT_LIAISON, nomMachineLocale);
	
	/*
	 * Init connection
	 */
	sprintf(
			input,
			"<soapenv:Envelope \
        xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/frameworkSchema\" \
        xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
        xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
        xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \
        <soapenv:Header> \
</soapenv:Header> \
<soapenv:Body> \
<NS1:Connection xmlns:NS1=\"urn:WS_CommandSEEIntf-IWS_CommandSEE\"> \
<IpAddress xsi:type=\"xsd:string\">%s</IpAddress> \
<IpPort xsi:type=\"xsd:string\">%s</IpPort> \
<URL xsi:type=\"xsd:string\">%s</URL> \
<MainClient xsi:type=\"xsd:boolean\">%s</MainClient> \
<ConnectNb xsi:type=\"xsd:int\">%d</ConnectN> \
</NS1:Connection> \
</soapenv:Body> \
</soapenv:Envelope>",myIP,myPort,myServiceUri,"true",0);

	/*
	 *  printf("La chaine : '%s'\n\n", *input);
	 */

	strcpy(dest_uri, ws_rad_baseuri);
	strcat(dest_uri, (char *) "/IWS_CommandSEE");

	/*
	 * Envoi requete WS
	 */

	/*
	 AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
	 "[tewsr]sending request to  %s", dest_uri);
	 */

	tewsr_options_t *options;

	/* DONE : Ajout de SOAPAction loginRequest */
	options = AXIS2_MALLOC(env->allocator, sizeof(tewsr_options_t));
	options->opt_name = AXIS2_MALLOC(env->allocator, sizeof(char) * 12);
	strcpy(options->opt_name, "http-header");
	options->short_name = 0;
	options->value = AXIS2_MALLOC(env->allocator, sizeof(char) * 64);
	strcpy(options->value, "SOAPAction: \"urn:WS_CommandSEEIntf-IWS_CommandSEE#Connection\"");
	axutil_array_list_add(array_list, env, (void *) options);

	status = tewsr_stub_invoke(env, array_list, dest_uri, input);

	switch (status) {
	case TEWSR_FAILURE: {
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewsr] tewsr connection failed!");
		printf("\ntewsr connection failed ! \n");


		if (!TipcSrvMsgWrite(pl_DG,
				XDM_IdentMsg(XDM_ETAT_AUTOTEL),	XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_NumPO,
				T_IPC_FT_INT4, XDC_APPEL,  		/* NoCdeCourante */
				T_IPC_FT_STR, "",    	/* NoTelAppele */
				T_IPC_FT_STR, "",       /* EtatRepondeur */
				T_IPC_FT_INT4, XDC_ERROR,/* CptRenduCde */
				T_IPC_FT_INT4, XDC_NOK,  /* EtatLiaison */
				NULL))
		{
			printf("[RT] Erreur msg XDM_ETAT_AUTOTEL(RAD) non envoye\n");
		}
		else
		{
			printf("[RT] Erreur msg XDM_ETAT_AUTOTEL(RAD) envoye\n");
		}

		if ( rt_error == 0 )
		{
			rt_error=1;

			XDG_EncodeDG2(pl_DG_Error,XDG_AM,XDC_NOM_SITE_CI);
			vl_horodate=TutGetWallTime();
			if (!TipcSrvMsgWrite ( pl_DG_Error,
					XDM_IdentMsg(XDM_AM_alarme), XDM_FLG_SRVMSGWRITE,
					T_IPC_FT_REAL8, vl_horodate,
					T_IPC_FT_CHAR, XDC_EQT_MAA,
					T_IPC_FT_INT2, numeroEqt,         /* recupere dans fichier ini*/
					T_IPC_FT_INT2, XZAMC_DEF_TRANS,
					T_IPC_FT_CHAR, XDC_VRAI,		   /* XDC_FAUX sur comm OK*/ /*XDC_VRAI sur comm HS -> login failed*/
					T_IPC_FT_STR, nomMachineLocale,
					NULL))
			{
				printf("[RT] Erreur Alarme(RAD) non envoyee\n");
			}
			else
			{
				printf("[RT] Erreur Alarme(RAD) envoyee\n");
			}
		}

	}
		break;
	case TEWSR_ERROR_NO_DESTURI: {
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewsr] tewsr no dest uri !");
		printf("\n[tewsr] tewsr no dest uri ! \n");
	}
		break;
	case TEWSR_SUCCESS: {
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
				"[tewsr] tewsr connection Success !");

		rt_error=0;

		XDG_EncodeDG2(pl_DG_Error,XDG_AM,XDC_NOM_SITE_CI);
		vl_horodate=TutGetWallTime();
		if (!TipcSrvMsgWrite ( pl_DG_Error,
				XDM_IdentMsg(XDM_AM_alarme), XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_REAL8, vl_horodate,
				T_IPC_FT_CHAR, XDC_EQT_MAA,
				T_IPC_FT_INT2, numeroEqt,         /* recupere dans fichier ini*/
				T_IPC_FT_INT2, XZAMC_DEF_TRANS,
				T_IPC_FT_CHAR, XDC_FAUX,		   /* XDC_FAUX sur comm OK*/ /*XDC_VRAI sur comm HS -> login failed*/
				T_IPC_FT_STR, nomMachineLocale,
				NULL))
		{
			printf("[RT] RAZ Alarme(RAD) non envoyee\n");
		}
		else
		{
			printf("[RT] RAZ Alarme(RAD) envoyee\n");
		}

	}
		break;
	};


	/* XDY_Datagroup pl_DG; */

	/* int finAppel; */

	/*
	 * DONE : Traiter les messages RADIO
	 * {"XDM_ECMD_AUTORAD", 		XDM_ECMD_AUTORAD, 	"str str str int2 str" }
	 *
	 * TipcSrvMsgWrite (  XDG_ECMD_AUTORAD
	 * ,XDM_IdentMsg(XDM_ECMD_AUTORAD)
	 * ,XDM_FLG_SRVMSGWRITE
	 * ,T_IPC_FT_INT4,	va_typeAppel
	 * ,T_IPC_FT_STR,	va_numeroGroupeRadio
	 * ,T_IPC_FT_STR,	va_numeroCibleAppel
	 * ,T_IPC_FT_INT2,	va_NumPO
	 * ,T_IPC_FT_STR,	va_nomSite
	 * ,NULL)
	 *
	 */

	if (TipcMsgRead(pa_Data->msg,
			 T_IPC_FT_INT4,	&va_typeAppel,
			 T_IPC_FT_STR,	&va_numeroGroupeRadio,
			 T_IPC_FT_STR,	&va_numeroCibleAppel,
			 T_IPC_FT_INT2,	&va_NumPO,
			 T_IPC_FT_STR,	&va_nomSite,
			NULL) != TRUE) {
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur dans TipcMsgRead");
		return;
	}

	switch (va_typeAppel)
	{
		case 3:
			strcpy(str_typeAppel,"GRP");
			break;
		case 2:
			strcpy(str_typeAppel, "IND");
			break;
		case 1:
			strcpy(str_typeAppel, "AIG");
			break;
		default:
			strcpy(str_typeAppel, "AIG");
			break;
	}

	/* NOTICE : Taille dynamique malloc required ? */
	sprintf(
			input,
			"<soapenv:Envelope \
        xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/frameworkSchema\" \
        xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
        xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
        xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \
        <soapenv:Header> \
</soapenv:Header> \
<soapenv:Body> \
<NS1:CommandSEE xmlns:NS1=\"urn:WS_CommandSEEIntf-IWS_CommandSEE\"> \
<CmdType xsi:type=\"xsd:string\">SAE_APPEL</CmdType> \
<CmdData xsi:type=\"xsd:string\">&lt;Appel&gt;&lt;TypeAppel&gt;%s&lt;/TypeAppel&gt;&lt;GSSI&gt;%s&lt;/GSSI&gt;&lt;ISSI&gt;%s&lt;/ISSI&gt;&lt;NumActionSAE&gt;01234567890&lt;/NumActionSAE&gt;&lt;NumSiteSAE&gt;%s&lt;/NumSiteSAE&gt;&lt;NumPosteSAE&gt;%d&lt;/NumPosteSAE&gt;&lt;/Appel&gt;</CmdData> \
</NS1:CommandSEE> \
</soapenv:Body> \
</soapenv:Envelope>",str_typeAppel,va_numeroGroupeRadio,va_numeroCibleAppel,va_nomSite, va_NumPO);

	/*
	 * printf("La chaine : '%s'\n\n", *input);
	 */

	strcpy(dest_uri, ws_rad_baseuri);
	strcat(dest_uri, (char *) "/IWS_CommandSEE");

	/*
	 * Envoi requete WS
	 */

	/*
	 AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
	 "[tewsr]sending request to  %s", dest_uri);
	 */

	/* DONE : Ajout de SOAPAction loginRequest */
	options = AXIS2_MALLOC(env->allocator, sizeof(tewsr_options_t));
	options->opt_name = AXIS2_MALLOC(env->allocator, sizeof(char) * 12);
	strcpy(options->opt_name, "http-header");
	options->short_name = 0;
	options->value = AXIS2_MALLOC(env->allocator, sizeof(char) * 64);
	strcpy(options->value, "SOAPAction: \"urn:WS_CommandSEEIntf-IWS_CommandSEE#Connection\"");
	axutil_array_list_add(array_list, env, (void *) options);

	status = tewsr_stub_invoke(env, array_list, dest_uri, input);

	switch (status) {
	case TEWSR_FAILURE: {
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewsr] tewsr invoke failed!");
		printf("\ntewsr invoke failed ! \n");
	}
		break;
	case TEWSR_ERROR_NO_DESTURI: {
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[tewsr] tewsr no dest uri !");
		printf("\n[tewsr]tewsr no dest uri ! \n");
	}
		break;
	case TEWSR_SUCCESS: {
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
				"[tewsr] tewsr login Success !");
	}
		break;
	};

}

