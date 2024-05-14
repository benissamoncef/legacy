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

static void tewst_sortir();

static void tewst_autotel2_process();

/*
 * Global variables
 */
axutil_env_t *env = NULL;
axutil_env_t *env2 = NULL;
axutil_array_list_t *array_list;

axis2_char_t *log_path = "tewst.log";
axutil_log_levels_t log_level = AXIS2_LOG_LEVEL_INFO;

/*
 * Ini params
 */
const char inifile[] = "/produits/migrazur/appliSD/fichiers/deq/tewst.ini";
char log_level_string[100];
char log_path_string[250];
char ws_tel_baseuri[250];
char nomMachineLocale[250];
int numeroEqt;
long n;

int rt_error = 0;

/*
 * main
 */
int main (int argc, char **argv)
{
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
	if (argc > 1)
	{
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
			print_help();
		else if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
			print_version();

	}

	/*
	 * DONE : vérifier s'il manque pas un free sur log_path
	 */

	/*
	 * Log  level
	 */
	n = ini_gets("tewst", "log-level", "info", log_level_string,
			sizearray(log_level_string), inifile);

	printf("Log level : %s \n", log_level_string);

	/*
	 * Log  path
	 */
	n = ini_gets("tewst", "log-path", ".", log_path_string,
			sizearray(log_path_string), inifile);

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

	printf("Log path string : %s \n", log_path_string);

	log_path = log_path_string;

	printf("Log path : %s \n", log_path);

	/*
	 * TODO : A voir... paramétrage du log façon AXIS2 ? Ou façon Migrazur ?
	 */
	env = axutil_env_create_all(log_path, log_level);

	/*
	 * NOTICE : Simplifier (ne sert qu'à initialiser array_list et récupérer l'option --soap)
	 */
	array_list = tewst_process_options(argc, argv, env);

	/*
	 * WS Tel URI
	 */
	n = ini_gets("tewst", "ws-tel-base-url",
			"http://localhost:9090/axis2/services/echo", ws_tel_baseuri,
			sizearray(ws_tel_baseuri), inifile);

	/*
	 * Gestion Alarmes
	 */
	n = ini_gets("tewst", "machine-locale",
			"wsGatewayDefault", nomMachineLocale,
			sizearray(nomMachineLocale), inifile);

	n = ini_gets("tewst", "numero-eqt",
			"wsGatewayDefault", numeroEqtString,
			sizearray(numeroEqtString), inifile);

	sscanf(numeroEqtString, "%d",&numeroEqt);

	printf("WS Tel Base URI : %s \n", ws_tel_baseuri);

	/*
	 * Init connection RT
	 */

	/*
	 * TODO : Régler le pb de la sortie standard avec JMG ( 03/09/2010 )
	 *
	 */
	printf("XZSC_01InitCnxSup Application = %s Task = %s", "migrazur",
			"tewst.x");

	vl_cr = XZSC_01InitCnxSup(argv[1], argv[0], XZSCC_INTERDIRE /* mise a jour horodate*/
	, XZSCC_AUTORISER /* systeme trace */
	, XZSCC_INTERDIRE /* synchro tsetat */
	, tewst_sortir /* fontion de sortie */
	, NULL /* redirection sortie standard *//* TODO : JMG a remplacé "/dev/null" par NULL pour que ça marche */
	);

	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE|XZSTC_DEBUG1,XZSTC_NECRAN);

	if (vl_cr != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur XZSC_01InitCnxSup cr = %d",
				vl_cr);
		exit(0);
	}

	strcpy(vl_dg, XDG_ECMD_AUTOTEL);

	if (XDG_JoindreDG(vl_dg) != T_TRUE)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,
				"probleme lors de l'abonnement au datagroup %s", vl_dg);
		exit (1);
	}

	/*
	 * Armement du Callback pour XDM_ECMD_AUTOTEL2
	 */
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ECMD_AUTOTEL2),
			(T_IPC_CONN_PROCESS_CB_FUNC) tewst_autotel2_process,
			(T_CB_ARG) NULL) == NULL)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,
				"probleme lors de la creation du callback de reception de XDM_ECMD_AUTOTEL2");
		exit (1);
	}

	XZST_03EcritureTrace(XZSTC_WARNING, "toto");
	XZST_03EcritureTrace(XZSTC_INFO, "[tewst] Before while...");

	/*
	 * printf("[INFO] avant while\n");
	 */

	while (1)
	{
		XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] while waiting for message...");

		/*
		 * printf("[INFO] dans while\n");
		 */
		printf("[INFO] while waiting for RT message\n");

		pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

		XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] Msg message received");
		/*
		 * printf("[RT] message reçu\n");
		 */

		if (pl_Msg != NULL)
		{

			XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] pl_Msg message not NULL: processing");

			/*
			 * printf("[RT] traitement message\n");
			 */

			TipcSrvMsgProcess(pl_Msg);
			TipcMsgDestroy(pl_Msg);

			/* Memorisation Test Presence RTserver */
			vl_IndPre = 0;

		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] pl_Msg message NULL");

			/* Test de Presence RTserver */
			if (vl_IndPre == 0)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] WARNING RTWorks Error");

				XZST_03EcritureTrace(XZSTC_WARNING,
						"Erreur RTWorks num: %d -- errno %d \n",
						TutErrNumGet(), errno);
			} else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] WARNING stopping tewst.x task");

				XZST_03EcritureTrace(XZSTC_WARNING,
						"Bouclage et arret tache tewst.x %d \n", vl_IndPre);

				/* Arret de la tache */
				tewst_sortir();
			}

			/* Indication absence RTserver */
			vl_IndPre++;
		}
	}

	axutil_env_free((void *) env);
	free(log_path);
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
             tewst 1.0     \n"),
							("\
\n"),
							("\
  Licensed to Steria under one or more\n"),
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
 limitations under the License.\n"), ("\
\n"), };
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
tewst 1.0, command line tool to consume webservices.\n"),
							("\
Usage: tewst.x migrazur \n\n") };

	int i = 0;
	for (i = 0; i < array_sizeof(help); i++)
		fputs(help[i], stdout);
	exit(0);
}

void tewst_sortir()
{
	exit(0);
}

void tewst_autotel2_process(va_Cnx, pa_Data, pa_Arg)
	T_IPC_CONN va_Cnx;T_IPC_CONN_PROCESS_CB_DATA pa_Data;T_CB_ARG pa_Arg;
{
	char input[2000];
	char dest_uri[300];
	axis2_status_t status;

	char LoginIni[50];
	char LoginPassword[50];
	char AlcLogin[50];
	char AlcPassword[50];

	char phone_timeout_string[15];
	int phone_timeout;

	T_INT4 va_codecmd;
	T_STR va_NumTel;
	T_INT2 va_NumPO;
	T_STR va_nomSite;

	XDY_Datagroup pl_DG;
	XDY_Datagroup pl_DG_Error;
	double vl_horodate;

	axutil_array_list_t *array_list2;
	int fake_argc=1;
	char ** fake_argv=NULL;

	fake_argv=malloc(sizeof(char*));
	fake_argv[0]=malloc(sizeof(char));
	strcpy(fake_argv[0], "\0");

	/*
	 * TODO : A voir... paramétrage du log façon AXIS2 ? Ou façon Migrazur ?
	 */
	env2 = axutil_env_create_all(log_path, log_level);

	array_list2 = tewst_process_options(fake_argc, fake_argv, env2);

	/*
	 * TipcSrvMsgWrite (  XDG_ECMD_AUTOTEL
	 ,XDM_IdentMsg(XDM_ECMD_AUTOTEL2)
	 ,XDM_FLG_SRVMSGWRITE
	 ,T_IPC_FT_INT4,va_codecmd
	 ,T_IPC_FT_STR, va_NumTel
	 ,T_IPC_FT_INT2,va_NumPO
	 ,T_IPC_FT_STR,va_nomSite
	 ,NULL)
	 */

	if (TipcMsgRead(pa_Data->msg, T_IPC_FT_INT4, &va_codecmd, T_IPC_FT_STR,
			&va_NumTel, T_IPC_FT_INT2, &va_NumPO, T_IPC_FT_STR, &va_nomSite,
			NULL) != TRUE) {
		XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] WARNING error in TipcMsgRead");

		XZST_03EcritureTrace(XZSTC_WARNING, "erreur dans TipcMsgRead");
		return;
	}

	/*
	 * Phone timeout
	 */
	n = ini_gets("tewst", "phone-timeout", "15", phone_timeout_string,
			sizearray(phone_timeout_string), inifile);

	sscanf(phone_timeout_string, "%d",&phone_timeout);

	/*
	 * printf("Phone timeout : %d \n", phone_timeout);
	 */

	/*
	 * Accounts
	 */
	sprintf(LoginIni, "login-%d", va_NumPO);
	sprintf(LoginPassword, "password-%d", va_NumPO);
	ini_gets("phones", LoginIni, "CI1", AlcLogin, sizearray(AlcLogin), inifile);
	ini_gets("phones", LoginPassword, "0000", AlcPassword,
			sizearray(AlcPassword), inifile);

	XDG_EncodeDG2 ( pl_DG, XDG_EETAT_AUTOTEL, va_nomSite);
	XDG_EncodeDG2 ( pl_DG_Error, XDG_ETAT_LIAISON, nomMachineLocale);

	/*
	 * DONE : Gérer les cas en fonction du type de commande
	 * va_cmd = XDC_APPEL (1)
	 * va_cmd = XDC_RENVOI (2)
	 * va_cmd = XDC_ANNUL_RENVOI (3)
	 */

	tewst_options_t *options;

	switch (va_codecmd)
	{
		case XDC_APPEL:
			XZST_03EcritureTrace(XZSTC_INFO, "[tewst] Case command XDC_APPEL");
			/*
			 * printf("Case command XDC_APPEL \n");
			 */

			/* TODO : Taille dynamique malloc required ? */
			sprintf(
					input,
					"<soapenv:Envelope \
xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/frameworkSchema\" \
xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\
<soapenv:Header>\
</soapenv:Header>\
<soapenv:Body>\
<q0:loginRequest>\
<q0:loginName>%s</q0:loginName>\
<q0:password>%s</q0:password>\
</q0:loginRequest>\
</soapenv:Body>\
</soapenv:Envelope>",
					AlcLogin, AlcPassword);

			/* printf("La chaine : '%s'\n\n", *input); */

			strcpy(dest_uri, ws_tel_baseuri);
			strcat(dest_uri, (char *) "/AlcApiFrameworkPort");

			/*
			 * Envoi requete WS
			 */

			/* XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst]sending request to  %s", dest_uri); */

			/* DONE : Ajout de SOAPAction loginRequest */
			options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
			options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 12);
			strcpy(options->opt_name, "http-header");
			options->short_name = 0;
			options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 25);
			strcpy(options->value, "SOAPAction: loginRequest");
			axutil_array_list_add(array_list2, env2, (void *) options);

			status = tewst_stub_invoke(env2, array_list2, dest_uri, input, 0);

			/* Resultats :
			 * resultCode : SUCCESS
			 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
			 */

			switch (status) {
			case TEWST_FAILURE: {
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst login failed!");

				printf("\ntewst login failed ! \n");

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
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] ERROR XDM_ETAT_AUTOTEL(TEL) NOT SENT");

					printf("[RT] Erreur msg XDM_ETAT_AUTOTEL(TEL) non envoye\n");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] XDM_ETAT_AUTOTEL(TEL) SENT");

					printf("[RT] Erreur msg XDM_ETAT_AUTOTEL(TEL) envoye\n");
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
						XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] ERROR ALARM(TEL) NOT SENT");
						printf("[RT] Erreur Alarme(TEL) non envoyee\n");
					}
					else
					{
						XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] ALARM(TEL) SENT");

						printf("[RT] Erreur Alarme(TEL) envoyee\n");
					}
				}

			}
				break;
			case TEWST_ERROR_NO_DESTURI: {
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst login failed !");

				printf("\ntewst login failed ! \n");
			}
				break;
			case TEWST_SUCCESS: {
				XZST_03EcritureTrace(XZSTC_DEBUG1,
						"[tewst] tewst login Success !");

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
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] ERROR RESET ALARM(TEL) NOT SENT");
					printf("[RT] RAZ Alarme(TEL) non envoyee\n");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] RESET ALARM(TEL) SENT");

					printf("[RT] RAZ Alarme(TEL) envoyee\n");
				}

			}
				break;
			};

			if ( status == TEWST_SUCCESS )
			{

				/*
				 * printf("Nom site : %s No de Cde : %d\n", va_nomSite, (int)va_codecmd);
				 */

				/*
				 * Cas de MakeCall ( No de Cde : 1 )
				 */

				sprintf(
						input,
						"<soapenv:Envelope \
xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/phoneSchema\" \
xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \
<soapenv:Header>\
</soapenv:Header>\
<soapenv:Body>\
<q0:makeCallRequest>\
<q0:fwSessionId>%s</q0:fwSessionId>\
<q0:lineNumber/>\
<q0:callee>%s</q0:callee>\
</q0:makeCallRequest>\
</soapenv:Body>\
</soapenv:Envelope>",
						AlcSessionId, va_NumTel);

				strcpy(dest_uri, ws_tel_baseuri);
				strcat(dest_uri, (char *) "/AlcPhone");

				/* DONE : Ajout de SOAPAction makeCallRequest */
				options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
				options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 12);
				strcpy(options->opt_name, "http-header");
				options->short_name = 0;
				options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 28);
				strcpy(options->value, "SOAPAction: makeCallRequest");
				axutil_array_list_add(array_list2, env2, (void *) options);

				status = tewst_stub_invoke(env2, array_list2, dest_uri, input, 0);

				/* Resultats :
				 * resultCode : SUCCESS
				 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
				 */

				switch (status) {
				case TEWST_FAILURE: {
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst makecall failed!");

					printf("\ntewst makecall failed ! \n");
				}
					break;
				case TEWST_ERROR_NO_DESTURI: {
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst makecall failed !");

					printf("\ntewst makecall failed ! \n");
				}
					break;
				case TEWST_SUCCESS: {
					XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst] makecall Success !");
				}
					break;
				};

				/* DONE : Move to echoSAE phoneStatusWatch */
				if ( status == TEWST_SUCCESS )
				{
					/*
					 * Envoi d'un message au web service avec comme infos :
					 *
					 * AlcSessionId
					 * va_nomSite
					 * va_NumPO
					 * va_codecmd
					 * va_NumTel
					 *
					 */

					sprintf(
							input,
							"<soapenv:Envelope \
xmlns:ns1=\"http://www.steria.com/wsp/ns/2010/10/26/escota/phoneSchema\" \
xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\
<soapenv:Header>\
</soapenv:Header>\
<soapenv:Body>\
<ns1:phoneStatusWatch>\
<ns1:wsTelUri>%s%s</ns1:wsTelUri>\
<ns1:fwSessionId>%s</ns1:fwSessionId>\
<ns1:va_nomSite>%s</ns1:va_nomSite>\
<ns1:va_NumPO>%d</ns1:va_NumPO>\
<ns1:va_codecmd>%d</ns1:va_codecmd>\
<ns1:va_NumTel>%s</ns1:va_NumTel>\
</ns1:phoneStatusWatch>\
</soapenv:Body>\
</soapenv:Envelope>",
				ws_tel_baseuri, (char *) "/AlcPhone", AlcSessionId, va_nomSite, (int)va_NumPO, (int)va_codecmd, va_NumTel);

					/* printf("La chaine : '%s'\n\n", input); */

					strcpy(dest_uri, "http://localhost:9090/axis2/services/echoSAE");

					/*
					 * Envoi requete WS
					 */

					 /* XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst]sending request to  %s", dest_uri); */

					/* DONE : Ajout de SOAPAction loginRequest */
					options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
					options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 12);
					strcpy(options->opt_name, "http-header");
					options->short_name = 0;
					options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 29);
					strcpy(options->value, "SOAPAction: phoneStatusWatch");
					axutil_array_list_add(array_list2, env2, (void *) options);

					options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
					options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 9);
					strcpy(options->opt_name, "reply-to");
					options->short_name = 0;
					options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 41);
					strcpy(options->value, "http://localhost:9090/axis2/services/null");
					axutil_array_list_add(array_list2, env2, (void *) options);

					status = tewst_stub_invoke(env2, array_list2, dest_uri, input, 1);

					/* Resultats :
					 * resultCode : SUCCESS
					 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
					 */

					switch (status) {
					case TEWST_FAILURE: {
						XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] ERROR phoneStatusWatch failed! (FAILURE)");

						printf("\ntewst phoneStatusWatch failed ! \n");
					}
						break;
					case TEWST_ERROR_NO_DESTURI: {
						XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] ERROR phoneStatusWatch failed ! (NO_DESTURI)");

						printf("\ntewst phoneStatusWatch failed ! \n");
					}
						break;
					case TEWST_SUCCESS: {
						XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst] phoneStatusWatch Success !");

						printf("\ntewst phoneStatusWatch success ! \n");
					}
						break;
					};

				}
			}
			/* end case XDC_APPEL */
			break;

		case XDC_RENVOI:
			XZST_03EcritureTrace(XZSTC_INFO, "[tewst] Case command XDC_RENVOI");
			/*
			 * printf("Case command XDC_RENVOI \n");
			 */

			/* TODO : Taille dynamique malloc required ? */
			sprintf(
					input,
					"<soapenv:Envelope \
xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/frameworkSchema\" \
xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \
<soapenv:Header>\
</soapenv:Header>\
<soapenv:Body>\
<q0:loginRequest>\
<q0:loginName>%s</q0:loginName>\
<q0:password>%s</q0:password>\
</q0:loginRequest>\
</soapenv:Body>\
</soapenv:Envelope>",
					AlcLogin, AlcPassword);

			/* dest_uri = tewst_destination (argc, argv); */
			strcpy(dest_uri, ws_tel_baseuri);
			strcat(dest_uri, (char *) "/AlcApiFrameworkPort");

			/*
			 * Envoi requete WS
			 */

			 /* XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst]sending request to  %s", dest_uri); */

			/* DONE : Ajout de SOAPAction loginRequest */
			options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
			options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 12);
			strcpy(options->opt_name, "http-header");
			options->short_name = 0;
			options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 25);
			strcpy(options->value, "SOAPAction: loginRequest");
			axutil_array_list_add(array_list2, env2, (void *) options);

			status = tewst_stub_invoke(env2, array_list2, dest_uri, input, 0);

			/* Resultats :
			 * resultCode : SUCCESS
			 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
			 */

			switch (status) {
			case TEWST_FAILURE: {
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst login failed!");

				printf("\ntewst login failed ! \n");

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
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] ERROR XDM_ETAT_AUTOTEL(TEL) NOT SENT");

					printf("[RT] Erreur msg XDM_ETAT_AUTOTEL(TEL) non envoye\n");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] XDM_ETAT_AUTOTEL(TEL) SENT");

					printf("[RT] Erreur msg XDM_ETAT_AUTOTEL(TEL) envoye\n");
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
						XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] ERROR ALARM(TEL) NOT SENT");
						printf("[RT] Erreur Alarme(TEL) non envoyee\n");
					}
					else
					{
						XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] ALARM(TEL) SENT");

						printf("[RT] Erreur Alarme(TEL) envoyee\n");
					}
				}

			}
				break;
			case TEWST_ERROR_NO_DESTURI: {
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst login failed !");

				printf("\ntewst login failed ! \n");
			}
				break;
			case TEWST_SUCCESS: {
				XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst] tewst login Success !");

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
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] ERROR ALARM(TEL) NOT SENT ");

					printf("[RT] RAZ Alarme(TEL) non envoyee\n");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] ALARM(TEL) SENT ");

					printf("[RT] RAZ Alarme(TEL) envoyee\n");
				}

			}
				break;
			};

			if ( status == TEWST_SUCCESS )
			{

				/*
				 * getTelephonicState
				 */

				sprintf(
						input,
						"<soapenv:Envelope \
xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/phoneSchema\" \
xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \
<soapenv:Header>\
</soapenv:Header>\
<soapenv:Body>\
<q0:getTelephonicStateRequest>\
<q0:fwSessionId>%s</q0:fwSessionId>\
<q0:lineNumber/>\
</q0:getTelephonicStateRequest>\
</soapenv:Body>\
</soapenv:Envelope>",
						AlcSessionId);

				strcpy(dest_uri, ws_tel_baseuri);
				strcat(dest_uri, (char *) "/AlcPhone");

				/* DONE : Ajout de SOAPAction getTelephonicStateRequest */
				options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
				options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 12);
				strcpy(options->opt_name, "http-header");
				options->short_name = 0;
				options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 37);
				strcpy(options->value, "SOAPAction: getTelephonicStateRequest");
				axutil_array_list_add(array_list2, env2, (void *) options);

				status = tewst_stub_invoke(env2, array_list2, dest_uri, input, 0);

				/* Resultats :
				 * resultCode : SUCCESS
				 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
				 */

				switch (status) {
				case TEWST_FAILURE: {
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] getTelephonicStateRequest failed!");
					printf("\ntewst getTelephonicStateRequest failed ! \n");
				}
					break;
				case TEWST_ERROR_NO_DESTURI: {
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] getTelephonicStateRequest no dest uri !");
					printf("\ntewst getTelephonicStateRequest no dest uri ! \n");
				}
					break;
				case TEWST_SUCCESS: {
					XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst] getTelephonicStateRequest Success !");
				}
					break;
				};

				if ( status == TEWST_SUCCESS )
				{
					XZST_03EcritureTrace(XZSTC_INFO, "[tewst] holdCall");
					/*
					 * holdCall
					 */

					sprintf(
							input,
							"<soapenv:Envelope \
xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/phoneSchema\" \
xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \
<soapenv:Header>\
</soapenv:Header>\
<soapenv:Body>\
<q0:holdCallRequest>\
<q0:fwSessionId>%s</q0:fwSessionId>\
<q0:lineNumber/>\
<q0:callRef>%s</q0:callRef>\
</q0:holdCallRequest>\
</soapenv:Body>\
</soapenv:Envelope>",
							AlcSessionId, AlcCurrentCallRef);

					strcpy(dest_uri, ws_tel_baseuri);
					strcat(dest_uri, (char *) "/AlcPhone");

					/* DONE : Ajout de SOAPAction holdCurrentCall */
					options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
					options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 12);
					strcpy(options->opt_name, "http-header");
					options->short_name = 0;
					options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 27);
					strcpy(options->value, "SOAPAction: holdCallRequest");
					axutil_array_list_add(array_list2, env2, (void *) options);

					status = tewst_stub_invoke(env2, array_list2, dest_uri, input, 0);

					/* Resultats :
					 * resultCode : SUCCESS
					 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
					 */

					switch (status) {
					case TEWST_FAILURE: {
						XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst holdcall failed!");
						printf("\ntewst holdcall failed ! \n");
					}
						break;
					case TEWST_ERROR_NO_DESTURI: {
						XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst holdcall no dest uri !");
						printf("\ntewst holdcall no dest uri ! \n");
					}
						break;
					case TEWST_SUCCESS: {
						XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst] holdcall Success !");
					}
						break;
					};

				}

			}
			/* end case XDC_RENVOI */
			break;
		case XDC_ANNUL_RENVOI:
			XZST_03EcritureTrace(XZSTC_INFO, "[tewst] Case command XDC_ANNUL_RENVOI");
			/*
			 * printf("Case command XDC_ANNUL_RENVOI \n");
			 */

			/* TODO : Taille dynamique malloc required ? */
			sprintf(
					input,
					"<soapenv:Envelope \
xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/frameworkSchema\" \
xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \
<soapenv:Header>\
</soapenv:Header>\
<soapenv:Body>\
<q0:loginRequest>\
<q0:loginName>%s</q0:loginName>\
<q0:password>%s</q0:password>\
</q0:loginRequest>\
</soapenv:Body>\
</soapenv:Envelope>",
					AlcLogin, AlcPassword);

			/* dest_uri = tewst_destination (argc, argv); */
			strcpy(dest_uri, ws_tel_baseuri);
			strcat(dest_uri, (char *) "/AlcApiFrameworkPort");

			/*
			 * Envoi requete WS
			 */

			 /* XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst]sending request to  %s", dest_uri); */

			/* DONE : Ajout de SOAPAction loginRequest */
			options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
			options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 12);
			strcpy(options->opt_name, "http-header");
			options->short_name = 0;
			options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 25);
			strcpy(options->value, "SOAPAction: loginRequest");
			axutil_array_list_add(array_list2, env2, (void *) options);

			status = tewst_stub_invoke(env2, array_list2, dest_uri, input, 0);

			/* Resultats :
			 * resultCode : SUCCESS
			 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
			 */

			switch (status) {
			case TEWST_FAILURE: {
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst login failed!");

				printf("\ntewst login failed ! \n");

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
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] ERROR XDM_ETAT_AUTOTEL(TEL) NOT SENT");

					printf("[RT] Erreur msg XDM_ETAT_AUTOTEL(TEL) non envoye\n");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] XDM_ETAT_AUTOTEL(TEL) SENT");

					printf("[RT] Erreur msg XDM_ETAT_AUTOTEL(TEL) envoye\n");
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
						XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] ERROR ALARM(TEL) NOT SENT");

						printf("[RT] Erreur Alarme(TEL) non envoyee\n");
					}
					else
					{
						XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] ALARM(TEL) SENT");

						printf("[RT] Erreur Alarme(TEL) envoyee\n");
					}
				}

			}
				break;
			case TEWST_ERROR_NO_DESTURI: {
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst login no dest uri !");

				printf("\ntewst login no dest uri ! \n");
			}
				break;
			case TEWST_SUCCESS: {
				XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst] login Success !");

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
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst][RT] ERROR RESET ALARM(TEL) NOT SENT");

					printf("[RT] RAZ Alarme(TEL) non envoyee\n");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_INFO, "[tewst][RT] RESET ALARM(TEL) SENT");

					printf("[RT] RAZ Alarme(TEL) envoyee\n");
				}

			}
				break;
			};

			if ( status == TEWST_SUCCESS )
			{

				/*
				 * getTelephonicState
				 */

				sprintf(
						input,
						"<soapenv:Envelope \
xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/phoneSchema\" \
xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \
<soapenv:Header>\
</soapenv:Header>\
<soapenv:Body>\
<q0:getTelephonicStateRequest>\
<q0:fwSessionId>%s</q0:fwSessionId>\
<q0:lineNumber/>\
</q0:getTelephonicStateRequest>\
</soapenv:Body>\
</soapenv:Envelope>",
						AlcSessionId);

				strcpy(dest_uri, ws_tel_baseuri);
				strcat(dest_uri, (char *) "/AlcPhone");

				/* DONE : Ajout de SOAPAction getTelephonicStateRequest */
				options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
				options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 12);
				strcpy(options->opt_name, "http-header");
				options->short_name = 0;
				options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 37);
				strcpy(options->value, "SOAPAction: getTelephonicStateRequest");
				axutil_array_list_add(array_list2, env2, (void *) options);

				status = tewst_stub_invoke(env2, array_list2, dest_uri, input, 0);

				/* Resultats :
				 * resultCode : SUCCESS
				 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
				 */

				switch (status) {
				case TEWST_FAILURE: {
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst getTelephonicStateRequest failed!");
					printf("\ntewst getTelephonicStateRequest failed ! \n");
				}
					break;
				case TEWST_ERROR_NO_DESTURI: {
					XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst getTelephonicStateRequest no dest uri !");
					printf("\ntewst getTelephonicStateRequest no dest uri ! \n");
				}
					break;
				case TEWST_SUCCESS: {
					XZST_03EcritureTrace(XZSTC_DEBUG1,
							"[tewst] getTelephonicStateRequest Success !");
				}
					break;
				};

				if ( status == TEWST_SUCCESS )
				{
					/*
					 * takeCall
					 */

					sprintf(
							input,
							"<soapenv:Envelope \
xmlns:q0=\"http://www.alcatel-lucent.com/wsp/ns/2008/03/26/ics/phoneSchema\" \
xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> \
<soapenv:Header>\
</soapenv:Header>\
<soapenv:Body>\
<q0:takeCallRequest>\
<q0:fwSessionId>%s</q0:fwSessionId>\
<q0:lineNumber/>\
<q0:callRef>%s</q0:callRef>\
</q0:takeCallRequest>\
</soapenv:Body>\
</soapenv:Envelope>",
							AlcSessionId, AlcCurrentCallRef);

					strcpy(dest_uri, ws_tel_baseuri);
					strcat(dest_uri, (char *) "/AlcPhone");

					/* DONE : Ajout de SOAPAction holdCurrentCall */
					options = AXIS2_MALLOC(env2->allocator, sizeof(tewst_options_t));
					options->opt_name = AXIS2_MALLOC(env2->allocator, sizeof(char) * 12);
					strcpy(options->opt_name, "http-header");
					options->short_name = 0;
					options->value = AXIS2_MALLOC(env2->allocator, sizeof(char) * 27);
					strcpy(options->value, "SOAPAction: takeCallRequest");
					axutil_array_list_add(array_list2, env2, (void *) options);

					status = tewst_stub_invoke(env2, array_list2, dest_uri, input, 0);

					/* Resultats :
					 * resultCode : SUCCESS
					 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
					 */

					switch (status) {
					case TEWST_FAILURE: {
						XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst takeCall failed!");
						printf("\ntewst takeCall failed ! \n");
					}
						break;
					case TEWST_ERROR_NO_DESTURI: {
						XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] tewst takeCall no dest uri !");
						printf("\ntewst takeCall no dest uri ! \n");
					}
						break;
					case TEWST_SUCCESS: {
						XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewst] takeCall Success !");
					}
						break;
					};

				}
			}
			/* end case XDC_ANNUL_RENVOI */
			break;

		default:
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewst] ERROR Case default (command unexpected)...");
			/*
			 * printf("Case command default \n");
			 */

			/* end default */
			break;
	}
	axutil_env_free((void *) env2);

}

