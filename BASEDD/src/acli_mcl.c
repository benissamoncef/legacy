/*E*/
/*Fichier : $Id: acli_mcl.c,v 1.7 2008/11/04 10:56:38 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2008/11/04 10:56:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE MCL * FICHIER acli_mcl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_mcl.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
* gaborit	05 jan 1995	: modif clean (1.2)
* gaborit       02 oct 1995	: appel acknoledge (xzex05) (1.4)
* volcic        04 oct 1995	: ajout param Horodate dans acknoledge (xzex05) (1.5)
* JBL        17/10/2008	: modification du type d'alarme : XDY_TypeAlarme au lieu de XDY_TypeAlerte et lecture du message SM : int2 au lieu de char 1.6
* PNI		04/11/2008	: ajout trace sur pb lecture du message SM v1.7
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_mcl.h"
#include "xzex.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_mcl.c,v 1.7 2008/11/04 10:56:38 gesconf Exp $ : acli_mcl" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	amcl01 : recoit le message RTWks XDM_AM_alarme, recupere les informations
*	les met en forme Sybase et appelle la SP AASP01.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void amcl01 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AM_alarme.
*	Le message RTwks doit contenir un XDY_Horodate,XDY_TypeEqt,XDY_Eqt,XDY_TypeAlerte,XDY_Booleen.
*	On appele XZAA;01 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_mcl.c,v 1.7 2008/11/04 10:56:38 gesconf Exp $ : acli_amcl01" ;
	
	/* Variables locales pour messages RTWks */
	
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_TypeEqt	vl_TypeEqt;		/* unsigned char*/
	XDY_Eqt		vl_Eqt;			/* short	*/
	XDY_TypeAlarme  vl_TypeAlarme; 		/* mot */
	XDY_Booleen	vl_Etat;		/* booleen  bit	*/
#ifndef _TIBCO_EMS
	T_STR		vl_NomMachine;
#else
	XDY_NomMachine		vl_NomMachine;
#endif
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_AMCL01_RPC_NAME;	/*Nom de la SP a appele*/
	
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
	XDY_Entier      vl_status=0;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : amcl01 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AM_alarme
	*/ 
	if (TipcMsgRead(pa_Data->msg,	T_IPC_FT_REAL8, &vl_Horodate_double, 
					T_IPC_FT_CHAR, &vl_TypeEqt,
					T_IPC_FT_INT2, &vl_Eqt,
					T_IPC_FT_INT2, &vl_TypeAlarme,
					T_IPC_FT_CHAR, &vl_Etat, 
					T_IPC_FT_STR, &vl_NomMachine,
					NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : amcl01 a recu  Horodate = %lf\n TypeEqt = %d\n Eqt = %d\n TypeAlarme = %d Etat = %d\n",
									vl_Horodate_double,
									vl_TypeEqt,
									vl_Eqt,
									vl_TypeAlarme,
									vl_Etat);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : amcl01 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 Sortie");
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : amcl01 a recu  Horodate = %lf\n TypeEqt = %d\n Eqt = %d\n TypeAlarme = %d Etat = %d\n",
									vl_Horodate_double,
									vl_TypeEqt,
									vl_Eqt,
									vl_TypeAlarme,
									vl_Etat);	
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
	 	XZST_03EcritureTrace(XZSTC_FATAL,"amcl01 : asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"amcl01: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"amcl01 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : amcl01 : Sortie ");
                asql_clean_cmd(pl_connection, vl_cmd);
		return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"amcl01 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
	   	return ;
	}	
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_TypeEqt,"@va_TypeEqt_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"amcl01 : \nasql_ctparam_tiny_input(va_TypeEqt_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&vl_Eqt,"@va_Eqt_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"atcl23 : \nasql_ctparam_smallint_input(va_Eqt_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : atcl23 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&vl_TypeAlarme,"@va_Type_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"amcl01 : \nasql_ctparam_small_input(va_TypeAlerte_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_Etat,"@va_Etat_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"amcl01 : \nasql_ctparam_bit_input(va_Booleen_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"amcl01 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 sort ");		
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
						XZST_03EcritureTrace(XZSTC_INFO,"amcl01: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"amcl01 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd,&vl_status );
						XZST_03EcritureTrace(XZSTC_INFO,"amcl01 : CS_STATUS_RESULT :%d",vl_status);
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_FATAL,"amcl01 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 : sortie");
                                        
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
						XZST_03EcritureTrace(XZSTC_FATAL,"amcl01: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
                                		return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"amcl01 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"amcl01 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"amcl01 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"amcl01: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"amcl01: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : amcl01 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	/* envoi de l'acknoledge */
	if (XZEX05_Acknoledge_Alarme(vl_Horodate_double, vl_TypeEqt, vl_Eqt, vl_TypeAlarme, vl_Etat, vl_NomMachine)!=XDC_OK)
		XZST_03EcritureTrace(XZSTC_WARNING,"%s : l'appel � XZEX05_Acknoledge_Alarme retourne XDC_NOK",version);

	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : amcl01 : Fin d execution ");
        
}


