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
#include "tewsc.h"

/*
 * RT Server required includes
 */
#include <xdc.h>
#include <xdg.h>
#include <xdy.h>
#include <xzst.h>
#include <xzsc.h>
#include <xzsm.h>
#include <xzamc.h>
#include <xzaac.h>


/*
 * Defines
 */
#define BUFF_SIZE 1024o
#define EVENEMENTS_MAX 100
#define NUM_EQUIPEMENT 94



/*
 * minIni defines
 */
#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

/*
 * Prototypes
 */
static void print_help();

static void print_version();

static void tewsc_sortir();


static void ewsc_acquitte_cb();
static void ewsc_rapproche_cb();
static void ewsc_ignore_cb();
static void ewsc_traiter_evenements_communautaires_cb();

void ewsc_acquitte(int va_idComm);
void ewsc_rapproche(int va_IdComm, int va_NumeroFMC, int va_CleFMC, char *va_TypeEvt);
void ewsc_ignore(int va_IdComm, int va_Motif);
void ewsc_cyclique();

int numeroAutoroute(char *va_autoroute);
int siteGestion(char *va_autoroute, XDY_PR pr);
void afficher(axiom_node_t *pa_node, const axutil_env_t *env, char *prefixe);
void decoder(axiom_node_t *pa_node, const axutil_env_t *env, char *prefixe, char *parent);
void decoder_datetime(char *asf_date, XDY_Horodate *result);


/*
 * Global variables
 */
axutil_env_t *envIgnore = NULL;
axutil_env_t *envAcquitte = NULL;
axutil_env_t *envRapproche = NULL;
axutil_env_t *envCyclique = NULL;

axis2_char_t *log_path = "tewsc.log";
axutil_log_levels_t log_level = AXIS2_LOG_LEVEL_INFO;

tewsc_EvenementTO_t *p_Evenements[EVENEMENTS_MAX];
int v_Nb_Evenements;

/*
 * Ini params
 */
const char inifile[] = "/produits/migrazur/appliSD/fichiers/deq/tewsc.ini";
char log_level_string[100];
char log_path_string[250];
char ws_asf_baseuri[250];
long n;
float va_frequence;


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
	XDY_Horodate vl_HorodateSec = 0;
	XDY_Horodate vl_horodateSecDernierAppel = 0;
	int vl_JourSemaine;
	char va_frequence_str[10];

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

	XZST_03EcritureTrace(XZSTC_DEBUG1, "[TEWSC]: Begin");

	/*
	 * DONE : v��rifier s'il manque pas un free sur log_path
	 */

	/*
	 * Log  level
	 */
	n = ini_gets("tewsc", "log-level", "info", log_level_string,
			sizearray(log_level_string), inifile);

	XZST_03EcritureTrace(XZSTC_DEBUG1, "[TEWSC]: Log level : %s \n", log_level_string);

	/*
	 * Log  path
	 */
	n = ini_gets("tewsc", "log-path", ".", log_path_string,
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

	XZST_03EcritureTrace(XZSTC_DEBUG1, "[TEWSC]: Log path string : %s \n", log_path_string);

	log_path = log_path_string;

	/*
	 * TODO : A voir... param��trage du log fa��on AXIS2 ? Ou fa��on Migrazur ?
	 */
/*	env = axutil_env_create_all(log_path, log_level); */

	/*
	 * NOTICE : Simplifier (ne sert qu'�� initialiser array_list et r��cup��rer l'option --soap)
	 */
/*	array_list = tewsc_process_options(argc, argv, env); */

	/*
	 * WS Tel URI
	 */
	n = ini_gets("tewsc", "ws-asf-base-url",
			"http://www.vinci-autoroutes.com/communaute", ws_asf_baseuri,
			sizearray(ws_asf_baseuri), inifile);

	/*
	 * Frequence de l'interrogation cyclique
	 */
	n = ini_gets("tewsc", "frequence-cyclique",
			"60", va_frequence_str,
			sizearray(va_frequence_str), inifile);
	if (sscanf(va_frequence_str,"%f",&va_frequence)!=1) {
	   XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",va_frequence_str);
	   va_frequence = 60.0;
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1, "[TEWSC]: WS Tel Base URI : %s \n", ws_asf_baseuri);

	/*
	 * Init connection RT
	 */

	/*
	 * TODO : R��gler le pb de la sortie standard avec JMG ( 03/09/2010 )
	 *
	 */

	XZST_03EcritureTrace(XZSTC_DEBUG1, "[TEWSC]: XZSC_01InitCnxSup Application = \"%s\" Task = \"%s\" \n", argv[1], argv[0]);

	vl_cr = XZSC_01InitCnxSup(argv[1], argv[0], XZSCC_INTERDIRE /* mise a jour horodate*/
	, XZSCC_AUTORISER /* systeme trace */
	, XZSCC_INTERDIRE /* synchro tsetat */
	, tewsc_sortir /* fontion de sortie */
	, NULL /* redirection sortie standard *//* TODO : JMG a remplac�� "/dev/null" par NULL pour que ��a marche */
	);

/*	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE|XZSTC_DEBUG1,XZSTC_NECRAN);*/
/*	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);*/
	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);

	if (vl_cr != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur XZSC_01InitCnxSup cr = %d",
				vl_cr);
		exit(0);
	}

#if 0
	/*
	 * METHODES DE TEST
	 */
	if (argc > 2)
	{
		int idComm;
		int numeroFMC;
		int cleFMC;
		int motif;

		printf("***** MODE TEST *****\n"); fflush(stdout);
		sscanf(argv[3], "%d", &idComm);

		if (!strcmp(argv[2], "ACQUITER")){
			printf("TEST ACQUITER\n");
			printf("     ID_COMM = %d\n", idComm);
			ewsc_acquitte(idComm);
		}
		else if (!strcmp(argv[2], "IGNORER")){
			printf("TEST IGNORER\n");
			sscanf(argv[4], "%d", &motif);
			printf("     ID_COMM = %d\n", idComm);
			printf("     MOTIF   = %d\n", motif);
			ewsc_ignore(idComm, motif);
		}
		else if (!strcmp(argv[2], "RAPPROCHER")){
			printf("TEST RAPPROCHER\n");
			sscanf(argv[4], "%d", &numeroFMC);
			sscanf(argv[5], "%d", &cleFMC);
			printf("     ID_COMM = %d\n", idComm);
			printf("     NUMERO  = %d\n", numeroFMC);
			printf("     CLE     = %d\n", cleFMC);
			ewsc_rapproche(idComm, numeroFMC, cleFMC, "evt TEST");
		}
		else if (!strcmp(argv[2], "CREER")){
			XDY_Horodate horodate;
			int temp;
			char autoroute[10];
			int pr;
			int sens;
			printf("TEST CREER\n");
			printf("     ID_COMM   = %d\n", idComm);
			sscanf(argv[4], "%s", autoroute);
			printf("     AUTOROUTE = %s\n", autoroute);
			sscanf(argv[5], "%d", &pr);
			printf("     PR        = %d\n", pr);
			sscanf(argv[6], "%d", &sens);
			printf("     SENS      = %d\n", sens);
			fflush(stdout);
			horodate = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &temp, &horodate);

			if (xzaa01_Creer_Alerte_Localisee(horodate, XDC_EQT_MAC, NUM_EQUIPEMENT, XZAAC_ALERTE_COMMUNAUTAIRE, "alerte copilote test", siteGestion(autoroute, (XDY_PR) pr), autoroute, (XDY_PR) pr, sens))
			{
				printf ("**** ERREUR ****");
			}
		}
		else
		{
			printf("**** TEST ERROR ****\n");
		}
		fflush(stdout);

		tewsc_sortir();
	}
#endif

	strcpy(vl_dg, XDG_COMMUNAUTAIRE);

	if (XDG_JoindreDG(vl_dg) != T_TRUE)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,
				"probleme lors de l'abonnement au datagroup %s", vl_dg);
		exit (1);
	}

	/*
	 * Armement des Callbacks
	 */
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_COMMU_ACQ),
			(T_IPC_CONN_PROCESS_CB_FUNC) ewsc_acquitte_cb,
			(T_CB_ARG) NULL) == NULL)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,
				"probleme lors de la creation du callback de reception de XDM_COMMU_ACQ");
		exit (1);
	}

	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_COMMU_RAPPROCHE),
			(T_IPC_CONN_PROCESS_CB_FUNC) ewsc_rapproche_cb,
			(T_CB_ARG) NULL) == NULL)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,
				"probleme lors de la creation du callback de reception de XDM_COMMU_RAPPROCHE");
		exit (1);
	}

	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_COMMU_IGNORE),
			(T_IPC_CONN_PROCESS_CB_FUNC) ewsc_ignore_cb,
			(T_CB_ARG) NULL) == NULL)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,
				"probleme lors de la creation du callback de reception de XDM_COMMU_IGNORE");
		exit (1);
	}

	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_COMMU_FMC),
			(T_IPC_CONN_PROCESS_CB_FUNC) ewsc_traiter_evenements_communautaires_cb,
			(T_CB_ARG) NULL) == NULL)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,
				"probleme lors de la creation du callback de reception de XDM_COMMU_IGNORE");
		exit (1);
	}

	XZST_03EcritureTrace(XZSTC_INFO, "[tewsc] Before while...");

	while (1)
	{
		XZST_03EcritureTrace(XZSTC_INFO, "[tewsc][RT] while waiting for message...");

		if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
		                        (int *) &vl_JourSemaine, (double *) &vl_HorodateSec ) != XDC_OK )
		      {
		      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_HorodateSec );
		      return XDC_NOK;
		      }

		if ( (vl_HorodateSec - vl_horodateSecDernierAppel) > va_frequence)
		{
			/*
			 * Appel Cyclique au WS ASF pour récupérer les alertes communautaires
			 */
			XZST_03EcritureTrace(XZSTC_DEBUG1, "[TEWSC]: Reveil Cyclique pour Interrogation ASF");
			ewsc_cyclique();
			vl_horodateSecDernierAppel = vl_HorodateSec;
		}


		XZST_03EcritureTrace(XZSTC_DEBUG1, "[INFO] while waiting for RT message\n");

		pl_Msg = TipcSrvMsgNext(va_frequence);

		XZST_03EcritureTrace(XZSTC_INFO, "[tewsc][RT] Msg message received");

		if (pl_Msg != NULL)
		{

			XZST_03EcritureTrace(XZSTC_INFO, "[tewsc][RT] pl_Msg message not NULL: processing");

			TipcSrvMsgProcess(pl_Msg);
			TipcMsgDestroy(pl_Msg);

			/* Memorisation Test Presence RTserver */
			vl_IndPre = 0;
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc][RT] pl_Msg message NULL");

			/* Test de Presence RTserver */
			if (vl_IndPre == 0)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc][RT] WARNING RTWorks Error");

				XZST_03EcritureTrace(XZSTC_WARNING,
						"Erreur RTWorks num: %d -- errno %d \n",
						TutErrNumGet(), errno);
			}
#if 0
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc] WARNING stopping tewsc.x task");

				XZST_03EcritureTrace(XZSTC_WARNING,
						"Bouclage et arret tache tewsc.x %d \n", vl_IndPre);

				/* Arret de la tache */
				vl_IndPre = 0;
				tewsc_sortir();
			}
#endif
			/* Indication absence RTserver */
			vl_IndPre++;
		}

	}

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
             tewsc 1.0     \n"),
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
tewsc 1.0, command line tool to consume webservices.\n"),
							("\
Usage: tewsc.x migrazur \n\n") };

	int i = 0;
	for (i = 0; i < array_sizeof(help); i++)
		fputs(help[i], stdout);
	exit(0);
}

void tewsc_sortir()
{
	exit(0);
}

void ewsc_acquitte_cb(va_Cnx, pa_Data, pa_Arg)
	T_IPC_CONN va_Cnx;T_IPC_CONN_PROCESS_CB_DATA pa_Data;T_CB_ARG pa_Arg;
{

	int vl_IdComm;

	XZST_03EcritureTrace(XZSTC_FONCTION,"[ewsc_acquitte_cb] IN");

	if (TipcMsgRead(pa_Data->msg, T_IPC_FT_INT4, &vl_IdComm,
			NULL) != TRUE) {
		XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc][RT] WARNING error in TipcMsgRead");

		return;
	}

	XZST_03EcritureTrace(XZSTC_INFO, "[ewsc_acquitte_cb] : Id_Comm = %d\n", vl_IdComm);

	XZST_03EcritureTrace(XZSTC_FONCTION,"[ewsc_acquitte_cb] OUT");
	ewsc_acquitte(vl_IdComm);
}

void ewsc_rapproche_cb(va_Cnx, pa_Data, pa_Arg)
	T_IPC_CONN va_Cnx;T_IPC_CONN_PROCESS_CB_DATA pa_Data;T_CB_ARG pa_Arg;
{
	int *pl_IdComm;
	int *pl_NumeroFMC;
	XDY_Octet *pl_CleFMC;
	T_STR vl_TypeEvt;

	XZST_03EcritureTrace(XZSTC_FONCTION,"[ewsc_rapproche_cb] IN");

	pl_IdComm = malloc (sizeof(int));
	pl_NumeroFMC = malloc (sizeof(int));
	pl_CleFMC = malloc (sizeof(XDY_Octet));

	if (TipcMsgRead(pa_Data->msg,
			T_IPC_FT_INT4, pl_IdComm,
			T_IPC_FT_INT4, pl_NumeroFMC,
			T_IPC_FT_INT2, pl_CleFMC,
			T_IPC_FT_STR,  &vl_TypeEvt,
			NULL) != TRUE) {
		XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc][RT] WARNING error in TipcMsgRead");

		return;
	}
	XZST_03EcritureTrace(XZSTC_INFO, "[ewsc_rapproche_cb] : Id_Comm = %d, numFMC=%d, cleFMC=%d, texte=%s\n", *pl_IdComm, *pl_NumeroFMC, (int) *pl_CleFMC, vl_TypeEvt);

	ewsc_rapproche(*pl_IdComm, *pl_NumeroFMC, (int)*pl_CleFMC, vl_TypeEvt);

	free(pl_IdComm);
	free(pl_NumeroFMC);
	free(pl_CleFMC);

	XZST_03EcritureTrace(XZSTC_FONCTION,"[ewsc_rapproche_cb] OUT");
}

void ewsc_ignore_cb(va_Cnx, pa_Data, pa_Arg)
	T_IPC_CONN va_Cnx;T_IPC_CONN_PROCESS_CB_DATA pa_Data;T_CB_ARG pa_Arg;
{
	int *pl_IdComm;
	XDY_Octet *pl_Motif;

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_ignore_cb] IN");

	pl_IdComm = malloc(sizeof(int));
	pl_Motif = malloc(sizeof(XDY_Octet));

	if (TipcMsgRead(pa_Data->msg,
			T_IPC_FT_INT4, pl_IdComm,
			T_IPC_FT_INT2, pl_Motif,
			NULL) != TRUE) {
		XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc][RT] WARNING error in TipcMsgRead");
		return;
	}

	XZST_03EcritureTrace(XZSTC_INFO, "[ewsc_ignore_cb] : Id_Comm = %d, motif=%d\n", *pl_IdComm, (int) *pl_Motif);

	ewsc_ignore(*pl_IdComm, (int) *pl_Motif);

	free(pl_IdComm);
	free(pl_Motif);

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_ignore_cb] OUT");
}

void ewsc_cyclique()
{
	char input[2000];
	char dest_uri[300];
	axis2_status_t status;

	char vl_Login[50];
	char vl_Password[50];
	char vl_soapAction[200];
	char vl_soapAction_WS[50];
	char vl_soapAction_FCT[50];

	tewsc_options_t *options;
	axutil_array_list_t *array_list2;
	int fake_argc=1;
	char ** fake_argv=NULL;

	fake_argv=malloc(sizeof(char*));
	fake_argv[0]=malloc(sizeof(char));
	strcpy(fake_argv[0], "\0");

	XZST_03EcritureTrace(XZSTC_FONCTION, "[Cyclique] IN");

	envCyclique = axutil_env_create_all(log_path, log_level);

	array_list2 = tewsc_process_options(fake_argc, fake_argv, envCyclique);
/*    array_list2 = axutil_array_list_create (envCyclique, 3); */


	ini_gets("tewsc", "login", "", vl_Login, sizearray(vl_Login), inifile);
	ini_gets("tewsc", "password", "", vl_Password, sizearray(vl_Password), inifile);
	ini_gets("tewsc", "acquerir-WS", "", vl_soapAction_WS, sizearray(vl_soapAction_WS), inifile);
	ini_gets("tewsc", "acquerir-FCT", "", vl_soapAction_FCT, sizearray(vl_soapAction_FCT), inifile);

	sprintf(vl_soapAction, "%s%s#%s", ws_asf_baseuri, vl_soapAction_WS, vl_soapAction_FCT);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[TEWSC]: soapAction = %s\n", vl_soapAction);

	/* Todo: a remettre pour Escota dans header
	 * <Action s:mustUnderstand=\"1\" xmlns=\"http://schemas.microsoft.com/ws/2005/05/addressing/none\">http://vinci-autoroutes.com/communaute/ICommunauteWS/AcquerirEvenementEnCours</Action> \
	 */
/*<Action s:mustUnderstand=\"1\" xmlns=\"http://schemas.microsoft.com/ws/2005/05/addressing/none\">%s%s/%s</Action> \ */
	sprintf(
			input,
			"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n\
<s:Header>\n\
</s:Header>\n\
<s:Body>\n\
  <AcquerirEvenementEnCours xmlns=\"http://vinci-autoroutes.com/communaute\">\n\
    <utilisateur xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\n\
      <ComplementUtilisateur i:nil=\"true\"/>\n\
      <DroitEntreprise />\n\
      <Utilisateur>%s</Utilisateur>\n\
    </utilisateur>\n\
    <societeConcernee>Escota</societeConcernee>\n\
  </AcquerirEvenementEnCours>\n\
</s:Body>\n\
</s:Envelope>",
	/*		ws_asf_baseuri,
			vl_soapAction_WS,*/
			vl_Login);
/*
  <AcquerirEvenementEnCours xmlns=\"%s%s\">\n\
    <utilisateur xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\n\
    <utilisateur xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\n\

      <DroitEntreprise />\n\

      <DroitEntreprise >\n\
        <Droit>LectureEcriture</Droit>\n\
	<Entreprise>Escota</Entreprise>\n\
      </DroitEntreprise>\n\
*/
	strcpy(dest_uri, ws_asf_baseuri);
	strcat (dest_uri, vl_soapAction_WS);
/* Todo: a remettre pour Escota
 * 	strcat(dest_uri, (char *) "/AcquerirEvenementEnCours");
*/
	/*
	 * Envoi requete WS
	 */

	/* Ajout de SOAPAction */
	options = AXIS2_MALLOC(envCyclique->allocator, sizeof(tewsc_options_t));
	options->opt_name = AXIS2_MALLOC(envCyclique->allocator, sizeof(char)*12);
	options->value = AXIS2_MALLOC(envCyclique->allocator, sizeof(char)*360);
	strcpy(options->opt_name, "http-header");
	options->short_name = 0;
	sprintf(options->value, "SOAPAction: \"http://vinci-autoroutes.com/communaute/ICommunauteWS/%s\"", vl_soapAction_FCT); 
/*	sprintf(options->value, "SOAPAction: \"http://tempuri.org/%s\"", vl_soapAction_FCT); */
/*	sprintf(options->value, "SOAPAction: \"http://tempuri.org/AcquerirEvenementEnCours\""); */
/*	sprintf(options->value, "SOAPAction: \"CommunauteWS/%s#%s\"", vl_soapAction_WS, vl_soapAction_FCT); */
/*	strcpy(options->value, "SOAPAction: \"AcquerirEvenementEnCours\""); */
/*	sprintf(options->value, "SOAPAction: \"%s/%s\"", dest_uri, vl_soapAction_FCT); */
/*	sprintf(options->value, "soapAction: \"%s#%s\"", dest_uri, vl_soapAction_FCT);*/
/*	strcpy(options->value, "SOAPAction: \"AcquerirEvenementEnCours\""); */
	axutil_array_list_add(array_list2, envCyclique, (void *) options);
	
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[TEWSC Cyclique]: Requete WS(%d) = \n %s \n\n", strlen(input), input);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[TEWSC Cyclique]: Sending Request to  \"%s\" - soapAction=\"%s\"", dest_uri, options->value);
	status = tewsc_stub_invoke(envCyclique, array_list2, dest_uri, input, 0, TEWSC_ACTION_ACQUERIR);

	XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewsc] apres tewsc_stub_invoke\n");

	/* Resultats :
	 * resultCode : SUCCESS
	 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
	 */

	switch (status) {
		case TEWST_FAILURE: {
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc] tewst AcquerirEvenementEnCours failed!");
		}
			break;
		case TEWST_ERROR_NO_DESTURI: {
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc] tewst AcquerirEvenementEnCours failed !");
		}
			break;
		case TEWST_SUCCESS: {
			XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewsc] AcquerirEvenementEnCours Success !");
		}
			break;
	};


/*
	AXIS2_FREE(envCyclique->allocator, options->opt_name);
	AXIS2_FREE(envCyclique->allocator, options->value);
	AXIS2_FREE(envCyclique->allocator, options);

XZST_03EcritureTrace(XZSTC_DEBUG1, "avant free");

	free(fake_argv[0]);
	free(fake_argv);

XZST_03EcritureTrace(XZSTC_DEBUG1, "apres free");
	axutil_array_list_free(array_list2, envCyclique);
*/
XZST_03EcritureTrace(XZSTC_DEBUG1, "avant axutil_env_free (ref=%d)", envCyclique->ref);
	/* PATCH Pb avec liberation de l'env*/
	envCyclique->ref = 1;
	axutil_env_free((void *) envCyclique);

	XZST_03EcritureTrace(XZSTC_FONCTION, "[Cyclique] OUT");
}


void ewsc_acquitte(va_idComm)
	int va_idComm;
{
	char input[2000];
	char dest_uri[300];
	axis2_status_t status;

	char vl_Login[50];
	char vl_Password[50];
	char vl_soapAction[200];
	char vl_soapAction_WS[50];
	char vl_soapAction_FCT[50];

	tewsc_options_t *options;
	axutil_array_list_t *array_list2;
	int fake_argc=1;
	char ** fake_argv=NULL;

	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_acquitte] IdComm = %d", va_idComm);

	fake_argv=malloc(sizeof(char*));
	fake_argv[0]=malloc(sizeof(char));
	strcpy(fake_argv[0], "\0");

	envAcquitte = axutil_env_create_all(log_path, log_level);
	array_list2 = tewsc_process_options(fake_argc, fake_argv, envAcquitte);
/*    array_list2 = axutil_array_list_create (envAcquitte, 3);*/

	ini_gets("tewsc", "login", "", vl_Login, sizearray(vl_Login), inifile);
	ini_gets("tewsc", "password", "", vl_Password, sizearray(vl_Password), inifile);
	ini_gets("tewsc", "acquitter-WS", "", vl_soapAction_WS, sizearray(vl_soapAction_WS), inifile);
	ini_gets("tewsc", "acquitter-FCT", "", vl_soapAction_FCT, sizearray(vl_soapAction_FCT), inifile);

	sprintf(vl_soapAction, "%s%s#%s", ws_asf_baseuri, vl_soapAction_WS, vl_soapAction_FCT);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_acquitte]: soapAction = %s\n", vl_soapAction);

/* Todo : a remettre pour escota dans header
 * <Action s:mustUnderstand=\"1\" xmlns=\"http://schemas.microsoft.com/ws/2005/05/addressing/none\">http://vinci-autoroutes.com/communaute/ICommunauteWS/Acquitter</Action> \
 *
 */
	sprintf(
			input,
			"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"> \
<s:Header></s:Header>\
<s:Body>\
<Acquitter xmlns=\"http://vinci-autoroutes.com/communaute\">\n\
<utilisateur xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\
<ComplementUtilisateur i:nil=\"true\" />\
<DroitEntreprise /> \
<Utilisateur>%s</Utilisateur>\
</utilisateur>\
<idEvenement>%d</idEvenement>\
</Acquitter>\
</s:Body>\
</s:Envelope>",
			vl_Login,
			va_idComm);

	strcpy(dest_uri, ws_asf_baseuri);
	strcat (dest_uri, vl_soapAction_WS);

	/*
	 * Envoi requete WS
	 */
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_acquitte]: Requete WS(%d) = \n %s \n\n", strlen(input), input);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_acquitte]: Sending Request to  \"%s\" - soapAction=\"%s\"", dest_uri, vl_soapAction);

	/* Ajout de SOAPAction */
	options = AXIS2_MALLOC(envAcquitte->allocator, sizeof(tewsc_options_t));
	options->opt_name = AXIS2_MALLOC(envAcquitte->allocator, sizeof(char)*12);
	options->value = AXIS2_MALLOC(envAcquitte->allocator, sizeof(char)*360);
	strcpy(options->opt_name, "http-header");
	options->short_name = 0;
	sprintf(options->value, "SOAPAction: \"http://vinci-autoroutes.com/communaute/ICommunauteWS/%s\"", vl_soapAction_FCT); 
	/*sprintf(options->value, "SOAPAction: \"http://tempuri.org/%s\"", vl_soapAction_FCT);*/
/*	strcpy(options->value, "SOAPAction: AcquerirEvenementEnCours"); */
	/*sprintf(options->value, "soapAction: \"%s#%s\"", dest_uri, vl_soapAction_FCT);*/
	axutil_array_list_add(array_list2, envAcquitte, (void *) options);

	status = tewsc_stub_invoke(envAcquitte, array_list2, dest_uri, input, 0, TEWSC_ACTION_OTHER);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_acquitte] apres tewsc_stub_invoke\n");

	/* Resultats :
	 * resultCode : SUCCESS
	 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
	 */
	switch (status) {
		case TEWST_FAILURE: {
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc] Acquitter failed!");
		}
			break;
		case TEWST_ERROR_NO_DESTURI: {
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc] Acquitter failed - No Dest URI!");
		}
			break;
		case TEWST_SUCCESS: {
			XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewsc] Acquitter Success !");
		}
			break;
	};

	AXIS2_FREE(envAcquitte->allocator, options->opt_name);
	AXIS2_FREE(envAcquitte->allocator, options->value);
	AXIS2_FREE(envAcquitte->allocator, options);
	free(fake_argv[0]);
	free(fake_argv);
	axutil_array_list_free(array_list2, envAcquitte);

	/* PATCH Pb avec liberation de l'env*/
	envAcquitte->ref = 1;
	axutil_env_free((void *) envAcquitte);

}

void ewsc_rapproche(va_IdComm, va_NumeroFMC, va_CleFMC, va_TypeEvt)
	int va_IdComm;	int va_NumeroFMC;	int va_CleFMC; char *va_TypeEvt;
{
	char input[2000];
	char dest_uri[300];
	axis2_status_t status;

	char vl_Login[50];
	char vl_Password[50];
	char vl_soapAction[200];
	char vl_soapAction_WS[50];
	char vl_soapAction_FCT[50];

	char vl_Id[9];
	char vl_Reference[15];
	char vl_TypeEvt[50];

	tewsc_options_t *options;
	axutil_array_list_t *array_list2;
	int fake_argc=1;
	char ** fake_argv=NULL;

	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_rapproche] IdComm=%d, numeroFMC=%d, cleFMC=%d", va_IdComm, va_NumeroFMC, va_CleFMC);

	fake_argv=malloc(sizeof(char*));
	fake_argv[0]=malloc(sizeof(char));
	strcpy(fake_argv[0], "\0");

	envRapproche = axutil_env_create_all(log_path, log_level);
	array_list2 = tewsc_process_options(fake_argc, fake_argv, envRapproche);
/*    array_list2 = axutil_array_list_create (envRapproche, 3);*/

	ini_gets("tewsc", "login", "", vl_Login, sizearray(vl_Login), inifile);
	ini_gets("tewsc", "password", "", vl_Password, sizearray(vl_Password), inifile);
	ini_gets("tewsc", "rapprocher-WS", "", vl_soapAction_WS, sizearray(vl_soapAction_WS), inifile);
	ini_gets("tewsc", "rapprocher-FCT", "", vl_soapAction_FCT, sizearray(vl_soapAction_FCT), inifile);

	sprintf(vl_soapAction, "%s%s#%s", ws_asf_baseuri, vl_soapAction_WS, vl_soapAction_FCT);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_rapproche]: soapAction = %s\n", vl_soapAction);


	sprintf(vl_Id, "%d%d", va_NumeroFMC, va_CleFMC);
	sprintf(vl_Reference, "ID_%d-%d", va_NumeroFMC, va_CleFMC);
	sprintf(vl_TypeEvt, va_TypeEvt);
	/* FORCAGE Type Evenement a NonDefini */
	sprintf(vl_TypeEvt, "NonDefini");

	/* Todo : a remettre pour escota dans header
	* <Action s:mustUnderstand=\"1\" xmlns=\"http://schemas.microsoft.com/ws/2005/05/addressing/none\">http://vinci-autoroutes.com/communaute/ICommunauteWS/Acquitter</Action> \
	*
	*/
	sprintf(
			input,
			"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"> \
	<s:Header></s:Header>\
	<s:Body>\
	<Rapprocher xmlns=\"http://vinci-autoroutes.com/communaute\">\n\
	<utilisateur xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\
	<ComplementUtilisateur i:nil=\"true\" />\
	<DroitEntreprise /> \
	<Utilisateur>%s</Utilisateur>\
	</utilisateur>\
	<idEvtSAE xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\
	<Complement_id>%d</Complement_id>\
	<Fournisseur>Escota</Fournisseur>\
	<Id>%d</Id>\
	<Reference>%s</Reference>\
	<TypeEvt>%s</TypeEvt>\
	</idEvtSAE>\
	<idEvenement>%d</idEvenement>\
	</Rapprocher>\
	</s:Body>\
	</s:Envelope>",
			vl_Login,
			0,
			-1, /* vl_Id, */
			vl_Reference,
			vl_TypeEvt,
			va_IdComm);

	strcpy(dest_uri, ws_asf_baseuri);
	strcat (dest_uri, vl_soapAction_WS);

	/*
	 * Envoi requete WS
	 */
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_rapproche]: Requete WS(%d) = \n %s \n\n", strlen(input), input);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_rapproche]: Sending Request to  \"%s\" - soapAction=\"%s\"", dest_uri, vl_soapAction);

	/* Ajout de SOAPAction */
	options = AXIS2_MALLOC(envRapproche->allocator, sizeof(tewsc_options_t));
	options->opt_name = AXIS2_MALLOC(envRapproche->allocator, sizeof(char)*12);
	options->value = AXIS2_MALLOC(envRapproche->allocator, sizeof(char)*360);
	strcpy(options->opt_name, "http-header");
	options->short_name = 0;
	sprintf(options->value, "SOAPAction: \"http://vinci-autoroutes.com/communaute/ICommunauteWS/%s\"", vl_soapAction_FCT); 
	/*sprintf(options->value, "SOAPAction: \"http://tempuri.org/%s\"", vl_soapAction_FCT);*/
	/*	strcpy(options->value, "SOAPAction: AcquerirEvenementEnCours"); */
	/*sprintf(options->value, "soapAction: \"%s#%s\"", dest_uri, vl_soapAction_FCT);*/
	axutil_array_list_add(array_list2, envRapproche, (void *) options);

	status = tewsc_stub_invoke(envRapproche, array_list2, dest_uri, input, 0, TEWSC_ACTION_OTHER);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_rapproche] apres tewsc_stub_invoke\n");

	/* Resultats :
	 * resultCode : SUCCESS
	 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
	 */
	switch (status) {
		case TEWST_FAILURE: {
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc] Rapprocher failed!");
		}
			break;
		case TEWST_ERROR_NO_DESTURI: {
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc] Rapprocher failed - No Dest URI!");
		}
			break;
		case TEWST_SUCCESS: {
			XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewsc] Rapprocher Success !");
		}
			break;
	};

	AXIS2_FREE(envRapproche->allocator, options->opt_name);
	AXIS2_FREE(envRapproche->allocator, options->value);
	AXIS2_FREE(envRapproche->allocator, options);
	free(fake_argv[0]);
	free(fake_argv);
	axutil_array_list_free(array_list2, envRapproche);

	/* PATCH Pb avec liberation de l'env*/
	envRapproche->ref = 1;
	axutil_env_free((void *) envRapproche);

}

void ewsc_ignore(va_IdComm, va_Motif)
	int va_IdComm;	int va_Motif;
{
	char input[2000];
	char dest_uri[300];
	axis2_status_t status;

	char vl_Login[50];
	char vl_Password[50];
	char vl_soapAction[200];
	char vl_soapAction_WS[50];
	char vl_soapAction_FCT[50];

	char vl_Motif[20];

	tewsc_options_t *options;
	axutil_array_list_t *array_list2;
	int fake_argc=1;
	char ** fake_argv=NULL;

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_ignore] IdComm=%d, idMotif=%d", va_IdComm, va_Motif);

	fake_argv=malloc(sizeof(char*));
	fake_argv[0]=malloc(sizeof(char));
	strcpy(fake_argv[0], "\0");

	envIgnore = axutil_env_create_all(log_path, log_level);
	array_list2 = tewsc_process_options(fake_argc, fake_argv, envIgnore);
/*    array_list2 = axutil_array_list_create (envIgnore, 3);*/

	ini_gets("tewsc", "login", "", vl_Login, sizearray(vl_Login), inifile);
	ini_gets("tewsc", "password", "", vl_Password, sizearray(vl_Password), inifile);
	ini_gets("tewsc", "ignorer-WS", "", vl_soapAction_WS, sizearray(vl_soapAction_WS), inifile);
	ini_gets("tewsc", "ignorer-FCT", "", vl_soapAction_FCT, sizearray(vl_soapAction_FCT), inifile);

	sprintf(vl_soapAction, "%s%s#%s", ws_asf_baseuri, vl_soapAction_WS, vl_soapAction_FCT);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_ignore]: soapAction = %s\n", vl_soapAction);


	if (va_Motif == XDC_COMMU_FAUSSE_ALERTE)
		strcpy(vl_Motif, MOTIF_FAUSSE_ALERTE);
	else if (va_Motif == XDC_COMMU_ESSAI)
		strcpy(vl_Motif, MOTIF_ESSAI);
	else{
		XZST_03EcritureTrace(XZSTC_WARNING, "[ewsc_ignore] idMotif %d INCONNU", va_Motif);
		axutil_env_free((void *) envIgnore);

		return;
	}


	/* Todo : a remettre pour escota dans header
	* <Action s:mustUnderstand=\"1\" xmlns=\"http://schemas.mic	AXIS2_FREE(envCyclique->allocator, options->opt_name);
	AXIS2_FREE(envCyclique->allocator, options->value);
	* rosoft.com/ws/2005/05/addressing/none\">http://vinci-autoroutes.com/communaute/ICommunauteWS/Acquitter</Action> \
	*
	*/
	sprintf(
			input,
			"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"> \
	<s:Header></s:Header>\
	<s:Body>\
	<IgnorerEvenement xmlns=\"http://vinci-autoroutes.com/communaute\">\n\
	<utilisateur xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\">\
	<ComplementUtilisateur i:nil=\"true\" />\
	<DroitEntreprise /> \
	<Utilisateur>%s</Utilisateur>\
	</utilisateur>\
	<idEvenement>%d</idEvenement>\
	<cause>%s</cause>\
	<commentaire i:nil=\"true\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\"/>\
	</IgnorerEvenement>\
	</s:Body>\
	</s:Envelope>",
			vl_Login,
			va_IdComm,
			vl_Motif);

	strcpy(dest_uri, ws_asf_baseuri);
	strcat (dest_uri, vl_soapAction_WS);

	/*
	 * Envoi requete WS
	 */
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_ignore]: Requete WS(%d) = \n %s \n\n", strlen(input), input);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_ignore]: Sending Request to  \"%s\" - soapAction=\"%s\"", dest_uri, vl_soapAction);

	/* Ajout de SOAPAction */
	options = AXIS2_MALLOC(envIgnore->allocator, sizeof(tewsc_options_t));
	options->opt_name = AXIS2_MALLOC(envIgnore->allocator, sizeof(char)*12);
	options->value = AXIS2_MALLOC(envIgnore->allocator, sizeof(char)*360);
	strcpy(options->opt_name, "http-header");
	options->short_name = 0;
	sprintf(options->value, "SOAPAction: \"http://vinci-autoroutes.com/communaute/ICommunauteWS/%s\"", vl_soapAction_FCT); 
	/*sprintf(options->value, "SOAPAction: \"http://tempuri.org/%s\"", vl_soapAction_FCT);*/
	/*	strcpy(options->value, "SOAPAction: AcquerirEvenementEnCours"); */
	/*sprintf(options->value, "soapAction: \"%s#%s\"", dest_uri, vl_soapAction_FCT);*/
	axutil_array_list_add(array_list2, envIgnore, (void *) options);

	status = tewsc_stub_invoke(envIgnore, array_list2, dest_uri, input, 0, TEWSC_ACTION_OTHER);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_ignore] apres tewsc_stub_invoke\n");

	/* Resultats :
	 * resultCode : SUCCESS
	 * resultCode : ERROR_INCOMPATIBLE_WITH_STATE
	 */
	switch (status) {
		case TEWST_FAILURE: {
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc] Ignorer failed!");
		}
			break;
		case TEWST_ERROR_NO_DESTURI: {
			XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc] Ignorer failed - No Dest URI!");
		}
			break;
		case TEWST_SUCCESS: {
			XZST_03EcritureTrace(XZSTC_DEBUG1, "[tewsc] Ignorer Success !");
		}
			break;
	};

	AXIS2_FREE(envIgnore->allocator, options->opt_name);
	AXIS2_FREE(envIgnore->allocator, options->value);
	AXIS2_FREE(envIgnore->allocator, options);
	free(fake_argv[0]);
	free(fake_argv);
	axutil_array_list_free(array_list2, envIgnore);

	/* PATCH Pb avec liberation de l'env*/
	envIgnore->ref = 1;
	axutil_env_free((void *) envIgnore);

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_ignore] : OUT");
}


void ewsc_decoder_CR_asf(
		axiom_node_t * pa_node,
	    const axutil_env_t * pa_env)
{
	char *tempo;

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_decoder_CR_asf]: IN");

	tempo = axiom_node_to_string(pa_node, pa_env);
	XZST_03EcritureTrace(XZSTC_INFO, "[ewsc_decoder_CR_asf]: reponse:\n%s", tempo);

	AXIS2_FREE(pa_env->allocator, tempo);

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_decoder_CR_asf]: OUT");
}


void ewsc_decoder_reponse_asf(
	    axiom_node_t * pa_node,
	    const axutil_env_t * pa_env)
{

   int pl_Tab[EVENEMENTS_MAX];
   int pl_TabT[EVENEMENTS_MAX];
   int pl_TabS[EVENEMENTS_MAX];
   int i, j;
   tewsc_EvenementTO_t *pl_Evenement;
   XDY_Datagroup vl_dg;

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_decoder_reponse_asf]: IN");

	for (i=0; i<v_Nb_Evenements; i++)
		free(p_Evenements[i]);
	v_Nb_Evenements = 0;

/*	afficher(pa_node, pa_env, ""); */
	decoder(pa_node, pa_env, "", "");

	/* Parcours du Flux XML reçu*/
	j=0;
	for (i = 0; i< v_Nb_Evenements; i++){
		/* Pour chaque evenenement*/
		if (!strcmp(p_Evenements[i]->Source, TEWSC_SOURCE_COMMUNAUTAIRE)
				&& (strcmp(p_Evenements[i]->Etat, TEWSC_ETAT_FAUSSEALERTE))
				&& (strcmp(p_Evenements[i]->Etat, TEWSC_ETAT_NONDEFINI)))
		{
			pl_Evenement = p_Evenements[i];
			pl_Tab[j] = pl_Evenement->Id;
			pl_TabS[j] = pl_Evenement->Synthese;
			pl_TabT[j] = pl_Evenement->Tendance;
			j++;
			XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_decoder_reponse_asf]: pl_tab[%d]=%d (PR=%d)", i, pl_Evenement->Id, pl_Evenement->PRDebut);
		}
	}

	if (j > 0){
		XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_decoder_reponse_asf]: Envoi du Message XDM_AA_COMMU_FMC");
		strcpy(vl_dg, XDG_AS_COMMU_FMC);
		if (!TipcSrvMsgWrite ( vl_dg,
						XDM_IdentMsg(XDM_AA_COMMU_FMC),
							XDM_FLG_SRVMSGWRITE,
							T_IPC_FT_INT4_ARRAY ,  pl_Tab , j,
							T_IPC_FT_INT4_ARRAY ,  pl_TabS , j,
							T_IPC_FT_INT4_ARRAY ,  pl_TabT , j,
							NULL))
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_AA_COMMU_FMC non effectue." );
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_AA_COMMU_FMC effectue." );
		}
	} else {
		XZST_03EcritureTrace(XZSTC_MESSAGE,"Rien a envoyer dans XDM_AA_COMMU_FMC" );
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_decoder_reponse_asf]: OUT");
}

void ewsc_traiter_evenements_communautaires_cb(va_Cnx, pa_Data, pa_Arg)
	T_IPC_CONN va_Cnx;T_IPC_CONN_PROCESS_CB_DATA pa_Data;T_CB_ARG pa_Arg;
{
tewsc_EvenementTO_t *pl_Evenement;
int				   	vl_IdComm;
XDY_Entier			vl_NumeroFMC;
XDY_Octet			vl_CleFMC;
XDY_Booleen	       	vl_Encours;
XDY_Synthese_Commu 	vl_Synthese;
XDY_Tendance_Commu 	vl_Tendance;
XDY_TexteAlerte    	vl_TexteAlerte;
int i;

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_traiter_evenements_communautaires_cb] IN");

	if (TipcMsgRead(pa_Data->msg,
			T_IPC_FT_INT4, &vl_IdComm,
			T_IPC_FT_INT4, &vl_NumeroFMC,
			T_IPC_FT_INT2, &vl_CleFMC,
			T_IPC_FT_BOOL, &vl_Encours,
			T_IPC_FT_INT4, &vl_Synthese,
			T_IPC_FT_INT4, &vl_Tendance,
			NULL) != TRUE) {
		XZST_03EcritureTrace(XZSTC_WARNING, "[tewsc][RT] WARNING error in TipcMsgRead");
	}

	XZST_03EcritureTrace(XZSTC_INFO, "[ewsc_traiter_evenements_communautaires_cb]: vl_IdComm=%d, vl_NumeroFMC=%d, vl_CleFMC=%d, vl_Encours=%d, vl_Synthese=%d, vl_Tendance=%d"
					, vl_IdComm
					, vl_NumeroFMC
					, vl_CleFMC
					, vl_Encours
					, vl_Synthese
					, vl_Tendance);

	XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_traiter_evenements_communautaires_cb]: recherche evenement recu %d", vl_IdComm);
	for (i = 0; i< v_Nb_Evenements; i++){
		if (p_Evenements[i] != NULL){
			if (p_Evenements[i]->Id == vl_IdComm){
				XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_traiter_evenements_communautaires_cb]: trouve : i=%d, id=%d", i, p_Evenements[i]->Id);
				pl_Evenement = p_Evenements[i];

				if (vl_NumeroFMC == -1)
				{
					XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_traiter_evenements_communautaires_cb]: debut creation alerte");
					/* FMC Inexistante */
					/* Creation ALERTE COMUNAUTAIRE */
					if (!strcmp(pl_Evenement->TypeLocalisation, "")){
					XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_traiter_evenements_communautaires_cb]: TypeLocalisation Vide !, remplacement par Autoroute");
						strcpy(pl_Evenement->TypeLocalisation, "Autoroute");
}
					sprintf(vl_TexteAlerte, "%d#%s#%s#%s#%d#%d", pl_Evenement->Id, pl_Evenement->TypeEvenement, pl_Evenement->TypeLocalisation, pl_Evenement->Etat, pl_Evenement->Tendance, pl_Evenement->Synthese);

					XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_traiter_evenements_communautaires_cb]: CREATION ALERTE \"%s\" %s PR:%d Sens:%d \n ", vl_TexteAlerte, pl_Evenement->Autoroute, pl_Evenement->PRDebut, pl_Evenement->Sens);
			XDY_Horodate horodate;
			int temp;
			XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &temp, &horodate);

					if (xzaa01_Creer_Alerte_Localisee(horodate,
														XDC_EQT_MAC,
														NUM_EQUIPEMENT,
														XZAAC_ALERTE_COMMUNAUTAIRE,
														vl_TexteAlerte,
														siteGestion(pl_Evenement->Autoroute, pl_Evenement->PRDebut),
														pl_Evenement->Autoroute,
														pl_Evenement->PRDebut,
														pl_Evenement->Sens) == XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_traiter_evenements_communautaires_cb]: creation OK");
					}
					else
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "[ewsc_traiter_evenements_communautaires_cb]: echec Creation Alerte");
					}

				}else
				{
					XZST_03EcritureTrace(XZSTC_DEBUG1, "[ewsc_traiter_evenements_communautaires_cb]: alerte liee a une FMC Existante");
					/* FMC Existante */
				}
				free(pl_Evenement);
				p_Evenements[i] = NULL;
			}
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "[ewsc_traiter_evenements_communautaires_cb] OUT");
}


void decoder(axiom_node_t *pa_node, const axutil_env_t *env, char *prefixe, char *parent)
{
	axiom_element_t *pl_element;
	axis2_char_t *pl_node_name;
	axiom_types_t node_type;
	axutil_hash_t *pl_liste;

	axiom_node_t *child;
	axiom_node_t *brother;
	axis2_char_t *value;

	char newPrefixe[200];
	char descrition[200];

	pl_element = axiom_node_get_data_element(pa_node, env);
	pl_node_name = axiom_element_get_localname(pl_element, env);

	sprintf(descrition,"%s > %s", prefixe, pl_node_name);

	node_type = axiom_node_get_node_type(pa_node, env);

	if (node_type == AXIOM_DOCUMENT){
		sprintf(descrition,"%s [TYPE = DOC]\n", descrition);

	} else if (node_type == AXIOM_ELEMENT) {
		sprintf(descrition, "%s [TYPE = ELEMENT]", descrition);

		if (axiom_element_get_is_empty(pl_element, env)){
			sprintf(descrition, "%s [ELEMENT IS EMPTY]", descrition);

			pl_liste = axiom_element_get_all_attributes(pl_element, env);

			if (pl_liste){
				int x = (int)axutil_hash_count(pl_liste);
				sprintf(descrition, "%s [ATTRIBUTES NOT EMPTY : %d]", descrition, x);
			} else {
				sprintf(descrition, "%s [ATTRIBUTES EMPTY]", descrition);
			}
		}else
		{
			sprintf(descrition, "%s [ELEMENT IS NOT EMPTY]", descrition);
			pl_liste = axiom_element_get_all_attributes(pl_element, env);

			if (pl_liste){
				int x = (int)axutil_hash_count(pl_liste);
				sprintf(descrition, "%s [ATTRIBUTES NOT EMPTY : %d]", descrition, x);
			} else {
				sprintf(descrition, "%s [ATTRIBUTES EMPTY]", descrition);
			}

			if (!axiom_node_get_first_element(pa_node, env))
			{
				value = axiom_element_get_text(pl_element, env, pa_node);
				sprintf(descrition, "%s Value=%s",descrition, value);
			}
			/*
			 * SI EvenementTO :
			 * créer une nouvelle structure courante et l'ajouter à la liste des evenements
			 * SI EntrepriseConcernee mettre a jour le champ correspondant
			 * SI Etat mettre a jour le champ correspondant
			 * ...
			 */

			if (!strcmp(pl_node_name, "EvenementTO")){
/*					if (p_Evenements[v_Nb_Evenements] != NULL)
						free(p_Evenements[v_Nb_Evenements]);
*/
				p_Evenements[v_Nb_Evenements] = malloc(sizeof(tewsc_EvenementTO_t));
				v_Nb_Evenements++;
				p_Evenements[v_Nb_Evenements-1]->Tendance = TEWSC_TENDANCE_NONDEFINIE;
				p_Evenements[v_Nb_Evenements-1]->Synthese = TEWSC_SYNTHESE_NULL;
			}

			else if (!strcmp(pl_node_name, "EntrepriseConcernee"))
				strcpy(p_Evenements[v_Nb_Evenements-1]->Enterprise, value);

			else if (!strcmp(pl_node_name, "Etat"))
				strcpy(p_Evenements[v_Nb_Evenements-1]->Etat, value);

			else if (!strcmp(pl_node_name, "EvtType"))
				strcpy(p_Evenements[v_Nb_Evenements-1]->TypeEvenement, value);

			else if (!strcmp(pl_node_name, "HorodateCreation"))
				decoder_datetime(value, &(p_Evenements[v_Nb_Evenements-1]->HorodateCreation));

			else if (!strcmp(pl_node_name, "HorodateDernierSignalement"))
				decoder_datetime(value, &(p_Evenements[v_Nb_Evenements-1]->HorodateDernierSignalement));

			else if (!strcmp(pl_node_name, "HorodateDerniereModification"))
				decoder_datetime(value, &(p_Evenements[v_Nb_Evenements-1]->HorodateDerniereModification));

			else if (!strcmp(pl_node_name, "Id"))
			{
				if (!strcmp(parent, "EvenementTO"))
					sscanf(value,"%d", &(p_Evenements[v_Nb_Evenements-1]->Id));
				else if (!strcmp(parent, "Rapprochement"))
					sscanf(value,"%d", &(p_Evenements[v_Nb_Evenements-1]->RapprochementId));
			}
			else if (!strcmp(pl_node_name, "Autoroute")){
				strcpy(p_Evenements[v_Nb_Evenements-1]->Autoroute, value);
			}

			else if (!strcmp(pl_node_name, "PrDebut")){
				sscanf(value,"%d", &(p_Evenements[v_Nb_Evenements-1]->PRDebut));
/*				printf("PR lu = %s / PR Decode = %d \n", value, p_Evenements[v_Nb_Evenements-1]->PRDebut);
				fflush(stdout);
*/			}

			else if (!strcmp(pl_node_name, "PrFin")){
				sscanf(value,"%d", &(p_Evenements[v_Nb_Evenements-1]->PRFin));
/*			printf("PR Debut = %d \n", p_Evenements[v_Nb_Evenements-1]->PRDebut);
			fflush(stdout);
*/			}
			else if (!strcmp(pl_node_name, "Sens"))
				sscanf(value, "Sens%d", (int *)&(p_Evenements[v_Nb_Evenements-1]->Sens));

			else if (!strcmp(pl_node_name, "TypeLocalisation"))
				strcpy(p_Evenements[v_Nb_Evenements-1]->TypeLocalisation, value);

			else if (!strcmp(pl_node_name, "Complement_id"))
				sscanf(value,"%d", &(p_Evenements[v_Nb_Evenements-1]->RapprochementComplement_Id));

/*			else if (!strcmp(pl_node_name, "Fournisseur"))
				null;
*/
			else if (!strcmp(pl_node_name, "Reference"))
				strcpy(p_Evenements[v_Nb_Evenements-1]->RapprochementReference, value);

			else if (!strcmp(pl_node_name, "TypeEvt"))
				strcpy(p_Evenements[v_Nb_Evenements-1]->RapprochementTypeEvenement, value);

			else if (!strcmp(pl_node_name, "Source"))
				strcpy(p_Evenements[v_Nb_Evenements-1]->Source, value);

			else if (!strcmp(pl_node_name, "Synthese"))
			{
				if (!strcmp(value, "NonDefini"))
					p_Evenements[v_Nb_Evenements-1]->Synthese = TEWSC_SYNTHESE_NULL;
				else if (!strcmp(value, "OK"))
					p_Evenements[v_Nb_Evenements-1]->Synthese = TEWSC_SYNTHESE_OK;
				else if (!strcmp(value, "KO"))
					p_Evenements[v_Nb_Evenements-1]->Synthese = TEWSC_SYNTHESE_KO;
				else{
					XZST_03EcritureTrace(XZSTC_WARNING, "Decoder : Synthese \"%s\" Inconnnue",  value);
					p_Evenements[v_Nb_Evenements-1]->Synthese = TEWSC_SYNTHESE_NULL;
				}
			}
			else if (!strcmp(pl_node_name, "Tendance"))
			{
				if (!strcmp(value, "Positive"))
					p_Evenements[v_Nb_Evenements-1]->Tendance = TEWSC_TENDANCE_POSITIVE;
				else if (!strcmp(value, "Negative"))
					p_Evenements[v_Nb_Evenements-1]->Tendance = TEWSC_TENDANCE_NEGATIVE;
				else if (!strcmp(value, "Egale"))
					p_Evenements[v_Nb_Evenements-1]->Tendance = TEWSC_TENDANCE_EGALE;
				else if (!strcmp(value, "NonDefini"))
					p_Evenements[v_Nb_Evenements-1]->Tendance = TEWSC_TENDANCE_NONDEFINIE;
				else {
					XZST_03EcritureTrace(XZSTC_WARNING, "Decoder : Tendance \"%s\" Inconnnue",  value);
					p_Evenements[v_Nb_Evenements-1]->Tendance = TEWSC_TENDANCE_NONDEFINIE;
				}
			}
		}

/*
		printf("%s\n",descrition);
		fflush(stdout);
*/
		if (! axiom_element_get_is_empty(pl_element, env)){
			child = axiom_node_get_first_element(pa_node, env);
			if (child){
				sprintf(newPrefixe, "%s  ", prefixe);
				decoder(child, env, newPrefixe, pl_node_name);
				while (brother = axiom_node_get_next_sibling(child,env)){
					decoder(brother, env, newPrefixe, pl_node_name);
					child = brother;
				}
			}
		}
	}

	fflush(stdout);

}

void decoder_datetime(char *asf_date, XDY_Horodate *result){
int j,m,a,h,i,s;
XZSMT_Horodate horodate;

	if (sscanf(asf_date
		  ,"%02u-%02u-%04uT%02u:%02u:%02u"
		  ,&a
		  ,&m
		  ,&j
		  ,&h
		  ,&i
		  ,&s) == 6)
	{
		sprintf(horodate, "%02u/%02u/%04u %02u:%02u:%02u"
						,j
						,m
						,a
						,h
						,i
						,s);
		XZSM_13ConversionHorodateSec(horodate, result);
	}

}

void afficher(axiom_node_t *pa_node, const axutil_env_t *env, char *prefixe)
{


	axiom_element_t *pl_element;
	axis2_char_t *pl_node_name;
	axiom_types_t node_type;
	axutil_hash_t *pl_liste;

	axiom_node_t *child1;
	axiom_node_t *child2;
	axiom_node_t *other_child;
	char newPrefixe[200];
	char descrition[200];

	pl_element = axiom_node_get_data_element(pa_node, env);
	pl_node_name = axiom_element_get_localname(pl_element, env);

/*	printf("%s> NOM = %s\n",prefixe, pl_node_name); */
	sprintf(descrition,"%s > %s", prefixe, pl_node_name);

	node_type = axiom_node_get_node_type(pa_node, env);

	if (node_type == AXIOM_DOCUMENT){
		sprintf(descrition,"%s [TYPE = DOC]\n", descrition);

	} else if (node_type == AXIOM_ELEMENT) {
		sprintf(descrition, "%s [TYPE = ELEMENT]", descrition);

		if (axiom_element_get_is_empty(pl_element, env)){
			sprintf(descrition, "%s [ELEMENT IS EMPTY]", descrition);

			pl_liste = axiom_element_get_all_attributes(pl_element, env);

			if (pl_liste){
				int x = (int)axutil_hash_count(pl_liste);
				sprintf(descrition, "%s [ATTRIBUTES NOT EMPTY : %d]", descrition, x);
			} else {
				sprintf(descrition, "%s [ATTRIBUTES EMPTY]", descrition);
			}
		}else
		{
			sprintf(descrition, "%s [ELEMENT IS NOT EMPTY]", descrition);
			pl_liste = axiom_element_get_all_attributes(pl_element, env);

			if (pl_liste){
				int x = (int)axutil_hash_count(pl_liste);
				sprintf(descrition, "%s [ATTRIBUTES NOT EMPTY : %d]", descrition, x);
/*
				axutil_hash_entry_t attribute;

				for (int i=0; i<x; i++){
					attribute = pl_liste->array[i];
					sprintf(descrition, "%s Value=%s",description, axiom_attribute_get_value((axiom_attribute_t)entry, env));
				}
*/
			} else {
				sprintf(descrition, "%s [ATTRIBUTES EMPTY]", descrition);
			}
			axis2_char_t *value;

			value = axiom_element_get_text(pl_element, env, pa_node);
			sprintf(descrition, "%s Value=%s",descrition, value);

		}


/*
 * SI EvenementTO :
 * créer une nouvelle structure courante et l'ajouter à la liste des evenements
 * SI EntrepriseConcernee mettre a jour le champ correspondant
 * SI Etat mettre a jour le champ correspondant
 * ...
 */


		printf("%s\n",descrition);
		fflush(stdout);

		if (! axiom_element_get_is_empty(pl_element, env)){
		/*childs*/
		child1 = axiom_node_get_first_child(pa_node, env);
/*		if (child1)
			afficher(child1, env, "  _CHILD1_");
*/		child2 = axiom_node_get_first_element(pa_node, env);
		if (child2){
/*			sprintf(newPrefixe, "%s > %s", prefixe, pl_node_name);*/
			sprintf(newPrefixe, "%s  ", prefixe);
			afficher(child2, env, newPrefixe);
			int i=1;
			while (other_child = axiom_node_get_next_sibling(child2,env)){
				i+=1;
/*				sprintf(newPrefixe, "  %s > %s", prefixe, pl_node_name);*/
				afficher(child2, env, newPrefixe);
				child2 = other_child;
			}
		}
		}
	}

	fflush(stdout);

	/* Parcours du Flux XML reçu*/

		/* Pour chaque evenenement*/

		/* recherche FMC liee */

			/* FMC Existante */

				/* FMC en Cours :
				 * verifier si la tendance ou la synthese a changer
				 * MISE A JOUR
				 * */

			/* FMC Inexistante */
				/* Creation ALERTE COMUNAUTAIRE */

	/*
	 * PENSER a LIBERER la MEMOIRE !!!!!
	 */
}


int numeroAutoroute(char *va_autoroute)
{
	XZST_03EcritureTrace(XZSTC_FONCTION, "numeroAutoroute IN: va_autoroute=%s",  va_autoroute);

	if (!strcmp(va_autoroute, "A58"))
		return 1;
	else if (!strcmp(va_autoroute, "A500"))
		return 2;
	else if (!strcmp(va_autoroute, "A50"))
		return 3;
	else if (!strcmp(va_autoroute, "A501"))
		return 4;
	else if (!strcmp(va_autoroute, "A51"))
		return 5;
	else if (!strcmp(va_autoroute, "A52"))
		return 6;
	else if (!strcmp(va_autoroute, "A520"))
		return 7;
	else if (!strcmp(va_autoroute, "A57"))
		return 8;
	else if (!strcmp(va_autoroute, "A57"))
		return 9;
	else if (!strcmp(va_autoroute, "N296"))
		return 10;
	else if (!strcmp(va_autoroute, "A570"))
		return 11;
	else return 0;

	XZST_03EcritureTrace(XZSTC_FONCTION, "numeroAutoroute OUT");
}

int siteGestion(char *va_autoroute, XDY_PR va_pr)
{
	XZST_03EcritureTrace(XZSTC_FONCTION, "siteGestion IN: va_autoroute=%s, va_pr=%d",  va_autoroute, va_pr);

	if (!strcmp(va_autoroute, "A51"))
		return XDC_DP;
	else if (!strcmp(va_autoroute, "A500"))
		return XDC_VC;
	else if (!strcmp(va_autoroute, "A57"))
		return XDC_VC;
	else if (!strcmp(va_autoroute, "A8"))
	{
		if (va_pr > 57700)
			return XDC_VC;
		else
			return XDC_DP;
	}
	else return XDC_DP;

	XZST_03EcritureTrace(XZSTC_FONCTION, "siteGestion OUT");
}
