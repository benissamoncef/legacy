/*E*/
/*Fichier : $Id: aeqa_calc.c,v 1.4 1996/12/05 18:25:24 niepceron Exp $      Release : $Revision: 1.4 $        Date : $Date: 1996/12/05 18:25:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE AEQA * FICHIER aeqa_calc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* calcule les equation d'alerte evt
*********************************
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	25 jan 1995	: creation (1.1)
* gaborit	03 fev 1995	: modif proc (1.2)
* gaborit	03 fev 1995	: modif type typefmc (1.3)
* gaborit	20 nov 1996	: Ajout d'un arg ds xdea01_evaluer (1.3)
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "aeqa_calc.h"

/* definitions de constantes */

#define AEQAC_NB_VAR	51
#define AEQAC_PROC	"XZAA;10"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: aeqa_calc.c,v 1.4 1996/12/05 18:25:24 niepceron Exp $ : aeqa_calc" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	recoit le message XDM_AEQA, le decode, recherche
*	les infos complementaires en base et calcule l'equation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void aeqa_calc 	(	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: aeqa_calc.c,v 1.4 1996/12/05 18:25:24 niepceron Exp $ : aeqa_calc" ;

	/* Tableau des variables de l'equation */
	
	XDEAT_VAR	pl_variables[AEQAC_NB_VAR]=
				{
					{"VR",0.0},	/* 1 */
					{"VM2",0.0},
					{"VM1",0.0},
					{"VL",0.0},
					{"BAU",0.0},	/* 5 */
					{"VR_I",0.0},
					{"VM2_I",0.0},
					{"VM1_I",0.0},
					{"VL_I",0.0},
					{"BAU_I",0.0},	/* 10 */
					{"DUREE",0.0},
					{"LONGUEUR",0.0},
					{"ACCIDENT",0.0},
					{"DELESTAGE",0.0},
					{"DEVIATION",0.0},	/* 15 */
					{"OPERATEUR",0.0},
					{"METEO",0.0},
					{"CONTRESENS",0.0},
					{"VEH_ARRETE",0.0},
					{"VEH_PANNE",0.0},	/* 20 */
					{"VEH_FEU",0.0},
					{"CIRQUE",0.0},
					{"CONVOI",0.0},
					{"MILITAIRE",0.0},
					{"VEH_LENT",0.0},	/* 25 */
					{"MANIFESTATION",0.0},
					{"GREVE",0.0},
					{"ESCARGOT",0.0},
					{"BASCULEMENT",0.0},
					{"TRAVAUX",0.0},	/* 30 */
					{"SS_CONCESS",0.0},
					{"QUEUE_BOUCHON",0.0},
					{"ECHANGEUR",0.0},
					{"EBOULEMENT",0.0},
					{"INCENDIE_VEGETATION",0.0},	/* 35 */
					{"INONDATION",0.0},
					{"OBSTACLE",0.0},
					{"CHAUSSEE_DETERIOREE",0.0},
					{"HUILE",0.0},
					{"ANIMAL",0.0},	/* 40 */
					{"FRONTIERE",0.0},
					{"ALTERCATION",0.0},
					{"AGRESSION",0.0},
					{"MALAISE",0.0},
					{"HORS_RESEAU",0.0},	/* 45 */
					{"AIRE",0.0},
					{"SALAGE",0.0},
					{"HOLD_UP",0.0},
					{"TETE_BOUCHON",0.0},
					{"PIETON",0.0},	/* 50 */
					{"AUTRE",0.0}
				};

	char*	pl_tab_fichier[XDF_Nb_Equation_Evt]=XDF_Equation_Evt;
	char	pl_nom_fichier[200];
	FILE*	pl_fichier;
	int	i;
	
	/* Variables locales pour messages RTWks */
#ifndef _TIBCO_EMS
	T_CHAR		vl_type;
	T_INT4		vl_VR;
	T_INT4		vl_VM2;
	T_INT4		vl_VM1;
	T_INT4		vl_VL;
	T_INT4		vl_BAU;
	T_INT4		vl_VR_I;
	T_INT4		vl_VM2_I;
	T_INT4		vl_VM1_I;
	T_INT4		vl_VL_I;
	T_INT4		vl_BAU_I;
	T_REAL8		vl_duree;
	T_REAL8		vl_longueur;
	T_INT2		vl_machine;
	T_REAL8		vl_horodate;
#else
	XDY_Octet	vl_type;
	XDY_Entier	vl_VR;
	XDY_Entier	vl_VM2;
	XDY_Entier	vl_VM1;
	XDY_Entier	vl_VL;
	XDY_Entier	vl_BAU;
	XDY_Entier	vl_VR_I;
	XDY_Entier	vl_VM2_I;
	XDY_Entier	vl_VM1_I;
	XDY_Entier	vl_VL_I;
	XDY_Entier	vl_BAU_I;
	XDY_Horodate	vl_duree;
	XDY_Horodate	vl_longueur;
	XDY_Mot		vl_machine;
	XDY_Horodate	vl_horodate;
#endif
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = AEQAC_PROC;		/*Nom de la SP a appeler */
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans aeqa_dir.
								*/
	CS_COMMAND	*pl_cmd;							
	
	/* Variables Specifiques Open Client */														
        int             	vl_Nb_Ligne_Lue=0;
        int             	vl_Nb_Col=0;
        tg_row_result   	vl_Tab_Ligne;
        int			vl_indice;
        	
	XDY_Entier 	vl_SPstatus=XDC_OK;			/* Contient le status de la SP */
	XDY_Booleen	vl_Bool	= XDC_VRAI;			/* Si vl_Bool	= XDC_VRAI la SP a trouve des donnes valides sinon XDC_FAUX*/
	
	/* Variables intermediaires pour les besoins de SYBASE */
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s : Debut d execution",version);
	
	/*A
	** Recuperation du contenu du message XDM_AEQA
	*/ 
	if (!TipcMsgRead(pa_Data->msg,	T_IPC_FT_CHAR,	&vl_type,
					T_IPC_FT_INT4,	&vl_VR,
					T_IPC_FT_INT4,	&vl_VM2,
					T_IPC_FT_INT4,	&vl_VM1,
					T_IPC_FT_INT4,	&vl_VL,
					T_IPC_FT_INT4,	&vl_BAU,
					T_IPC_FT_INT4,	&vl_VR_I,
					T_IPC_FT_INT4,	&vl_VM2_I,
					T_IPC_FT_INT4,	&vl_VM1_I,
					T_IPC_FT_INT4,	&vl_VL_I,
					T_IPC_FT_INT4,	&vl_BAU_I,
					T_IPC_FT_REAL8,	&vl_duree,
					T_IPC_FT_REAL8,	&vl_longueur,
					T_IPC_FT_REAL8,	&vl_horodate,
					T_IPC_FT_INT2,	&vl_machine,
					NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING ," %s : TipcMsgRead a echoue",version);	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s Sortie",version);
		return;
	}

	/*A affectation des valeurs recues a la structure des variables */
	pl_variables[0].valeur=(double)vl_VR;
	pl_variables[1].valeur=(double)vl_VM2;
	pl_variables[2].valeur=(double)vl_VM1;
	pl_variables[3].valeur=(double)vl_VL;
	pl_variables[4].valeur=(double)vl_BAU;
	pl_variables[5].valeur=(double)vl_VR_I;
	pl_variables[6].valeur=(double)vl_VM2_I;
	pl_variables[7].valeur=(double)vl_VM1_I;
	pl_variables[8].valeur=(double)vl_VL_I;
	pl_variables[9].valeur=(double)vl_BAU_I;
	pl_variables[10].valeur=(double)vl_duree;
	pl_variables[11].valeur=(double)vl_longueur;
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"%s : ct_cmd_alloc() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s Sortie",version);		
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_command() a echoue avec le code %d",version,vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s : sortie",version);
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s : ct_send() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s sort ",version);		
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"%s: ROW RESULTS",version);
						break;

					case  CS_PARAM_RESULT:
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd,&vl_SPstatus );
						XZST_03EcritureTrace(XZSTC_INFO,"%s : CS_STATUS_RESULT",version);
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_* a echoue",version);
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s : sortie",version);
                                }

	
				break;


				case CS_MSG_RESULT:
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_res_info(msgtype) a echoue",version);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"%s : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n",version, vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"%s : CS_CMD_SUCCEED \n",version);
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"%s : CS_CMD_DONE \n",version);
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne CS_CMD_FAIL.",version);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s : sortie",version);
					asql_clean_cmd (pl_connection,pl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne un resultat de type inattendu",version);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s : sortie",version);
					asql_clean_cmd (pl_connection,pl_cmd);
					return;
					break;
				}
		}
	
	if(vl_SPstatus == XDC_OK)
	
	{
		vl_Bool	= XDC_VRAI;
		XZST_03EcritureTrace(XZSTC_INFO,"%s : SP a retourne des donnees valides",version);
		for (vl_indice = 0 ; vl_indice<vl_Nb_Ligne_Lue;vl_indice++)
		{
			i=(XDY_Entier)(* (XDY_Mot   *)(vl_Tab_Ligne[vl_indice][0].pt_value));
			i+=11; /* pour positionner au bon endroit dans le tableau */
			if ((i>11)&&(i<AEQAC_NB_VAR))
			{
				pl_variables[i].valeur=(double) (* (XDY_Entier   *)( vl_Tab_Ligne[vl_indice][1].pt_value));
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s PROCEDURE retourne type d'evenement incorrect",version);
			}
		}	
	}
	else
	{
		vl_Bool	= XDC_FAUX;
		XZST_03EcritureTrace(XZSTC_WARNING,"%s : SP a retourne NOK",version);
	}
		
	/*A
        ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
        */

        if ((vl_retcode = asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne)) != XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
        }
        asql_clean_cmd (pl_connection,pl_cmd);
        if ((vl_type<1)||(vl_type>XDF_Nb_Equation_Evt))
        {
        	XZST_03EcritureTrace(XZSTC_WARNING,"%s type d'evenement incorrect",version);
        	vl_Bool = XDC_FAUX;
        }
        if (vl_Bool==XDC_VRAI)
        /*A on ouvre le fichier et on appelle la fonction de calcul */
        {
		strcpy(pl_nom_fichier,XDC_PATH_EQA);
		strcat(pl_nom_fichier,"/");
		strcat(pl_nom_fichier,pl_tab_fichier[(int)vl_type-1]);
		pl_fichier=fopen(pl_nom_fichier,"r");
		
		if (pl_fichier==NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s echec ouverture %s",version,pl_nom_fichier);
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_INFO,"%s succes ouverture %s",version,pl_nom_fichier);
			if (xdea01_evaluer(	pl_fichier,
						pl_variables,
						AEQAC_NB_VAR,
						vl_horodate,
						XDC_EQT_MAC,
						vl_machine,
						XDC_NON,
						NULL)!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s echec de xdea01_evaluer",version);
			}
			fclose(pl_fichier);
		}
		
        }
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : %s: Fin d execution ",version);
	return;
}

