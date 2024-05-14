	/*E*/
/*Fichier : $Id: acli_acl.c,v 1.7 2020/11/03 15:47:25 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2020/11/03 15:47:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACL * FICHIER acli_acl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* ACL contient les fonctions de type Open Client declenchees
* sur reception de message RTWks.
********************
*	aacl01 : recoit le message RTWks XDM_AA_alerte, recupere les informations
*	les met en forme Sybase et appelle la SP AASP01.
********************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
**************	
* gaborit	29 Dec 1994	: Ajout de l asql_clean		V 1.2
* guilhou	29 dec 1994	: texteAlerte format T_STR	V 1.3 ?
* gaborit	05 jan 1995	: Ajout de l asql_cmd_alloc	V 1.3
* niepceron	26 fev 1997	: Ajout de l aacl02 pour tdp	V 1.4
* JMG		21/03/11	: ajout aacl01_localisee 	V 1.5
* LCL		16/09/19	: ajout aacl01_cockpit COCKPIT LOT27 DEM 1351 V 1.6
* LCL		28/04/20	: MOVIS Ajout site local DEMXXX	V 1.7
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_acl.h"
#include "xzdd.h"
#include "xzaec.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_acl.c,v 1.7 2020/11/03 15:47:25 pc2dpdy Exp $ : acli_acl" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	aacl01 : recoit le message RTWks XDM_AA_alerte, recupere les informations
*	les met en forme Sybase et appelle la SP AASP01.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void aacl01 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AA_alerte.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	On appele XZAA;01 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_acl.c,v 1.7 2020/11/03 15:47:25 pc2dpdy Exp $ : acli_aacl01" ;
	
	/* Variables locales pour messages RTWks */
	
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_TypeEqt	vl_TypeEqt;		/* unsigned char*/
	XDY_Eqt		vl_Eqt;			/* short	*/
	XDY_TypeAlerte  vl_TypeAlerte; 		/* unsigned char*/
#ifndef _TIBCO_EMS
	T_STR 		vl_TexteAlerte;		/* str		*/
#else
	char 		vl_TexteAlerte[512];		/* str		*/
#endif
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_AACL01_RPC_NAME;	/*Nom de la SP a appele*/
	
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
	int		vl_ValRet;
	XDY_NomSite     pl_NomSite;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s",version);

       /*A
        ** Recuperation du site de la machine
	   MOVIS ajout site local
        */
        if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_FATAL, "aacl01 : Recuperation du Nom du site impossible .");
                exit(1);
        }

	/*A
	** Recuperation du contenu du message XDM_ETDP_SEUIL
	*/ 
	if (TipcMsgRead(pa_Data->msg,	T_IPC_FT_REAL8, &vl_Horodate_double, 
					T_IPC_FT_CHAR, &vl_TypeEqt,
					T_IPC_FT_INT2, &vl_Eqt,
					T_IPC_FT_CHAR, &vl_TypeAlerte,
					T_IPC_FT_STR,&vl_TexteAlerte, 
					NULL))

	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : aacl01 a recu  Horodate = %lf\n TypeEqt = %d\n Eqt = %d\n TypeAlerte = %d Texte de l alerte = %s\n",
									vl_Horodate_double,
									vl_TypeEqt,
									vl_Eqt,
									vl_TypeAlerte,
									vl_TexteAlerte);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : aacl01 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 Sortie");
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
	 	XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
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
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : aacl01 : Sortie ");
                asql_clean_cmd(pl_connection, vl_cmd);
		return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
	   	return ;
	}	
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_TypeEqt,"@va_TypeEqt_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_tiny_input(va_TypeEqt_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&vl_Eqt,"@va_NumEqt_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"atcl23 : \nasql_ctparam_smallint_input(va_Eqt_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : atcl23 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_TypeAlerte,"@va_Type_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_tiny_input(va_TypeAlerte_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_TexteAlerte,"@va_Texte_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_char_input(va_TexteAlerte_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)pl_NomSite,"@va_NomSiteLocal_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_char_input(pl_NomSite) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 sort ");		
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
						XZST_03EcritureTrace(XZSTC_INFO,"aacl01: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd,&vl_status );
						XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : XZAA01SP retourne %d",vl_status);
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
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
						XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
						return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
		
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT %s",version);
        return;
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	aacl02 : recoit le message RTWks XDM_ETDP_SEUIL, recupere les informations
*	les met en forme Sybase et appelle la SP XZAD01.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void aacl02 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_ETDP_SEUIL.
*	Le message RTwks doit contenir un XDY_Mot,2 XDY_Octet, XDY_Horodate.
*	On appele XZAD;01 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour 
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_acl.c,v 1.7 2020/11/03 15:47:25 pc2dpdy Exp $ : acli_aacl02" ;
	
	/* Variables locales pour messages RTWks */
	
	XDY_Horodate 	vl_Horodate_double;	/* double 	*/
	XDY_Octet	vl_TypeFrn_in;		/* unsigned char*/
	XDY_Mot		vl_zdp;			/* short	*/
	XDY_Octet	vl_Site_in; 		/* unsigned char*/
#ifndef _TIBCO_EMS
	T_CHAR	vl_TypeFrn,vl_Site;		/* unsigned char*/
#else
	XDY_Octet	vl_TypeFrn,vl_Site;		/* unsigned char*/
#endif
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_AACL02_RPC_NAME;	/*Nom de la SP a appele*/
	
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
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s",version);
	
	/*A
	** Recuperation du contenu du message XDM_ETDP_SEUIL
	*/ 
	if (TipcMsgRead(pa_Data->msg,	T_IPC_FT_INT2, &vl_zdp,
					T_IPC_FT_CHAR, &vl_Site,
					T_IPC_FT_CHAR, &vl_TypeFrn,
					T_IPC_FT_REAL8, &vl_Horodate_double, 
					NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : aacl02 a recu  Eqt = %d\n  Site = %d\n TypeFrn = %d\n Horodate = %lf\n",
									vl_zdp,
									vl_Site,
									vl_TypeFrn,
									vl_Horodate_double);	
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : aacl02 a recu  Eqt = %d\n  Site = %d\n TypeFrn = %d\n Horodate = %lf\n",
									vl_zdp,
									vl_Site,
									vl_TypeFrn,
									vl_Horodate_double);	
		vl_TypeFrn_in=(XDY_Octet)vl_TypeFrn;
		vl_Site_in=(XDY_Octet)vl_Site;
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : aacl02 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 Sortie");
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
	 	XZST_03EcritureTrace(XZSTC_FATAL,"aacl02 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl02: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	
	if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&vl_zdp,"@va_Numero_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl02 : \nasql_ctparam_smallint_input(va_Numero_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : atcl23 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl02 : \nasql_ctparam_tiny_input(va_Site_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
			
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_TypeFrn_in,"@va_Type_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl02 : \nasql_ctparam_tiny_input(va_Type_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate_double,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : aacl02 : Sortie ");
                asql_clean_cmd(pl_connection, vl_cmd);
		return ;
        }
        
	if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl02 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
	   	return ;
	}	

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl02 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 sort ");		
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
						XZST_03EcritureTrace(XZSTC_INFO,"aacl02: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"aacl02 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd,&vl_status );
						XZST_03EcritureTrace(XZSTC_INFO,"aacl02 : XZAA01SP retourne %d",vl_status);
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aacl02 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 : sortie");
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
						XZST_03EcritureTrace(XZSTC_FATAL,"aacl02: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
						return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"aacl02 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"aacl02 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"aacl02 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"aacl02: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"aacl02: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl02 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}

	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT %s",version);
        return;
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       aacl01_localisee : recoit le message RTWks XDM_AA_alerte_localisee, recupere les informations
*       les met en forme Sybase et appelle la SP AASP01.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void aacl01_localisee   (       T_IPC_CONN                      va_Cnx,
                        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
                        T_CB_ARG                        pa_ARG)
/*
* ARGUMENTS EN ENTREE :
*
*       Arguments d'une callback RTServer.
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
*       Fonction declenchee sur reception de message XDM_AA_alerte.
*       Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*       On appele XZAA;01 une SP n effectuant pas de select et ne prenant pas
*       de parametre de retour
*
------------------------------------------------------*/
{
        static char*version = "$Id: acli_acl.c,v 1.7 2020/11/03 15:47:25 pc2dpdy Exp $ : acli_aacl01" ;

        /* Variables locales pour messages RTWks */

        XDY_Horodate    vl_Horodate_double;     /* double       */
        XDY_TypeEqt     vl_TypeEqt;             /* unsigned char*/
        XDY_Eqt         vl_Eqt;                 /* short        */
        XDY_TypeAlerte  vl_TypeAlerte;          /* unsigned char*/
#ifndef _TIBCO_EMS
        T_STR           vl_TexteAlerte;         /* str          */
        T_STR           vl_autoroute;
#else
        char           vl_TexteAlerte[512];         /* str          */
        XDY_NomAuto           vl_autoroute;
#endif
        XDY_PR          vl_pr;
        int     vl_sens2;
        XDY_Octet vl_sens;

        /* Variables locales pour L Open Client SYBASE */

        CS_CHAR *rpc_name = ACLIC_AACL11_RPC_NAME;      /*Nom de la SP a appele*/

        CS_CONTEXT      *pl_context   = XZAGV_Context;
        CS_CONNECTION   *pl_connection= XZAGV_Connexion;        /* Variable Globale de connexion
                                                                ** initialise par xzag_connexion
                                                                ** appele dans acli_dir.
                                                                */

        CS_COMMAND      *vl_cmd;

        CS_DATETIME     vl_Horodate_in;
        CS_RETCODE      vl_retcode;
        CS_INT          vl_res_type;
        CS_SMALLINT     vl_msg_id;
        XDY_Entier      vl_status=0;
	int		vl_ValRet;
	XDY_NomSite     pl_NomSite;

       XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s",version);

        /*A
        ** Recuperation du site de la machine
	   MOVIS ajout site local
        */
        if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_FATAL, "aacl01 : Recuperation du Nom du site impossible .");
                exit(1);
        }

        /*A
        ** Recuperation du contenu du message XDM_ETDP_SEUIL
        */
        if (TipcMsgRead(pa_Data->msg,   T_IPC_FT_REAL8, &vl_Horodate_double,
                                        T_IPC_FT_CHAR, &vl_TypeEqt,
                                        T_IPC_FT_INT2, &vl_Eqt,
                                        T_IPC_FT_CHAR, &vl_TypeAlerte,
                                        T_IPC_FT_STR,&vl_TexteAlerte,
                                        T_IPC_FT_STR,&vl_autoroute,
                                        T_IPC_FT_INT4,&vl_pr,
                                        T_IPC_FT_INT4,&vl_sens2,
                                        NULL))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"tacli : aacl01_localisee a recu  Horodate = %lf\n TypeEqt = %d\n Eqt = %d\n TypeAlerte = %d Texte de l alerte = %s\n Autoroute=%s\n PR=%d\n Sens=%d\n",
                                                                         vl_Horodate_double,
                                                                        vl_TypeEqt,
                                                                        vl_Eqt,
                                                                        vl_TypeAlerte,
                                                                        vl_TexteAlerte,
                                                                        vl_autoroute,
                                                                        vl_pr,
                                                                        vl_sens2);
                vl_sens=(XDY_Octet)vl_sens2;
                XZST_03EcritureTrace(XZSTC_WARNING,"tacli : aacl01_localisee %d",vl_sens);
 
        }
        else
        {
                /*B
                ** Erreur detectee : Trace l erreur
                */
                XZST_03EcritureTrace(XZSTC_WARNING ," tacli : aacl01 : TipcMsgRead a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 Sortie");
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
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : ct_cmd_alloc() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 Sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        /*A
        ** Envoie de la commande RPC pour notre stored procedure.
        */
        if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_command() a echoue avec le code %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
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
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : aacl01 : Sortie ");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }

        if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : asql_ct_param_date_input(date) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);

                return ;
        }
        if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_TypeEqt,"@va_TypeEqt_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_tiny_input(va_TypeEqt_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&vl_Eqt,"@va_NumEqt_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"atcl23 : \nasql_ctparam_smallint_input(va_Eqt_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : atcl23 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_TypeAlerte,"@va_Type_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_tiny_input(va_TypeAlerte_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_TexteAlerte,"@va_Texte_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_char_input(va_TexteAlerte_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_char_input(va_Autoroute_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_pr,"@va_PR_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_tiny_input(va_PR_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_sens,"@va_Sens_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_tiny_input(va_TypeAlerte_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)pl_NomSite,"@va_NomSiteLocal_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : \nasql_ctparam_char_input(pl_NomSite) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
        /*A
        ** Envoie de la commande au Sql server
        */
        if (ct_send(vl_cmd) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : ct_send() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 sort ");
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
                                                XZST_03EcritureTrace(XZSTC_INFO,"aacl01: ROW RESULTS");
                                                break;

                                        case  CS_PARAM_RESULT:
                                                vl_retcode = asql_fetch_data (vl_cmd );
                                                XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : PARAMETER RESULTS\n");
                                                break;

                                        case  CS_STATUS_RESULT:
                                                vl_retcode = asql_fetch (vl_cmd,&vl_status );
                                                XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : XZAA01SP retourne %d",vl_status);
                                                break;
                                }

                                if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
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
                                                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_res_info(msgtype) a echoue");
                                                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                                                /*return vl_retcode; OM*/
                                                asql_clean_cmd(pl_connection, vl_cmd);
                                                return;
                                        }

                                        XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

                                        break;

                                case CS_CMD_SUCCEED:
                                        /*
                                        ** Pas de colonnes retournees.
                                        */
                                        XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : CS_CMD_SUCCEED \n");

                                        break;

                                case CS_CMD_DONE:

                                        /*
                                        ** Commande effectuee : positionnement des parametres de retour.
                                        */
                                        XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : CS_CMD_DONE \n");
                                        break;

                                case CS_CMD_FAIL:
                                        /*
                                        ** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
                                        */
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_results retourne CS_CMD_FAIL.");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                        return;
                                        break;

                                default:
                                        /*
                                        ** Il s est passe quelque chose d inattendu.
                                        */
                                        printf("general = default \n");
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_results retourne un resultat de type inattendu");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                        return;
                                }
                }
        asql_clean_cmd(pl_connection, vl_cmd);
        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT %s",version);
        return;
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       aacl01_cockpit : recoit le message RTWks XDM_AA_alerte_cockpit, recupere les informations
*       les met en forme Sybase et appelle la SP AASP01.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void aacl01_cockpit(       T_IPC_CONN                      va_Cnx,
                        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
                        T_CB_ARG                        pa_ARG)
/*
* ARGUMENTS EN ENTREE :
*
*       Arguments d'une callback RTServer.
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
*       Fonction declenchee sur reception de message XDM_AA_alerte_cockpit.
*       Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*       On appele XZAA;01 une SP n effectuant pas de select et ne prenant pas
*       de parametre de retour
*
------------------------------------------------------*/
{
        static char *version = "$Id: acli_acl.c,v 1.7 2020/11/03 15:47:25 pc2dpdy Exp $ : acli_aacl01_cockpit" ;

        /* Variables locales pour messages RTWks */

        XDY_Horodate    vl_Horodate_double;     /* double       */
        XDY_TypeEqt     vl_TypeEqt;             /* unsigned char*/
        XDY_Eqt         vl_Eqt;                 /* short        */
        XDY_TypeAlerte  vl_TypeAlerte;          /* unsigned char*/
#ifndef _TIBCO_EMS
        T_STR           vl_TexteAlerte;         /* str          */
        T_STR           vl_autoroute;
	T_STR		vl_idCockpit;
#else
        char           vl_TexteAlerte[512];         /* str          */
        XDY_NomAuto           vl_autoroute;
	char		vl_idCockpit[128];
#endif
        XDY_PR          vl_pr_deb;
        XDY_PR          vl_pr_fin;
        int     	vl_sens2;
        XDY_Octet 	vl_sens;
	int		vl_probabilite;

	int		vl_TypeFMC;
	int		vl_poste;
	int		vl_site;

        /* Variables locales pour L Open Client SYBASE */

        CS_CHAR *rpc_name = ACLIC_AACL31_RPC_NAME;      /*Nom de la SP a appele*/

        CS_CONTEXT      *pl_context   = XZAGV_Context;
        CS_CONNECTION   *pl_connection= XZAGV_Connexion;        /* Variable Globale de connexion
                                                                ** initialise par xzag_connexion
                                                                ** appele dans acli_dir.
                                                                */

        CS_COMMAND      *vl_cmd;

        CS_DATETIME     vl_Horodate_in;
        CS_RETCODE      vl_retcode;
        CS_INT          vl_res_type;
        CS_SMALLINT     vl_msg_id;
        XDY_Entier      vl_status=0;
	T_STR		vl_posteName;

       XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s",version);

        /*A
        ** Recuperation du contenu du message XDM_AA_alerte_cockpit
        */


        if (TipcMsgRead(pa_Data->msg,   
                                        T_IPC_FT_INT4,	&vl_TypeFMC,
                                        T_IPC_FT_INT4,	&vl_poste,
					T_IPC_FT_INT4, 	&vl_site,
					T_IPC_FT_STR, 	&vl_idCockpit,
					T_IPC_FT_REAL8, &vl_Horodate_double,
					T_IPC_FT_CHAR, 	&vl_TypeEqt,
					T_IPC_FT_INT2, 	&vl_Eqt,
					T_IPC_FT_CHAR,  &vl_TypeAlerte,
					T_IPC_FT_STR,	&vl_TexteAlerte,
                                        T_IPC_FT_STR,	&vl_autoroute,
                                        T_IPC_FT_INT4,	&vl_pr_deb,
                                        T_IPC_FT_INT4,	&vl_pr_fin,
                                        T_IPC_FT_INT4,	&vl_sens2,
					T_IPC_FT_INT4,	&vl_probabilite,
					NULL))
	       {
                XZST_03EcritureTrace(XZSTC_WARNING,"tacli : aacl01_cockpit a recu  Horodate = %lf\n TypeEqt = %d\n Eqt = %d\n TypeFMC = %d\n Texte de l alerte = %s\n IDCockpit= %s\n Autoroute=%s\n PRi deb=%d\n PR fin=%d\n Sens=%d\n",
                                                                         vl_Horodate_double,
                                                                        vl_TypeEqt,
                                                                        vl_Eqt,
                                                                        vl_TypeFMC,
                                                                        vl_TexteAlerte,
									vl_idCockpit,
                                                                        vl_autoroute,
                                                                        vl_pr_deb,
                                                                        vl_pr_fin,
                                                                        vl_sens2);

		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : aacl01_cockpit a recu Poste = %d\n Site = %d\n Confirmee/Signalee = %d",vl_poste,vl_site,vl_probabilite);
                vl_sens=(XDY_Octet)vl_sens2;
                XZST_03EcritureTrace(XZSTC_WARNING,"tacli : aacl01_cockpit %d",vl_sens);
 
        }
        else
        {
                /*B
                ** Erreur detectee : Trace l erreur
                */
                XZST_03EcritureTrace(XZSTC_WARNING ," tacli : aacl01_cockpit : TipcMsgRead a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit Sortie");
                return;
        }

	/* détermination du site */
	if (vl_site == XDC_DP)
		vl_posteName = "PODP1";
	else
        	vl_posteName = "POVC1";

        /*A
        ** Open Client
        */
        /*A
        ** Initialisation de la commande
        */

        if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01_cockpit : ct_cmd_alloc() a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit Sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        /*A
        ** Envoie de la commande RPC pour notre stored procedure.
        */
        if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01_cockpit: ct_command() a echoue avec le code %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
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
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : aacl01_cockpit : Sortie ");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }

        if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : asql_ct_param_date_input(date) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);

                return ;
        }
        if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_TypeEqt,"@va_TypeEqt_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_tiny_input(va_TypeEqt_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&vl_Eqt,"@va_NumEqt_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_smallint_input(va_NumEqt_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_TypeFMC,"@va_TypeFMC_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_int_input(va_TypeFMC_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_poste,"@va_Poste_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_int_input(va_Poste_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_site,"@va_Site_in"))!= CS_SUCCEED)
        {
                 XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_int_input(va_Site_in) a echoue avec %d",vl_retcode);
                 XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                 asql_clean_cmd(pl_connection, vl_cmd);
                 return ;
        }
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_TexteAlerte,"@va_Texte_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_char_input(va_TexteAlerte_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR*)vl_idCockpit,"@va_IdCockpit_in"))!=CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_char_input(va_IdCockpit_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return;
        }
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_char_input(va_Autoroute_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_pr_deb,"@va_PR_deb_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_int_input(va_PR_deb_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_pr_fin,"@va_PR_fin_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_int_input(va_PR_fin_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
        if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_sens,"@va_Sens_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : \nasql_ctparam_tiny_input(va_Sens_in) a echoue avec %d",vl_retcode);
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_probabilite,"@va_Probabilite_in"))!= CS_SUCCEED)
        {
	        XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 cockpit : asql_ctparam_int_input(va_Probabilite_in) a echoue avec %d",vl_retcode);
	        asql_clean_cmd(pl_connection, vl_cmd);
	        return;
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1, "va_TypeEqt_in=%d\n va_NumEqt_in=%d\n va_TypeFMC_in=%d\n va_Poste_in=%d\n va_Site_in=%d\n va_Texte_in=%s\n va_IdCockpit_in=%s\n va_Autoroute_in=%s\n va_PR_deb_in=%d\n va_PR_fin_in=%d\n va_Sens_in=%d\n va_Probabilite_in=%d\n va_PosteName=%d",vl_TypeEqt,vl_Eqt,vl_TypeFMC,vl_poste,vl_site,vl_TexteAlerte,vl_idCockpit,vl_autoroute,vl_pr_deb,vl_pr_fin,vl_sens,vl_probabilite,vl_posteName		);

        /*A
        ** Envoie de la commande au Sql server
        */

        if (ct_send(vl_cmd) != CS_SUCCEED)
        {
                
                XZST_03EcritureTrace(XZSTC_FATAL,"aacl01 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit sort ");
                asql_clean_cmd(pl_connection, vl_cmd);
                return ;
        }
       /*A
        ** Traitement du resultat
        */

        
	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
                XZST_03EcritureTrace(XZSTC_INFO,"aacl01_cockpit: Proc ret=%d",vl_status);
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
        					XZST_03EcritureTrace(XZSTC_DEBUG1, "aacl01_cockpit : case Row result");
                                                vl_retcode = asql_fetch_data(vl_cmd);
                                                XZST_03EcritureTrace(XZSTC_INFO,"aacl01_cockpit: ROW RESULTS");
                                                break;

                                        case  CS_PARAM_RESULT:
        					XZST_03EcritureTrace(XZSTC_DEBUG1, "aacl01_cockpit : case param result");
                                                vl_retcode = asql_fetch_data (vl_cmd );
                                                XZST_03EcritureTrace(XZSTC_INFO,"aacl01_cockpit : PARAMETER RESULTS\n");
                                                break;

                                        case  CS_STATUS_RESULT:
        					XZST_03EcritureTrace(XZSTC_DEBUG1, "aacl01_cockpit : case status result");
                                                vl_retcode = asql_fetch (vl_cmd,&vl_status );
                                                XZST_03EcritureTrace(XZSTC_INFO,"aacl01_cockpit : XZAE199 retourne %d",vl_status);
                                                
						/*
						** Appel plan d'action
						*/						 
						if (vl_status == 0)
						{
							XZST_03EcritureTrace(XZSTC_INFO,"aacl01_cockpit : poste name = %s\n",vl_posteName);
							if (XZDD02_CalculerPA(vl_Horodate_double,-1,vl_posteName,vl_TypeFMC,XZAEC_FMC_Regulation,XZDDC_PA_SANS_TFM,XZDDC_MODE_OPERATIONNEL)==XDC_OK)
							{
								XZST_03EcritureTrace(XZSTC_INFO,"aacl01_cockpit : XZDD02_CalculerPA success\n");
							}
							else
							{
								XZST_03EcritureTrace(XZSTC_INFO,"aacl01_cockpit : XZDD02_CalculerPA fail \n");
							}
						}
						break;
                                }

                                if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aacl01_cockpit : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
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
                                                		XZST_03EcritureTrace(XZSTC_FATAL,"aacl01_cockpit: ct_res_info(msgtype) a echoue");
                                                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01_cockpit : sortie");
                                                /*return vl_retcode; */
                                                asql_clean_cmd(pl_connection, vl_cmd);
                                                return;
                                        }

                                        XZST_03EcritureTrace(XZSTC_INFO,"aacl01_cockpit : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

                                        break;

                                case CS_CMD_SUCCEED:

					/*
                                        ** Pas de colonnes retournees.
                                        */
                                        XZST_03EcritureTrace(XZSTC_INFO,"aacl01 : CS_CMD_SUCCEED \n");
                                        break;

                                case CS_CMD_DONE:

                                        /*
                                        ** Commande effectuee : positionnement des parametres de retour.
                                        */
                                        XZST_03EcritureTrace(XZSTC_INFO,"aacl01_cockpit : CS_CMD_DONE \n");
                                        break;

                                case CS_CMD_FAIL:
                                        /*
                                        ** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
                                        */
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_results retourne CS_CMD_FAIL.");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                        return;
                                        break;

                                default:
                                        /*
                                        ** Il s est passe quelque chose d inattendu.
                                        */
                                        printf("general = default \n");
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aacl01: ct_results retourne un resultat de type inattendu");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aacl01 : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
                                        return;
					break;
			}
         
	 }
        asql_clean_cmd(pl_connection, vl_cmd);
        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT %s",version);
        return;
}
