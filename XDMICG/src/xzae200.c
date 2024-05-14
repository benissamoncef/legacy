/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae200.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1
* JMG	17/02/04 	SAGA : return de datex *
* JMG	25/10/05	ajout param pour PMVA 1.3
* JPL	27/02/09 : Ajout parametre 'Feu maitrise' (DEM 845) 1.4
* PNI	30/03/09 : Ajout retour d'un flag cause_operation pour affichage cause sur PMVA DEM876 1.5
* PNI 09/10/09	: Config_Voies passe à char[10] pour ajouter le picto en sens inverse DEM915 1.6
* JPL	07/09/10 : Taille du texte 'conseil' plus grande (pour DEM 942) 1.7
* JPL	12/10/11 : Ajout parametre 'origine' (DEM 995)  1.8
* JPL	03/02/12 : Taille du texte '_longueur_bouchon' plus grande (pour DEM 1010)  1.9
* VR	25/05/12 : ajout regulation DEM/1016
* JPL	09/07/12 : Ajout parametres 'sortie usager' et 'indicateur bifurcation' (DEM 1033)  1.11
* JMG	25/01/13 : correction pb heure fin fermeturre echangeur 1.12
* PNI	23/09/14 : ajout lib_cause et lib_secours DEM1097 1.13
* JMG	18/06/15 : ajout h debut et h debut prevu datetime DEM1130 1.14
* JMG	26/11/16 : ajout majeur lot 23 1.15
* JPL	17/02/17 : Prise en compte du champ de complement rappel client (DEM 1192)  1.16
* JMG	20/03/17 : ajout type_cause 1.17
* JMG	19/09/27 : convergence 1.18
* JMG	17/05/17 : personnel travaux 1.19 DEM1282
* JPL	29/06/18 : Prise en compte du nombre de passagers (DEM 1295)  1.20
* JMG	10/10/18 : ajout cause et cle cause LOT25 DEM1301 1.21
* LCL	24/10/18 : correction nom de fonction 1.22
* JMG	26/11/18 : correction cle clause  1.23
* JPL	08/04/19 : Prise en compte de l'indicateur d'écoulement (DEM 1326)  1.24
* JPL	25/09/19 : Prise en compte de l'indicateur de bouchon (DEM 1346)  1.25
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae200.h"

static char *version = " $Id: xzae200.c,v 1.25 2020/06/02 21:22:55 devgfi Exp $ $Revision: 1.25 $ $Date: 2020/06/02 21:22:55 $ ";
	
/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X------------------------------------------------------
* SERVICE RENDU : 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAE200_Lire_Infos_TFM (  XDY_FonctionInt va_FonctionUtilisateur_in,
			XDY_Entier	va_numero_in,
			XDY_Octet      va_cle_in)


/*
*
* PARAMETRES EN ENTREE :
*
* int           va_NumEvt_in
* tinyint       va_CleEvt_in
* XDY_Fonction  va_FonctionUtilisateur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
*  Liste des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR *rpc_name = XZAE200C_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAE200C_NB_PARM_RETURN;
       	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	char vl_origine[XDC_Lon_Nom];
	char vl_autoroute[100];
	char vl_lib_sens[100];
	char vl_duree[100];
	char vl_secours[255];
	char vl_debut[100];
	char vl_localisation[100];
	char vl_neutralisation[100];
	char vl_victimes[100];
	char vl_gravite[100];
	char vl_vehicules[100];
	char vl_chaussee_glissante[100];
	char vl_conseil[200];
	char vl_voies_bloquees[100];
	char vl_groupe_nominal[100];
	char vl_groupe_verbal[100];
	char vl_fin[100];
	char vl_longueur_bouchon[150];
	char vl_periode[100];
	char vl_retour_normale[100];
	char vl_visibilite[100];
	char vl_cause[100];
	char vl_lib_cause[100];
	char vl_lib_secours[100];
	char vl_date_fin[40];
	char vl_objet_fax[100];
	XDY_Booleen vl_prevu;
	XDY_Booleen vl_confirme;
	XDY_Booleen vl_termine;
	XDY_Autoroute vl_numero_autoroute;
	XDY_PR vl_pr;
	XDY_PR	vl_pr_tete;
	XDY_Sens	vl_sens;
	XDY_Octet	vl_pt_car,vl_num_pt_car;
	char		vl_nom_pt_car[30 + 1];
	char		vl_sortie_usager[XDC_Lon_Sortie + 1];
	XDY_Booleen	vl_indic_bifurcation;
	int vl_longueur;
	XDY_Octet vl_BAU,vl_VL,vl_VM1,vl_VM2,vl_VR;
	XDY_Octet vl_VL_I,vl_VM1_I,vl_VM2_I,vl_VR_I;
	XDY_Octet vl_nb_vehicules,vl_type_meteo;
	XDY_Booleen vl_personnalites,vl_agent_escota;
	int vl_valeur_duree;
	int vl_dist_visibilite;
	XDY_Octet vl_type_vehicule,vl_vl,vl_pl;
	XDY_Octet vl_moto,vl_nb_victimes,vl_morts,vl_blesses_graves,vl_car;
	XDY_Booleen vl_matieres_dangereuses,vl_animaux_vivants;
	XDY_Octet vl_remorque;
	XDY_Octet	vl_majeur;
	XDY_Booleen vl_voies_retrecies;
	XDY_Booleen vl_entree1;
	XDY_Booleen vl_entree2;
	XDY_Booleen vl_sortie1;
	XDY_Booleen vl_sortie2;
	XDY_Booleen vl_type_incident;
	XDY_Octet   vl_numero_carburant;
	char 	vl_abrev_pt_car[20];
	XDY_Octet   vl_type_bouchon;
	char	vl_tdp[255];
	XDY_Octet	vl_type_travaux;
	XDY_Octet	vl_nb_manifestants;
	XDY_Booleen vl_violence;
	XDY_Booleen vl_perception_peage;
	char vl_config_voies[9 + 1];
	XDY_Octet vl_datex; /*SAGA*/
	char vl_horaire_debut_prevu[10];
	char vl_horaire_fin_prevue[10];
	char vl_nuit[20];
	XDY_Octet vl_nature_travaux;
	XDY_Octet vl_sous_conc;
	XDY_Octet vl_feu_maitrise;
	XDY_Octet vl_cause_operation;
	int vl_vitesse, vl_vitesse_opposee;
	char vl_zone[100];
	XDY_Eqt	vl_numero_zone;
	char vl_scenario[100];
	char vl_vitesse_transitoire[200];
	XDY_Horodate vl_HDebut, vl_HDebut_prevu;
	char vl_complement_rappel[40];
	XDY_Mot	vl_type_cause;
	XDY_Octet vl_intervention, vl_zone_urbaine;
	XDY_Octet vl_personnel;
	XDY_Octet vl_nb_passagers;
	int vl_num_cause;
	XDY_Octet vl_cle_cause;
	XDY_Octet vl_ecoulement;
	XDY_Octet vl_bouchon;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAE200_Lire_Infos_TFM : evt %d %d",
			va_numero_in,va_cle_in);
			
			
   	/*A
        **  test les parametres d'entree
        */

	if ( va_FonctionUtilisateur_in == NULL)
	{
	      XZST_03EcritureTrace(XZSTC_WARNING,"va_FonctionUtilisateur_in est nulle");
	      return (XDC_NOK);
	}

	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae200: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE200_Lire_Infos_TFM retourne %d", retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae200: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE200_Lire_Infos_TFM retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_numero_in,"@va_numero_fmc"))!= CS_SUCCEED)
	{
    	   	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE150: asql_ctparam_int_input(va_numero_in) a echoue");
	   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE150_Message_Datex retourne %d", retcode);
	   	asql_clean_cmd (connection,cmd);
	   	return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_cle_in,"@va_cle_fmc"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAE150: asql_ctparam_int_input(va_cle_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE150_Message_Datex retourne %d", retcode);
		asql_clean_cmd (connection,cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae200: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE200_Lire_Infos_TFM retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	i=0;
	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)res_type)
				{
					case  CS_ROW_RESULT:
						retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae200: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzae200 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae200 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae200 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE200_Lire_Infos_TFM retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
						   return (XDC_NOK);
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae200: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE200_Lire_Infos_TFM retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
			 			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae200: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE200_Lire_Infos_TFM retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae200: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE200_Lire_Infos_TFM retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);
				}
	}

	
	XZST_03EcritureTrace(XZSTC_INTERFACE, "XZAE200_Lire_Infos_TFM %d lignes", vl_Nb_Ligne_Lue);

	           
		/*B
	         * recup des parametres de retour
	         */
		
		i=0;
		strcpy(vl_autoroute, (char *) vl_Tab_Ligne[i][0].pt_value);
	        strcpy(vl_duree, (char *) vl_Tab_Ligne[i][1].pt_value);
	        strcpy(vl_secours, (char *) vl_Tab_Ligne[i][2].pt_value);
	        strcpy(vl_debut, (char *) vl_Tab_Ligne[i][3].pt_value);
	        strcpy(vl_localisation, (char *) vl_Tab_Ligne[i][4].pt_value);
	        strcpy(vl_neutralisation, (char *) vl_Tab_Ligne[i][5].pt_value);
	        strcpy(vl_victimes, (char *) vl_Tab_Ligne[i][6].pt_value);
	        strcpy(vl_gravite, (char *) vl_Tab_Ligne[i][7].pt_value);
	        strcpy(vl_vehicules, (char *) vl_Tab_Ligne[i][8].pt_value);
	        strcpy(vl_chaussee_glissante, (char *) vl_Tab_Ligne[i][9].pt_value);
	        strcpy(vl_conseil, (char *) vl_Tab_Ligne[i][10].pt_value);
	        strcpy(vl_voies_bloquees, (char *) vl_Tab_Ligne[i][11].pt_value);
	        strcpy(vl_groupe_nominal, (char *) vl_Tab_Ligne[i][12].pt_value);
	        strcpy(vl_groupe_verbal, (char *) vl_Tab_Ligne[i][13].pt_value);
	        strcpy(vl_fin, (char *) vl_Tab_Ligne[i][14].pt_value);
	        strcpy(vl_longueur_bouchon, (char *) vl_Tab_Ligne[i][15].pt_value);
	        strcpy(vl_periode, (char *) vl_Tab_Ligne[i][16].pt_value);
	        strcpy(vl_retour_normale, (char *) vl_Tab_Ligne[i][17].pt_value);
	        strcpy(vl_visibilite, (char *) vl_Tab_Ligne[i][18].pt_value);
	        strcpy(vl_cause, (char *) vl_Tab_Ligne[i][19].pt_value);
		vl_prevu = * (XDY_Booleen *) vl_Tab_Ligne[i][20].pt_value;
		vl_confirme = * (XDY_Booleen *) vl_Tab_Ligne[i][21].pt_value;
		vl_termine = * (XDY_Booleen *) vl_Tab_Ligne[i][22].pt_value;
		vl_numero_autoroute = * (XDY_Autoroute *) vl_Tab_Ligne[i][23].pt_value;
		vl_sens = * (XDY_Sens *) vl_Tab_Ligne[i][24].pt_value;
		vl_pr = * (XDY_PR *) vl_Tab_Ligne[i][25].pt_value;
		vl_pr_tete = * (XDY_PR *) vl_Tab_Ligne[i][26].pt_value;
		vl_num_pt_car = * (XDY_Octet *) vl_Tab_Ligne[i][27].pt_value;
		vl_pt_car = * (XDY_Octet *) vl_Tab_Ligne[i][28].pt_value;
		strcpy(vl_nom_pt_car,(char *) vl_Tab_Ligne[i][29].pt_value);
		vl_longueur = * (int *) vl_Tab_Ligne[i][30].pt_value;
		vl_BAU = * (XDY_Octet *) vl_Tab_Ligne[i][31].pt_value;
		vl_VL = * (XDY_Octet *) vl_Tab_Ligne[i][32].pt_value;
		vl_VM1 = * (XDY_Octet *) vl_Tab_Ligne[i][33].pt_value;
		vl_VM2 = * (XDY_Octet *) vl_Tab_Ligne[i][34].pt_value;
		vl_VR = * (XDY_Octet *) vl_Tab_Ligne[i][35].pt_value;
		strcpy(vl_objet_fax,(char *) vl_Tab_Ligne[i][36].pt_value);
		strcpy(vl_lib_sens,(char *) vl_Tab_Ligne[i][37].pt_value);
		vl_nb_vehicules = * (XDY_Octet *) vl_Tab_Ligne[i][38].pt_value;
		vl_type_meteo = * (XDY_Octet *) vl_Tab_Ligne[i][39].pt_value;
		vl_personnalites = * (XDY_Booleen *) vl_Tab_Ligne[i][40].pt_value;
		vl_agent_escota = * (XDY_Booleen *) vl_Tab_Ligne[i][41].pt_value;
		vl_valeur_duree = * (int *) vl_Tab_Ligne[i][42].pt_value;
		vl_dist_visibilite = * (int *) vl_Tab_Ligne[i][43].pt_value;
		vl_type_vehicule = * (XDY_Octet *) vl_Tab_Ligne[i][44].pt_value;
		vl_vl = * (XDY_Octet *) vl_Tab_Ligne[i][45].pt_value;
		vl_pl = * (XDY_Octet *) vl_Tab_Ligne[i][46].pt_value;
		vl_moto = * (XDY_Octet *) vl_Tab_Ligne[i][47].pt_value;
		vl_nb_victimes = * (XDY_Octet *) vl_Tab_Ligne[i][48].pt_value;
		vl_morts = * (XDY_Octet *) vl_Tab_Ligne[i][49].pt_value;
		vl_blesses_graves = * (XDY_Octet *) vl_Tab_Ligne[i][50].pt_value;
		vl_car = * (XDY_Octet *) vl_Tab_Ligne[i][51].pt_value;
		vl_matieres_dangereuses = * (XDY_Booleen *) vl_Tab_Ligne[i][52].pt_value;
		vl_animaux_vivants = * (XDY_Booleen *) vl_Tab_Ligne[i][53].pt_value;
		vl_VL_I = * (XDY_Octet *) vl_Tab_Ligne[i][54].pt_value;
		vl_VM1_I = * (XDY_Octet *) vl_Tab_Ligne[i][55].pt_value;
		vl_VM2_I = * (XDY_Octet *) vl_Tab_Ligne[i][56].pt_value;
		vl_VR_I = * (XDY_Octet *) vl_Tab_Ligne[i][57].pt_value;
		vl_voies_retrecies = * (XDY_Booleen *) vl_Tab_Ligne[i][58].pt_value;
		vl_entree1 = * (XDY_Booleen *) vl_Tab_Ligne[i][59].pt_value;
		vl_entree2 = * (XDY_Booleen *) vl_Tab_Ligne[i][60].pt_value;
		vl_sortie1 = * (XDY_Booleen *) vl_Tab_Ligne[i][61].pt_value;
		vl_sortie2 = * (XDY_Booleen *) vl_Tab_Ligne[i][62].pt_value;
		vl_type_incident = * (XDY_Booleen *) vl_Tab_Ligne[i][63].pt_value;
		vl_numero_carburant = * (XDY_Octet *) vl_Tab_Ligne[i][64].pt_value;
	        strcpy(vl_abrev_pt_car, (char *) vl_Tab_Ligne[i][65].pt_value);
		vl_type_bouchon = * (XDY_Octet *) vl_Tab_Ligne[i][66].pt_value;
	        strcpy(vl_tdp, (char *) vl_Tab_Ligne[i][67].pt_value);
		vl_type_travaux = * (XDY_Octet *) vl_Tab_Ligne[i][68].pt_value;
		vl_remorque = * (XDY_Octet *) vl_Tab_Ligne[i][69].pt_value;
	        strcpy(vl_date_fin, (char *) vl_Tab_Ligne[i][70].pt_value);
		vl_nb_manifestants = * (XDY_Octet *) vl_Tab_Ligne[i][71].pt_value;
		vl_violence = * (XDY_Booleen *) vl_Tab_Ligne[i][72].pt_value;
		vl_perception_peage = * (XDY_Booleen *) vl_Tab_Ligne[i][73].pt_value;
	        strcpy(vl_config_voies, (char *) vl_Tab_Ligne[i][74].pt_value);
		vl_datex = * (XDY_Octet *) vl_Tab_Ligne[i][75].pt_value; 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "XZAE200_Lire_Infos_TFM %s", (char *) vl_Tab_Ligne[0][76].pt_value);
		strcpy(vl_horaire_debut_prevu,(char *) vl_Tab_Ligne[i][76].pt_value);
		strcpy(vl_horaire_fin_prevue,(char *) vl_Tab_Ligne[i][77].pt_value);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "XZAE200_Lire_Infos_TFM %s", (char *) vl_Tab_Ligne[0][77].pt_value);
		strcpy(vl_nuit,(char *) vl_Tab_Ligne[i][78].pt_value);
		vl_nature_travaux = * (XDY_Octet *) vl_Tab_Ligne[i][79].pt_value;
		vl_sous_conc = * (XDY_Octet *) vl_Tab_Ligne[i][80].pt_value;
		vl_feu_maitrise = * (XDY_Octet *) vl_Tab_Ligne[i][81].pt_value;
		vl_cause_operation = * (XDY_Octet *) vl_Tab_Ligne[i][82].pt_value;

		strcpy(vl_origine,(char *) vl_Tab_Ligne[i][83].pt_value);

		vl_vitesse = * (XDY_Octet *) vl_Tab_Ligne[i][84].pt_value;
		vl_vitesse_opposee = * (XDY_Octet *) vl_Tab_Ligne[i][85].pt_value;
		strcpy(vl_zone,(char *) vl_Tab_Ligne[i][86].pt_value);
		strcpy(vl_scenario,(char *) vl_Tab_Ligne[i][87].pt_value);
		strcpy(vl_vitesse_transitoire,(char *) vl_Tab_Ligne[i][88].pt_value);
		vl_numero_zone = * (XDY_Eqt *) vl_Tab_Ligne[i][89].pt_value;

		strcpy(vl_sortie_usager,(char *) vl_Tab_Ligne[i][90].pt_value);
		vl_indic_bifurcation = * (XDY_Booleen *) vl_Tab_Ligne[i][91].pt_value;
	        strcpy(vl_lib_cause, (char *) vl_Tab_Ligne[i][92].pt_value);
	        strcpy(vl_lib_secours, (char *) vl_Tab_Ligne[i][93].pt_value);
                asql_date_Sybase2Ux(*(CS_DATETIME*) vl_Tab_Ligne[i][94].pt_value,&vl_HDebut);
                asql_date_Sybase2Ux(*(CS_DATETIME*) vl_Tab_Ligne[i][95].pt_value,&vl_HDebut_prevu);
		vl_majeur = * (XDY_Octet *) vl_Tab_Ligne[i][96].pt_value;
		strcpy(vl_complement_rappel, (char *) vl_Tab_Ligne[i][97].pt_value);
		vl_type_cause = * (XDY_Mot *) vl_Tab_Ligne[i][98].pt_value;
		vl_zone_urbaine = * (XDY_Octet *) vl_Tab_Ligne[i][99].pt_value;
		vl_intervention = * (XDY_Octet *) vl_Tab_Ligne[i][100].pt_value;
		vl_personnel = * (XDY_Octet *) vl_Tab_Ligne[i][101].pt_value;
		vl_nb_passagers = * (XDY_Octet *) vl_Tab_Ligne[i][102].pt_value;
		vl_num_cause = * (int *) vl_Tab_Ligne[i][103].pt_value;
		
		vl_cle_cause = * (XDY_Octet *) vl_Tab_Ligne[i][104].pt_value;
		vl_ecoulement = * (XDY_Octet *) vl_Tab_Ligne[i][105].pt_value;
		vl_bouchon = * (XDY_Octet *) vl_Tab_Ligne[i][106].pt_value;

		XZST_03EcritureTrace(XZSTC_INTERFACE, "XZAE200_Lire_Infos_TFM vl_datex = %d", vl_datex);
		XZST_03EcritureTrace(XZSTC_FONCTION, "XZAE200_Lire_Infos_TFM vl_horaire_debut_prevu = %s", vl_horaire_debut_prevu);
		XZST_03EcritureTrace(XZSTC_FONCTION, "XZAE200_Lire_Infos_TFM vl_horaire_fin_prevue = %s", vl_horaire_fin_prevue);
		XZST_03EcritureTrace(XZSTC_FONCTION, "XZAE200_Lire_Infos_TFM vl_hdebut = %lf", vl_HDebut);
		XZST_03EcritureTrace(XZSTC_FONCTION, "XZAE200_Lire_Infos_TFM vl_hdebut_prevu = %lf", vl_HDebut_prevu);
		XZST_03EcritureTrace(XZSTC_FONCTION, "XZAE200_Lire_Infos_TFM vl_intervention = %d", vl_intervention);
		XZST_03EcritureTrace(XZSTC_FONCTION, "XZAE200_Lire_Infos_TFM vl_type_cause = %d", vl_type_cause);
		XZST_03EcritureTrace(XZSTC_FONCTION, "XZAE200_Lire_Infos_TFM cause = %d/%d", vl_num_cause, vl_cle_cause);

	        /*B
		 ** Appel de la fonction utilisateur FonctionUtilisateur
		 */
		  
		if (( retcode = (*va_FonctionUtilisateur_in) (vl_autoroute,
							      vl_duree,
							      vl_secours,
							      vl_debut,
							      vl_localisation,
							      vl_neutralisation,
							      vl_victimes,
							      vl_gravite,
							      vl_vehicules,
							      vl_chaussee_glissante,
							      vl_conseil,
							      vl_voies_bloquees,
							      vl_groupe_nominal,
							      vl_groupe_verbal,
							      vl_fin,
							      vl_longueur_bouchon,
							      vl_periode,
							      vl_retour_normale,
							      vl_visibilite,
							      vl_cause,
							      vl_prevu,
							      vl_confirme,
							      vl_termine,
							      vl_numero_autoroute,
							      vl_sens,
							      vl_pr,
							      vl_pr_tete,
							      vl_pt_car,
							      vl_num_pt_car,
							      vl_nom_pt_car,
							      vl_longueur,
							      vl_BAU,
							      vl_VL,
							      vl_VM1,
							      vl_VM2,
							      vl_VR,
							      vl_objet_fax,
							      vl_lib_sens,
							      vl_nb_vehicules,
							      vl_type_meteo,
							      vl_personnalites,
							      vl_agent_escota,
							      vl_valeur_duree,
							      vl_dist_visibilite,
							      vl_type_vehicule,
							      vl_vl,
							      vl_pl,
							      vl_moto,
							      vl_nb_victimes,
							      vl_morts,
							      vl_blesses_graves,
							      vl_car,
							      vl_matieres_dangereuses,
							      vl_animaux_vivants,
							      vl_VL_I,
							      vl_VM1_I,
							      vl_VM2_I,
							      vl_VR_I,
							      vl_voies_retrecies,
							      vl_entree1,
							      vl_entree2,
							      vl_sortie1,
							      vl_sortie2,
							      vl_type_incident,
							      vl_numero_carburant,
							      vl_abrev_pt_car,
							      vl_type_bouchon,
							      vl_tdp,
							      vl_type_travaux,
							      vl_remorque,
							      vl_date_fin,
							      vl_nb_manifestants,
							      vl_violence,
							      vl_perception_peage,
							      vl_config_voies,
							      vl_datex,			/*SAGA*/
							      vl_horaire_debut_prevu,
							      vl_horaire_fin_prevue,
							      vl_nuit,
							      vl_nature_travaux,
							      vl_sous_conc,
							      vl_feu_maitrise,
							      vl_cause_operation,
								  vl_origine,
								  vl_vitesse,
								  vl_vitesse_opposee,
								  vl_zone,
								  vl_scenario,
								  vl_vitesse_transitoire,
								  vl_numero_zone,
							      vl_sortie_usager,
							      vl_indic_bifurcation,
							      vl_lib_cause,
							      vl_lib_secours,
							      vl_HDebut,
							      vl_HDebut_prevu,
							      vl_majeur,
							      vl_complement_rappel,
							      vl_type_cause,
							      vl_zone_urbaine,
							      vl_intervention,
							      vl_personnel,
							      vl_nb_passagers,
							      vl_num_cause,vl_cle_cause,
							      vl_ecoulement,
							      vl_bouchon)
							!= XDC_OK ))
		{
		        XZST_03EcritureTrace(XZSTC_FATAL,"appel de va_FonctionUtilisateur_in s'est mal deroule");
		        asql_clean_cmd(connection, cmd);
			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
			return (XDC_NOK);
		}	

	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE200_Lire_Infos_TFM retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
