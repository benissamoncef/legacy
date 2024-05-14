/*E*/
/*Fichier : @(#)acli_tcl.c	1.21      Release : 1.21        Date : 10/05/09
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE TCL * FICHIER acli_tcl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*	atcl22 : Fonction declenchee sur reception de message XDM_AT_TUBE.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;22 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.  
**********************
*	atcl23 : Fonction declenchee sur reception de message XDM_AT_echangeur.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour. 
**********************
*	atcl24 : Fonction declenchee sur reception de message XDM_AT_NAV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;24 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour. 
********************** 
*	atcl25 : Fonction declenchee sur reception de message XDM_AT_PMV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;25 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
**********************
*	atcl40 : Fonction declenchee sur reception de message XDM_AT_SIG.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;40 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.  
**********************
*	atcl41 : Fonction declenchee sur reception de message XDM_AT_ECL.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;41 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour. 
**********************
*	atcl42 : Fonction declenchee sur reception de message XDM_AT_VEN.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;42 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour. 
********************** 
*	atcl43 : Fonction declenchee sur reception de message XDM_AT_ENR.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;43 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
********************** 
*	atcl44 : Fonction declenchee sur reception de message XDM_AT_INC.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;44 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
**********************
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	05 Oct 1994	: Creation
* mercier       10 Oct 1994	: Creation de atcl23,atcl23,atcl24,atcl25
*************
* MERCIER	01 Fev	1995	: Correction Num_Action en ResultaAction	V 1.4
* TORREGROSSA	17 Fev	1995	: Ajout atcl26 V 1.5
* TORREGROSSA	20 Fev	1995	: Modif atcl26 V 1.6
* TORREGROSSA	20 Fev	1995	: Modif atcl27 V 1.7 
*                                              a V 1.12
* GABORIT	04 Avr 1995	: Modif null atcl27 (1.13)
* GABORIT	19 Jun 1995	: modif test vl_retcode dans atcl26 (trace Warning erronnee) (1.15)
* GABORIT       22 Jun 1995	: modif atcl27 (pb de liberation malloc) (1.16)
* Guilhou	27 aut 1996	: modif atcl22,23,24,25 pour num action en int (DEM/1172) (1.17)
* TORREGROSSA	29 Jan 1998	: Ajout atcl28 (DEM/1539) (1.18)
* JMG		11/10/05	: ajout PMVA BAF BAD 1.19
* AAZ       27/08/2007  : Ajout SIG, ECL, VEN, ENR et INC
* JMG		15/09/09	: FACTIS DEM914
* VR		12/12/11 	:	Ajout PR					V(DEM/1016)
* JMG		29/09/14 : inhibition maj bouton alarmes dans tcl27 FACTIS
* JMG		26/06/16 : ajout SAGA 1.24
* LCL		28/02/18 : Ajout controleurs de feux CTRL_FEUX 1.25 DEM1284
* ABE		17/12/20 : Ajout atcl70 pour IMU DEM-SAE155
* ABE		06/04/21 : Ajout atcl80 pour SONO DEM-SAE244 V1.29
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* LCL		14/01/22	: correction trace DatagroupeIhm dans atcl27 SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_tcl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_tcl" ;

/* declaration de fonctions internes */
int acli_Nb_Critiques   ( int *     );
/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	recoit le message RTWks XDM_AT_TUBE, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP22.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl22 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_TUBE.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;22 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl22" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* entier	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL22_RPC_NAME;	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl22 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_TUBE
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl22 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl22 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl22 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl22: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl22 : \nasql_ctparam_smallint_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl22 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl22 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl22 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl22 : \nasql_ctparam_bit_input(va_ResultatAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl22 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl22: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl22 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl22 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl22 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);

                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl22: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl22 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl22 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl22 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl22: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl22: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl22 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl22 : Fin d execution ");
        
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu d une action dans la base
*
*	atcl23 : recoit le message RTWks XDM_AT_echangeur, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP23.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl23 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_echangeur.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl23" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL23_RPC_NAME;	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl23 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_echangeur
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, 
				T_IPC_FT_REAL8,&vl_Horodate_double, 
				T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl23 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl23 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl23 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl23: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl23 : \nasql_ctparam_smallint_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl23 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl23 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl23 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 : sortie");
	  	
		asql_clean_cmd(pl_connection, vl_cmd);
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl23 : \nasql_ctparam_bit_input(va_ResultatAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl23 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl23: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl23 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl23 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl23 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 : sortie");
                                  	asql_clean_cmd(pl_connection, vl_cmd);
      
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl23: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl23 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl23 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl23 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl23: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl23: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl23 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl23 : Fin d execution ");
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_NAV, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP24.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl24 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_NAV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;24 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl24" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL24_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl24 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_NAV
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl24 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl24 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl24 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl24: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl24 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl24 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl24 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl24 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl24 : \nasql_ctparam_bit_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl24 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl24: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl24 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl24 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl24 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl24: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl24 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl24 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl24 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl24: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 : sortie");
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl24: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl24 : Fin d execution ");
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_PMV, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP25.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl25 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_PMV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;24 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl25" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL25_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl25 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_TUBE
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl25 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl25 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl25 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl25 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl25 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl25 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl25 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
	  	
		asql_clean_cmd(pl_connection, vl_cmd);
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl25 : \nasql_ctparam_bit_input(vl_ResultatAction) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl25 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl25: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl25 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl25 : Fin d execution ");
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Retourne l'etat des equipements pour equest.
*
*	recoit le message RTWks XDM_AT_INI_EQT, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP26.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl26 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_INI_EQT.
*	Le message RTwks doit contenir un XDY_Octet,XDY_Entier,XDY_NomMachine.
*	On appele XZAT;26 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl26" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Mot		vl_Type;		/* smallint	*/
	XDY_Octet	vl_TypeChoisi;		/* short	*/
	XDY_Entier 	vl_Etats;		/* entier 	*/
#ifndef _TIBCO_EMS
	T_STR		vl_LocalTechnique; 	/* char		*/
#else
	XDY_NomMachine		vl_LocalTechnique; 	/* char		*/
#endif
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL26_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	tg_row_result 	vl_Tab_Ligne;
	int		i =0;
	int		j =0;
	XDY_Entier	vl_SPstatus;

	/*A
	** Definition des variables locales 
	*/	
        XDY_Entier	vl_Eqt, vl_Etat; 
        XDY_Octet	vl_TypeEqt; 
        XDY_NomMachine	vl_NomEqt; 
        XDY_Entier	vl_Code; 
        XZSCT_NomSite	vl_Site; 
		
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl26 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_INI_EQT
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2, &vl_Type, T_IPC_FT_INT4,&vl_Etats, T_IPC_FT_STR, &vl_LocalTechnique, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl26 a recu Type = %d\n Etats = %d\n LocalTechnique = %s\n",
									vl_Type,
									vl_Etats,
									vl_LocalTechnique);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl26 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl26 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl26: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	vl_TypeChoisi = vl_Type;
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_TypeChoisi,"@va_Type_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl26 : \nasql_ctparam_smallint_input(va_Type_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
        
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Etats,"@va_Etats_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl26 : asql_ct_param_int_input(Etats) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 : sortie");
	  	
		asql_clean_cmd(pl_connection, vl_cmd);
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_LocalTechnique,"@va_LocalTechnique_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl26 : \nasql_ctparam_char_input(NULL) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl26 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_row_result(vl_cmd, &vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl26: ROW RESULTS nb lignes:%d ", vl_Nb_Ligne_Lue);

						for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						{
						 	vl_TypeEqt   = *(XDY_Octet  *) vl_Tab_Ligne[i][0].pt_value;
						 	vl_Eqt = *(XDY_Mot  *) vl_Tab_Ligne[i][1].pt_value;
						   	strcpy(vl_NomEqt, 
								vl_Tab_Ligne[i][2].pt_value);
						   	vl_Etat   = *(XDY_Entier *) vl_Tab_Ligne[i][3].pt_value;
						   	strcpy (vl_Site, vl_Tab_Ligne[i][4].pt_value);
						   	

							if (vl_Etat & XDC_EQT_HS)
								vl_Code = XDC_EQT_HORS_SRV ;
							else	vl_Code = XDC_EQT_EN_SRV;
							vl_retcode=XZEC03_Mise_EnHorsSrv_Eqt(vl_Site, vl_LocalTechnique, (int)vl_TypeEqt, (int)vl_Eqt, vl_Code);

							if (vl_retcode!=XDC_OK)
								XZST_03EcritureTrace(XZSTC_WARNING,"atcl26 : XZEC03_Mise_EnHorsSrv_Eqt retourne XDC_NOK");

							if (vl_Code != XDC_EQT_HORS_SRV)
							{
								if (vl_Etat & XDC_EQT_DESACTIVE)
									vl_Code = XDC_EQT_INACTIF ;
								else vl_Code = XDC_EQT_ACTIF;
								vl_retcode=XZEC04_Activ_Desactiv_Eqt(vl_Site, vl_LocalTechnique, (int)vl_TypeEqt, (int)vl_Eqt, vl_Code);
								if (vl_retcode!=XDC_OK)
									XZST_03EcritureTrace(XZSTC_WARNING,"atcl26 : XZEC04_Activ_Desactiv_Eqt retourne XDC_NOK");
							}
						}

						vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

                                                if (vl_retcode != XDC_OK)
                                                {
                                                        XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
                                                        asql_clean_cmd(pl_connection, vl_cmd);
                                                        return ;
                                                }
						vl_retcode = CS_SUCCEED;
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl26 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd, &vl_SPstatus );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl26 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl26 : LE RESULTAT de STATUS DE LA PROCEDURE STOCKEE = %ld", vl_SPstatus);
						if (vl_SPstatus != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : atcl26  retourne XDC_NOK");
                                        		asql_clean_cmd(pl_connection, vl_cmd);
							return ;
						}
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl26 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl26: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl26 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl26 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl26 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl26: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl26: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl26 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl26 : Fin d execution ");
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Retourne l'etat des equipements pour ihm synoptique
*
*	recoit le message RTWks XDM_AT_INI_IHM, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP27.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl27 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_INI_IHM.
*	Le message RTwks doit contenir un XDY_Octet,XDY_Entier,XDY_Datagroup.
*	On appele XZAT;26 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl27" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Mot		vl_Type;		/* smallint	*/
	XDY_Octet	vl_TypeChoisi;		/* short	*/
	XDY_Entier 	vl_Etats;		/* entier 	*/
#ifndef _TIBCO_EMS
        T_STR           vl_DatagroupeIhm;
#else
        XDY_Datagroup           vl_DatagroupeIhm;
#endif
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL27_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	tg_row_result 	vl_Tab_Ligne;
	int		i =0;
	int		j =0;
	int		vl_nombre;
	XDY_Entier	vl_SPstatus;

	/*A
	** Definition des variables locales 
	*/	
	XZIST_Etat_Eqt	*vl_Eqts;
		
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl27 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_INI_IHM
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2, &vl_Type, T_IPC_FT_INT4,&vl_Etats, T_IPC_FT_STR, &vl_DatagroupeIhm, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl27 a recu Type = %d\n Etats = %d\n DatagroupeIhm = %s\n",
									vl_Type,
									vl_Etats,
									vl_DatagroupeIhm);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl27 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 Sortie");
		return;	 
	}


#if 0
	if (vl_Type==XDC_EQT_PMV)  /*pour ne le faire qu une fois*/ {
           XZST_03EcritureTrace(XZSTC_INFO,"atcl27 : appel acli_Nb_Critiques");
          /*init de l etat du bouton des alarmes critiques*/
          /*appel xzam12sp.prc*/
          acli_Nb_Critiques(&vl_nombre);
          XZST_03EcritureTrace(XZSTC_INFO,"atcl27 : nombre %d",vl_nombre);
          /*appel XZIS01*/
          XZIS01_Positionner_Indicateur_Alarme(vl_nombre>0 ? XDC_VRAI : 0);
        }
#endif

	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl27 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl27: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	vl_TypeChoisi = vl_Type;
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_TypeChoisi,"@va_Type_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl27 : \nasql_ctparam_smallint_input(va_Type_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
        
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Etats,"@va_Etats_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl27 : asql_ct_param_int_input(Etats) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 : sortie");
	  	
		asql_clean_cmd(pl_connection, vl_cmd);
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)" ","@va_LocalTechnique_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl27 : \nasql_ctparam_char_input(" ") a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl27 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_row_result(vl_cmd, &vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl27: ROW RESULTS nb lignes:%d ", vl_Nb_Ligne_Lue);
						if (vl_Nb_Ligne_Lue!=0)
						{
							vl_Eqts = (XZIST_Etat_Eqt *) malloc(vl_Nb_Ligne_Lue * sizeof(XZIST_Etat_Eqt));
							if (vl_Eqts == NULL)
							{
								XZST_03EcritureTrace(XZSTC_WARNING," atcl27: allocation memoire a echouee ");
                                        			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
								asql_clean_cmd(pl_connection, vl_cmd);
								return ;
							}
						}
						else asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

						for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						{
						 	vl_Eqts[i].type   = *(XDY_Octet  *) vl_Tab_Ligne[i][0].pt_value;
						 	vl_Eqts[i].numero = *(XDY_Mot  *) vl_Tab_Ligne[i][1].pt_value;
						   	vl_Eqts[i].etat   = *(XDY_Entier *) vl_Tab_Ligne[i][3].pt_value;
						}

						if (vl_Nb_Ligne_Lue!=0)
						{
							vl_retcode=XZIS02_Init_Etat_Eqt(vl_DatagroupeIhm, vl_Eqts, vl_Nb_Ligne_Lue);
							free( vl_Eqts);
	
							if (vl_retcode!=XDC_OK)
								XZST_03EcritureTrace(XZSTC_WARNING,"atcl27 : XZIS02_Init_Etat_Eqt retourne XDC_NOK");
							vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	                                                if (vl_retcode != XDC_OK)
	                                                {
	                                                        XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
	                                                        asql_clean_cmd(pl_connection, vl_cmd);
	                                                        return ;
	                                                }
                                                }
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl27 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd, &vl_SPstatus );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl27 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl27 : LE RESULTAT de STATUS DE LA PROCEDURE STOCKEE = %ld", vl_SPstatus);
						if (vl_SPstatus != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : atcl27  retourne XDC_NOK");
                                        		asql_clean_cmd(pl_connection, vl_cmd);
							return ;
						}
						break;
				}
				
				if ((vl_retcode != CS_SUCCEED) && (vl_res_type != CS_ROW_RESULT))
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl27 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl27: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl27 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl27 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl27 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl27: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl27: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl27 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl27 : Fin d execution ");
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_PAL, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP27.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl28 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_PAL.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;27 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 11/04/05 : acli_atcl28" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL28_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl28 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_PAL
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl28 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl28 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl28 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl28: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl28 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl28 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl28 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl28 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl28 : \nasql_ctparam_bit_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl28 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl24 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl28: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl28 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl28 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl28 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl28: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl28 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl28 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl28 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl28: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 : sortie");
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl28: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl28 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl28 : Fin d execution ");
        
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_PMVA, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP250.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl250 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_PMV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;24 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl250" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL250_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : atcl250 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_TUBE
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : atcl250 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl250 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl250 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl250: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl250 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl250 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl250 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl250 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 : sortie");
	  	
		asql_clean_cmd(pl_connection, vl_cmd);
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl250 : \nasql_ctparam_bit_input(vl_ResultatAction) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl250 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl250: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl250 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl250 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl250 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl250: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl250 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl250 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl250: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl250: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl250 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl250 : Fin d execution ");
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_PRV, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP51.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl51 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_NAV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;24 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl51" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL51_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl51 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_PRV
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : atcl51 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl51 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl51 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl51: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl51 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl51 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl51 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl51 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl51 : \nasql_ctparam_bit_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl51 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl51: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl51 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl51 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl51 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl51: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl51 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl51 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl51 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl51: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 : sortie");
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl51: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl51 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl51 : Fin d execution ");
        
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_CFE, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP61.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl60 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_CFE.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT530 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl60" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL60_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl60 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_PRV
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : atcl60 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl60 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl60 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl60: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl60 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl60 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl60 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl60 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl60 : \nasql_ctparam_bit_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl60 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl60: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl60 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl60 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl60 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl60: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl60 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl60 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl60 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl60: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 : sortie");
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl60: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl60 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl60 : Fin d execution ");
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_IMU, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP61.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl70 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_IMU.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT530 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl70" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL70_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl70 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_PRV
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : atcl70 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl70 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl70 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl70: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl70 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl70 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl70 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl70 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl70 : \nasql_ctparam_bit_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl70 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl70: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl70 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl70 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl70 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl70: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl70 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl70 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl70 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl70: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 : sortie");
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl70: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl70 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl70 : Fin d execution ");
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_SONO, recupere les informations
*	les met en forme Sybase et appelle la XZAT550 .
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl80 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_SONO.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT550 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl80" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	XDY_Mot		vl_Nb_PAUErreur;
	XDY_Mot		vl_Nb_PAUPilotes;
	XDY_Octet	vl_temp;
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL80_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl80 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_PRV
	*/ 
	if (TipcMsgRead(pa_Data->msg,
		T_IPC_FT_INT4, &vl_Num_Action, 
		T_IPC_FT_REAL8,&vl_Horodate_double, 
		T_IPC_FT_CHAR, &vl_ResultatAction,
		T_IPC_FT_INT2, &vl_Nb_PAUErreur, 
		T_IPC_FT_INT2, &vl_Nb_PAUPilotes,
		 NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : atcl80 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n va_Nb_PAUErreur = %d\n va_Nb_PAUPilotes=%d",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction,
									vl_Nb_PAUErreur, 
									vl_Nb_PAUPilotes);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl80 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl80 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl80: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl80 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl80 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl80 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl80 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl80 : \nasql_ctparam_bit_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	vl_temp=vl_Nb_PAUErreur;
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_temp,"@va_Nb_PAUErreur_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl80 : \nasql_ctparam_tinyint_input(va_Nb_PAUErreur) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	vl_temp=vl_Nb_PAUPilotes;
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_temp,"@va_Nb_PAUPilotes_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl80 : \nasql_ctparam_tinyint_input(va_Nb_PAUPilotes) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl80 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl80: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl80 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl80 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl80 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl80: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl80 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl80 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl80 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl80: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl80: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl80 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl80 : Fin d execution ");
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_BAF, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP30.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl30 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_PMVA.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;30 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl30" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL30_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl30 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_TUBE
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : atcl30 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl30 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl30 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl30 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl30 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl30: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl30 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl30 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl30 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl30 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl30 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl30 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl30 : sortie");
	  	
		asql_clean_cmd(pl_connection, vl_cmd);
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl30 : \nasql_ctparam_bit_input(vl_ResultatAction) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl30 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl30 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl30 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl30: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl25 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl25 : Fin d execution ");
        
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_BAD, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP30.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl31 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_PMVA.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;31 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl31" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL31_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl31 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_TUBE
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl31 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl31 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl31 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl31 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl31 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl31: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl31 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl31 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl31 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl31 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl31 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl31 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl31 : sortie");
	  	
		asql_clean_cmd(pl_connection, vl_cmd);
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl31 : \nasql_ctparam_bit_input(vl_ResultatAction) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl31 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl31 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl31 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl31: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl25 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl25 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl25: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl25 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl25 : Fin d execution ");
        
}
/* AAZ 27/08/2007 */

void atcl40 (	T_IPC_CONN                  va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
        		T_CB_ARG                    pa_ARG) 
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
*	Fonction declenchee sur reception de message XDM_AT_SIG.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;40 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl40" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier      vl_Num_Action;		/* entier	*/
	XDY_Horodate    vl_Horodate_double;	/* double 	*/
	XDY_Booleen     vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL40_RPC_NAME;	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl40 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_SIG
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl40 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl40 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl40 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl40: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl40 : \nasql_ctparam_smallint_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl40 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl40 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl40 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl40 : \nasql_ctparam_bit_input(va_ResultatAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl40 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl40: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl40 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl40 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl40 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);

                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl40: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl40 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl40 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl40 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl40: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl40: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl40 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl40 : Fin d execution ");
        
}


void atcl41 (	T_IPC_CONN                  va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
        		T_CB_ARG                    pa_ARG) 
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
*	Fonction declenchee sur reception de message XDM_AT_ECL.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;41 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl41" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier      vl_Num_Action;		/* entier	*/
	XDY_Horodate    vl_Horodate_double;	/* double 	*/
	XDY_Booleen     vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL41_RPC_NAME;	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl41 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_ECL
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl41 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl41 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl41 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl41: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl41 : \nasql_ctparam_smallint_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl41 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl41 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl41 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl41 : \nasql_ctparam_bit_input(va_ResultatAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl41 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl41: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl41 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl41 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl41 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);

                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl41: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl41 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl41 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl41 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl41: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl41: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl41 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl41 : Fin d execution ");
        
}


void atcl42 (	T_IPC_CONN                  va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
        		T_CB_ARG                    pa_ARG) 
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
*	Fonction declenchee sur reception de message XDM_AT_VEN.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;42 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl42" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier      vl_Num_Action;		/* entier	*/
	XDY_Horodate    vl_Horodate_double;	/* double 	*/
	XDY_Booleen     vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL42_RPC_NAME;	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl42 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_VEN
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl42 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl42 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl42 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl42: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl42 : \nasql_ctparam_smallint_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl42 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl42 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl42 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl42 : \nasql_ctparam_bit_input(va_ResultatAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl42 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl42: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl42 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl42 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl42 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);

                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl42: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl42 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl42 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl42 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl42: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl42: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl42 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl42 : Fin d execution ");
        
}


void atcl43 (	T_IPC_CONN                  va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
        		T_CB_ARG                    pa_ARG) 
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
*	Fonction declenchee sur reception de message XDM_AT_ENR.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;43 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl43" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier      vl_Num_Action;		/* entier	*/
	XDY_Horodate    vl_Horodate_double;	/* double 	*/
	XDY_Booleen     vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL43_RPC_NAME;	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl43 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_ENR
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl43 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl43 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl43 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl43: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl43 : \nasql_ctparam_smallint_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl43 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl43 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl43 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl43 : \nasql_ctparam_bit_input(va_ResultatAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl43 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl43: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl43 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl43 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl43 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);

                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl43: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl43 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl43 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl43 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl43: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl43: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl43 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl43 : Fin d execution ");
        
}


void atcl44 (	T_IPC_CONN                  va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
        		T_CB_ARG                    pa_ARG) 
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
*	Fonction declenchee sur reception de message XDM_AT_INC.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;44 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl44" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier      vl_Num_Action;		/* entier	*/
	XDY_Horodate    vl_Horodate_double;	/* double 	*/
	XDY_Booleen     vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL44_RPC_NAME;	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl44 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_INC
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl44 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl44 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl44 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl44: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl44 : \nasql_ctparam_smallint_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl44 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl44 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl44 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl44 : \nasql_ctparam_bit_input(va_ResultatAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl44 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl44: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl44 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl44 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl44 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);

                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl44: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl44 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl44 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl44 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl44: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl44: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl44 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl44 : Fin d execution ");
        
}


void atcl45 (	T_IPC_CONN                  va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
        		T_CB_ARG                    pa_ARG) 
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
*	Fonction declenchee sur reception de message XDM_AT_EXP.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;45 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl45" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier      vl_Num_Action;		/* entier	*/
	XDY_Horodate    vl_Horodate_double;	/* double 	*/
	XDY_Booleen     vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL45_RPC_NAME;	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : atcl45 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_EXP
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : atcl45 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl45 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl45 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl45: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl45 : \nasql_ctparam_smallint_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl45 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl45 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl45 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
	  	
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl45 : \nasql_ctparam_bit_input(va_ResultatAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl45 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl45: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl45 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl45 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl45 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);

                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl45: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl45 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl45 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl45 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl45: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl45: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl45 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl45 : Fin d execution ");
        
}



/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle XZAP01SP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int acli_Nb_Critiques   (
                        int      *Nombre_Out )


/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*  XDC_OK
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
*
------------------------------------------------------*/
{
        CS_CHAR *rpc_name ="XZAM12";
        CS_CONTEXT      *context;
        CS_CONNECTION   *connection=XZAGV_Connexion;
        CS_COMMAND      *cmd;
        CS_DATAFMT      datafmt;
        CS_RETCODE      retcode;
        CS_INT          res_type;
        CS_SMALLINT     msg_id;
        int             num_Tab_donnees2 = 1;
        tg_Colonne_donnees2 Tab_donnees2[1];
        XDY_Entier      vl_SPstatus;

        /*A
        ** Definition des variables intermediaires pour la conversion des chaine s au format Sybase
        */


        /*A
        ** Initialisation de la commande
        */

        if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzam12: ct_cmd_alloc() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAM12 retourne %d" , retcode);
                asql_clean_cmd(connection, cmd);
                return (XDC_NOK);
        }

        /*A
        ** Envoie de la commande RPC pour notre stored procedure.
        */

        if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzam12: ct_command() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAM12 retourne %d" , retcode);
                asql_clean_cmd(connection, cmd);
                return (XDC_NOK);
        }

        /*A
        ** Initialisation de la structure pour chacun des parametres d I/O
        ** A faire passer a la rpc.
        */
        strcpy(datafmt.name ,"@va_nombre_out");
        datafmt.namelen   = CS_NULLTERM;
        datafmt.datatype  = CS_SMALLINT_TYPE;
        datafmt.maxlength = CS_UNUSED;
        Tab_donnees2[0].pt_value = (CS_VOID *)Nombre_Out;
        datafmt.status    = CS_RETURN;
        datafmt.locale    = NULL;
       if ((retcode = ct_param(cmd, &datafmt, (CS_VOID *)Nombre_Out,
                CS_SIZEOF(CS_SMALLINT), CS_UNUSED)) != CS_SUCCEED)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzam12: ct_param(NumPA_Out) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAM12 retourne %d" , retcode);
                asql_clean_cmd(connection, cmd);
                return (XDC_NOK);
        }

        /*A
        ** Envoie de la commande au Sql server
        */
        if (ct_send(cmd) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzam12: ct_send() a echoue") ;
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAM12 retourne %d" , retcode);
                asql_clean_cmd(connection, cmd);
                return (XDC_NOK);
        }

        /*A
        ** Traitement du resultat
        */
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
                                                retcode = asql_fetch_data (cmd ) ;
                                                XZST_03EcritureTrace(XZSTC_INFO, "xzam121: ROW RESULTS");
                                                break;

                                        case  CS_PARAM_RESULT:
                                                retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO, "xzam12 : PARAMETER RESULTS\n");
                                                break;

                                        case  CS_STATUS_RESULT:
                                                retcode = asql_fetch (cmd, &vl_SPstatus);
                                                XZST_03EcritureTrace(XZSTC_INFO, "xzam12 : CS_STATUS_RESULT = %ld", retcode);
                                                XZST_03EcritureTrace(XZSTC_INFO, "xzam12 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
                                                if (vl_SPstatus != XDC_OK)
                                                {
                                                   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAM12 retourne 1");
                                                   asql_clean_cmd(connection, cmd);
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
                                                XZST_03EcritureTrace(XZSTC_WARNING,"xzam12: ct_res_info(msgtype) a echoue");
                                                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAM12 retourne %d", retcode);
                                                asql_clean_cmd(connection, cmd);
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
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzam12: ct_results retourne CS_CMD_FAIL.");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAM12 retourne %d", retcode);
                                        break;

                                default:
                                        /*
                                        ** Il s est passe quelque chose d inattendu.
                                        */
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzam12: ct_results retourne un resultat de type inattendu");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAM12 retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
                                        return (XDC_NOK);
                                }
        }
        /*A
        ** Retourne le resultat et les parametres de sorties renseignes
        */
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAM12 retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        return(XDC_OK);
}




/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Met le compte-rendu dans la base
*
*	recoit le message RTWks XDM_AT_EQT_SAGA, recupere les informations
*	les met en forme Sybase et appelle la SP ATSP250.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void atcl251 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AT_PMV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAT;24 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "@(#)acli_tcl.c	1.21 10/05/09 : acli_atcl250" ;
	
	/* Variables locales pour messages RTWks */
	XDY_Entier	vl_Num_Action;		/* short	*/
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Booleen  	vl_ResultatAction; 	/* unsigned char*/
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ATCL251_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_DATETIME     vl_Horodate_in;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : atcl251 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AT_EQT_SAGA
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Num_Action, T_IPC_FT_REAL8,&vl_Horodate_double, T_IPC_FT_CHAR, &vl_ResultatAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : atcl251 a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d\n",
									vl_Num_Action,
									vl_Horodate_double,
									vl_ResultatAction);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : atcl251 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"atcl251 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl251: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_Num_Action,"@va_NumAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl251 : \nasql_ctparam_int_input(va_NumAction_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"atcl251 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl251 : Sortie ");
		asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl251 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 : sortie");
	  	
		asql_clean_cmd(pl_connection, vl_cmd);
	   	return ;
	}
	
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_ResultatAction,"@va_ResultatAction_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl251 : \nasql_ctparam_bit_input(vl_ResultatAction) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"atcl251 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"atcl251: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl251 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"atcl251 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"atcl251 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"atcl251: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"atcl251 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl251: CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"atcl251 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl251: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_WARNING,"atcl251: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : atcl251 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : atcl251 : Fin d execution ");
        
}
