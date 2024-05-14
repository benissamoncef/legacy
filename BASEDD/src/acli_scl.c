/*E*/
/*Fichier : $Id: acli_scl.c,v 1.14 2013/01/29 10:55:46 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2013/01/29 10:55:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ASCL * FICHIER acli_scl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   	Appertient a tacli. Effectue l animation synoptique 
*  sur reception de message rtworks (XDM_AS_FdeT).
*********************************
*	ascl06 : recoit le message RTWks XDM_AS_FdeT, recupere les informations
*	les met en forme Sybase et appelle la SP XZAS;06.
*	XZAS;06 retourne un 60 lignes de mesures 6 mns a renvoyer a XZIS10
* 	afin d animer les synoptiques courbes traffic 6mins.
*********************************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	15 Nov 1994	: Creation
* gaborit	10 jan 1995	: modif va_horodate_in va_horodate_debut_in (1.2)
* gaborit	13 jan 1995	: modif va_horodate_out  (1.3)
* C.T.		13 jan 1995	: Ajout fonction ascl02 (1.4)
* C.T.		13 jan 1995	: Ajout fonction ascl03 (1.5)
* gaborit	17 jan 1995	: modif pr fin ascl02 et 03 (1.6)
* gaborit	17 jan 1995	: modif code retour date (1.7)
* gaborit	18 jan 1995	: allocation pl_Tab_donnees2 (1.8)
* gaborit	20 jan 1995	: ascl02 et 03 appel de xzis si 0 lignes (1.9)
* niepceron	23 sep 1996	: ascl02 : ajout de la plage (ana3) (1.10)
* niepceron     04 fev 1997	: TDP
* Hilmarcher    06 sep 2005     : rajout param num_fmc ds ascl03 DEM509 v1.12 
* JMG		20/04/11	: correction ascl06 pour 64 bits 1.13
* JMG		04/08/12	: correction plantage acli_scl10 1.14
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_scl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_scl.c,v 1.14 2013/01/29 10:55:46 pc2dpdy Exp $ : acli_scl" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	ascl06 : recoit le message RTWks XDM_AS_FdeT, recupere les informations
*	les met en forme Sybase et appelle la SP XZAS;06.
*	XZAS;06 retourne un 60 lignes de mesures 6 mns a renvoyer a XZIS10
* 	afin d animer les synoptiques courbes traffic 6mins.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ascl06 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AS_FdeT.
*	Le message RTwks doit contenir un XDY_Datagroup, XDY_Eqt,XDY_Sens,
*  					XDY_Voie,XDY_Horodate,XDY_Octet .
*	On appele XZAS;06 qui renvoie des lignes colonnes (Mesures traffic 6min)
*  a renvoyer dans un message sur le DG recu dans le message qui a declenche ce client. 
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_scl.c,v 1.14 2013/01/29 10:55:46 pc2dpdy Exp $ : acli_ascl06" ;
	
	/* Variables locales pour messages RTWks */
	
	XDY_Datagroup   vl_datagroup_specif;
#ifndef _TIBCO_EMS
	T_STR		pl_datagroup;
#else
	XDY_Datagroup		pl_datagroup;
#endif
	XDY_Mot		vl_NumStation;
	XDY_Sens	vl_Sens;
	XDY_Voie	vl_Voie;
	XDY_Horodate	vl_HorodateDebut;
	XDY_Horodate	vl_HorodateDebut_out;
	XDY_Octet	vl_TypeDeDonnees;
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ASCL06_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/
	CS_COMMAND	*pl_cmd;							
	
	/* Variables Specifiques Open Client */														
	int             	vl_num_Tab_donnees2 = ACLIC_ASCL06_NB_PARM_RETURN;
        tg_Colonne_donnees2	pl_Tab_donnees2[ACLIC_ASCL06_NB_PARM_RETURN];
        int             	vl_Nb_Ligne_Lue=0;
        int             	vl_Nb_Col=0;
        tg_row_result   	vl_Tab_Ligne;
        int			vl_indice;
        
	ASCL06T_Tab_LignesDonnees vl_Tab_Mesure_6mns;
	
	XDY_Entier 	vl_SPstatus=XDC_OK;			/* Contient le status de la SP */
	XDY_Booleen	vl_Bool	= XDC_VRAI;			/* Si vl_Bool	= XDC_VRAI la SP a trouve des donnes valides sinon XDC_FAUX*/
	
	/* Variables intermediaires pour les besoins de SYBASE */
	CS_DATETIME     vl_Horodate_in;
	CS_DATETIME     vl_Horodate_out;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : ascl06 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AS_FdeT
	*/ 
	if (TipcMsgRead(pa_Data->msg,	T_IPC_FT_STR,	&pl_datagroup,
					T_IPC_FT_INT2,	&vl_NumStation,
					T_IPC_FT_CHAR,	&vl_Sens,
					T_IPC_FT_CHAR,	&vl_Voie,
					T_IPC_FT_REAL8,	&vl_HorodateDebut,
					T_IPC_FT_CHAR,	&vl_TypeDeDonnees,
					NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : ascl06 a recu DG=%s NumStation =%d dans le sens %u sur voie %u a horodate %lf sur type de donnees = %u",
						pl_datagroup,
						vl_NumStation,
						vl_Sens,
						vl_Voie,
						vl_HorodateDebut,
						vl_TypeDeDonnees);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : ascl06 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	strcpy(vl_datagroup_specif,pl_datagroup);

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 Sortie");		
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl06: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
		asql_clean_cmd (pl_connection,pl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_HorodateDebut,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : ascl06 : Sortie ");
                asql_clean_cmd (pl_connection,pl_cmd);
                return ;
        }
        
        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&vl_NumStation,"@va_Station_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : \nasql_ctparam_smallint_input(va_Station_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");	
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : \nasql_ctparam_tiny_input(va_Sens_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");	
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	
        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&vl_Voie,"@va_Voie_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : \nasql_ctparam_tiny_input(va_Voie_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");	
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_HorodateDebut_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}	
	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&vl_TypeDeDonnees,"@va_TypeDeDonnees_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : \nasql_ctparam_tiny_input(va_TypeDeDonnees_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");	
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	/*
	** 1 parametre en output
	*/
	pl_Tab_donnees2[0].pt_value = (CS_VOID *)&vl_Horodate_out;
	if ((vl_retcode = asql_ctparam_date_output(pl_cmd,(CS_DATETIME *)&vl_Horodate_out,"@va_HorodateDebut_out"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : asql_ct_param_date_output(date) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}
	
	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 sort ");		
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
						XZST_03EcritureTrace(XZSTC_INFO,"ascl06: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd, vl_num_Tab_donnees2,pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"ascl06 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd,&vl_SPstatus );
						XZST_03EcritureTrace(XZSTC_INFO,"ascl06 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"ascl06: ct_res_info(msgtype) a echoue");
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"ascl06 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"ascl06 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"ascl06 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl06: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
					asql_clean_cmd (pl_connection,pl_cmd);
					
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl06: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
					asql_clean_cmd (pl_connection,pl_cmd);
					return;
					break;
				}
		}
	
	if(vl_SPstatus == XDC_OK)
	
	{
		vl_Bool	= XDC_VRAI;
		XZST_03EcritureTrace(XZSTC_INFO,"ascl06 : SP a retourne des donnees valides");
		for (vl_indice = 0 ; vl_indice<ACLIC_ASCL06_NB_LIG_MESURES;vl_indice++)
		{
			vl_Tab_Mesure_6mns[vl_indice].Valeur	= * (XDY_Mot   *) vl_Tab_Ligne[vl_indice][0].pt_value;
			vl_Tab_Mesure_6mns[vl_indice].Validite	= * (XDY_Octet *) vl_Tab_Ligne[vl_indice][1].pt_value;
		}	
	}
	else
	{
		vl_Bool	= XDC_FAUX;
		switch( vl_SPstatus)
		{
			case XDC_NOK : XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : SP a rencontre un prob d acces aux donnees");
			break;
			case XDC_ARG_INV : XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : SP a recu des arguments invalides");
			break;
			case XZASC_RADT_PAS_MES : XZST_03EcritureTrace(XZSTC_INFO,"ascl06 : SP n a pas de mesure a retourner");
			break;
			default : XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : SP a retourne un status non attendu");
			break;
		}
	}
	
	/*A
	** Si donnees trouvees, Envoie des donnees aux synoptiques
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Sybase2Ux(vl_Horodate_out,&vl_HorodateDebut_out))!= XDC_OK)
        {
         	XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : asql_date_Sybase2Ux(vl_Horodate_in,&vl_HorodateDebut_out) a echoue");
         	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : ascl06 : Sortie ");
     		vl_Bool	= XDC_FAUX;
     	}	
	if ((vl_retcode = XZIS10_Donnees_FdeT(	vl_Bool,
						vl_datagroup_specif,
						vl_NumStation,
						vl_Sens,
						vl_Voie,
						vl_HorodateDebut_out,
						vl_TypeDeDonnees,
						vl_Tab_Mesure_6mns))!=XDC_OK)
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : XZIS10_Donnees_FdeT a echoue");
        }
	else {	
		
	/*A
        ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
        ** si il y a lieu
        */

	/*test si donneees presentes 04/08/12*/
	if (vl_Bool==XDC_VRAI) {
		if (vl_SPstatus != XZASC_RADT_PAS_MES) {
        		if ((vl_retcode = asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne)) != XDC_OK)
        		{	
               			 XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
        		}
		}
	}
	}

	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : ascl06 : Fin d execution ");
        asql_clean_cmd (pl_connection,pl_cmd);
	return;
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	ascl07 : recoit le message RTWks XDM_AS_FdeT, recupere les informations
*	les met en forme Sybase et appelle la SP XZAS;06.
*	XZAS;06 retourne un 60 lignes de mesures 6 mns a renvoyer a XZIS10
* 	afin d animer les synoptiques courbes traffic 6mins.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ascl07 	(	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AS_FdeT.
*	Le message RTwks doit contenir un XDY_Datagroup, XDY_Eqt,XDY_Sens,
*  					XDY_Voie,XDY_Horodate,XDY_Octet .
*	On appele XZAS;06 qui renvoie des lignes colonnes (Mesures traffic 6min)
*  a renvoyer dans un message sur le DG recu dans le message qui a declenche ce client. 
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_scl.c,v 1.14 2013/01/29 10:55:46 pc2dpdy Exp $ : acli_ascl07" ;
	
	/* Variables locales pour messages RTWks */
	
	XDY_Datagroup   vl_datagroup_specif;
#ifndef _TIBCO_EMS
	T_STR 		pl_datagroup;
#else
	XDY_Datagroup 		pl_datagroup;
#endif
	XDY_Mot		vl_NumZone;
	XDY_Horodate	vl_HorodateDebut;
	XDY_Mot		vl_seuil_max_out;
	XDY_Mot		vl_seuil_bas_out;
	XDY_Mot		vl_seuil_haut_out;
	XDY_Nom		vl_NomAutoroute_out;
	XDY_Entier	vl_PRdebut_out;
	XDY_Entier	vl_PRfin_out;
	XDY_Octet	vl_Sens_out;
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_ASCL13_RPC_NAME;		/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/
	CS_COMMAND	*pl_cmd;							
	
	/* Variables Specifiques Open Client */														
	int             	vl_num_Tab_donnees2 = ACLIC_ASCL14_NB_PARM_RETURN;
        tg_Colonne_donnees2	pl_Tab_donnees2[ACLIC_ASCL14_NB_PARM_RETURN];
        int             	vl_Nb_Ligne_Lue=0;
        int             	vl_Nb_Col=0;
        tg_row_result   	vl_Tab_Ligne;
        int			vl_indice;
        
	XDY_Mot  *vl_Tab;
	
	XDY_Entier 	vl_SPstatus=XDC_OK;			/* Contient le status de la SP */
	XDY_Booleen	vl_Bool	= XDC_VRAI;			/* Si vl_Bool	= XDC_VRAI la SP a trouve des donnes valides sinon XDC_FAUX*/
        int             	vl_Taille=0;
	
	/* Variables intermediaires pour les besoins de SYBASE */
	CS_DATETIME     vl_Horodate_in;
	CS_DATETIME     vl_Horodate_out;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : ascl07 : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_AS_TDP
	*/ 
	if (TipcMsgRead(pa_Data->msg,	T_IPC_FT_STR,	&pl_datagroup,
					T_IPC_FT_INT2,	&vl_NumZone,
					T_IPC_FT_REAL8,	&vl_HorodateDebut,
					NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : ascl07 a recu DG=%s NumZone =%d  a horodate %lf ",
						pl_datagroup,
						vl_NumZone,
						vl_HorodateDebut);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : ascl07 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 Sortie");
		return;	 
	}
	/*A
	** Open Client
	*/
	/*A
	** Initialisation de la commande				
	*/

	/*strcpy(vl_datagroup_specif,pl_datagroup);*/
	sprintf(vl_datagroup_specif,"%s",pl_datagroup);

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 Sortie");		
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");
		asql_clean_cmd (pl_connection,pl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	
	/*A
	** Transformation de l horodate de double vers le structure Sybase
	*/
	if ((vl_retcode = asql_date_Ux2Sybase(vl_HorodateDebut,&vl_Horodate_in))!= XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : ascl07 : Sortie ");
                asql_clean_cmd (pl_connection,pl_cmd);
                return ;
        }
        
        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&vl_NumZone,"@va_Zdp_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : \nasql_ctparam_smallint_input(va_Station_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");	
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}	
	
	
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_Horodate_in,"@va_HorodateDebut_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : asql_ct_param_date_input(date) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
	        return;
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
						XZST_03EcritureTrace(XZSTC_INFO,"ascl07: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd, vl_num_Tab_donnees2,pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd,&vl_SPstatus );
						XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_res_info(msgtype) a echoue");
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");
					asql_clean_cmd (pl_connection,pl_cmd);
					
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");
					asql_clean_cmd (pl_connection,pl_cmd);
					return;
					break;
				}
		}
	
	if(vl_SPstatus == XDC_OK)
	
	{
		vl_Tab = (XDY_Mot *)malloc( vl_Nb_Ligne_Lue*sizeof(XDY_Mot));
		for (vl_indice = 0 ; vl_indice<vl_Nb_Ligne_Lue;vl_indice++)
		{
			vl_Tab[vl_indice]= * (XDY_Mot   *) vl_Tab_Ligne[vl_indice][0].pt_value;
		}	
		vl_Taille=vl_Nb_Ligne_Lue;
	}
	else
	{
		vl_Bool	= XDC_FAUX;
		switch( vl_SPstatus)
		{
			case XDC_NOK : XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : SP a rencontre un prob d acces aux donnees");
			break;
			case XDC_ARG_INV : XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : SP a recu des arguments invalides");
			break;
			case XZASC_RADT_PAS_MES : XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : SP n a pas de mesure a retourner");
			break;
			default : XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : SP a retourne un status non attendu");
			break;
		}
	}
	
	/*A
	** Envoi des donnees aux synoptiques
	*/
	/*A
        ** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
        ** si il y a lieu
        */

        if ((vl_retcode = asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne)) != XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
        }
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : ascl07 : Fin d execution ");
        asql_clean_cmd (pl_connection,pl_cmd);
        
        /************************ XZAS14 **************************************/
	rpc_name = (CS_CHAR *) ACLIC_ASCL14_RPC_NAME;
	
	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 Sortie");		
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");
		asql_clean_cmd (pl_connection,pl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	        
        if ((vl_retcode = asql_ctparam_smallint_input(pl_cmd,(CS_SMALLINT *)&vl_NumZone,"@va_Zdp_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : \nasql_ctparam_smallint_input(va_Station_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");	
		asql_clean_cmd (pl_connection,pl_cmd);
		return ;
	}	
	
	
	/*
	** 6 parametres en output
	*/
	pl_Tab_donnees2[0].pt_value = (CS_VOID *)&vl_seuil_max_out;
	if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)&vl_seuil_max_out,"@va_seuil_max_out"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : \nasql_ctparam_int_output(va_seuil_max_out) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}
	pl_Tab_donnees2[1].pt_value = (CS_VOID *)&vl_seuil_bas_out;
	if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)&vl_seuil_max_out,"@va_seuil_bas_out"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : \nasql_ctparam_int_output(va_seuil_bas_out) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}
	pl_Tab_donnees2[2].pt_value = (CS_VOID *)&vl_seuil_haut_out;
	if ((vl_retcode = asql_ctparam_smallint_output(pl_cmd,(CS_SMALLINT *)&vl_seuil_max_out,"@va_seuil_haut_out"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : \nasql_ctparam_int_output(va_seuil_haut_out) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}
	pl_Tab_donnees2[3].pt_value = (CS_VOID *)vl_NomAutoroute_out;
	if ((vl_retcode = asql_ctparam_char_output(pl_cmd,(CS_CHAR *)vl_NomAutoroute_out,"@va_NomAutoroute_out")) != CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : \nasql_ctparam_char_output(va_NomAutoroute_out) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}
	pl_Tab_donnees2[4].pt_value = (CS_VOID *)&vl_PRdebut_out;
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)&vl_PRdebut_out,"@va_PRdebut_out"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : \nasql_ctparam_int_output(va_PRdebut_out) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}
	pl_Tab_donnees2[5].pt_value = (CS_VOID *)&vl_PRfin_out;
	if ((vl_retcode = asql_ctparam_int_output(pl_cmd,(CS_INT *)&vl_PRfin_out,"@va_PRfin_out"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : \nasql_ctparam_int_output(va_PRfin_out) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}
	pl_Tab_donnees2[6].pt_value = (CS_VOID *)&vl_Sens_out;
	if ((vl_retcode = asql_ctparam_tinyint_output(pl_cmd,(CS_TINYINT *)&vl_Sens_out,"@va_Sens_out"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : \nasql_ctparam_tinyint_output(va_Sens_out) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl06 : sortie");
	  	asql_clean_cmd (pl_connection,pl_cmd);
	   	return ;
	}


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
	        return;
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
						XZST_03EcritureTrace(XZSTC_INFO,"ascl07: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd, vl_num_Tab_donnees2,pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd,&vl_SPstatus );
						XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : CS_STATUS_RESULT");
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"ascl07 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_res_info(msgtype) a echoue");
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"ascl07 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");
					asql_clean_cmd (pl_connection,pl_cmd);
					
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl07: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl07 : sortie");
					asql_clean_cmd (pl_connection,pl_cmd);
					return;
					break;
				}
		}
	
	
	/*A
	** Envoi des donnees aux synoptiques
	*/
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : ascl07 : Fin d execution ");
        asql_clean_cmd (pl_connection,pl_cmd);
        
	if ((vl_retcode = XZIS21_Donnees_TDP(  vl_datagroup_specif,
						vl_NomAutoroute_out,
						vl_PRdebut_out,
						vl_PRfin_out,
						vl_Sens_out,
						vl_seuil_max_out,	
						vl_seuil_bas_out,
						vl_seuil_haut_out,
						(XDY_Octet)vl_Taille,
						vl_Tab))!=XDC_OK)
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"ascl06 : XZIS10_Donnees_TDP a echoue");
        }
        
	return;

}
	

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAS02SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ascl02 	(	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) 

/*
*
* PARAMETRES EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
*
* Retourne la liste des PR debut, PR fin des zones d'influence
* dans l'ordre des PR de debut (croissant si sens sud sinon decroissant)
* puis la liste des mesures avec la date dans l'ordre des PR de debut
* (croissant si sens sud sinon decroissant) et des dates croissantes
*
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
* Select dans les tables MESURE_NIVEAU
*
* MECANISMES :
*	Fonction declenchee sur reception de message XDM_AS_
*	Le message RTwks doit contenir un XDY_Datagroup, XDY_Entier,XDY_PR,
*  					XDY_PR,XDY_Horodate.
*	On appele XZAS;02 qui renvoie des lignes colonnes (Mesures traffic 6min)
*  a renvoyer dans un message sur le DG recu dans le message qui a declenche ce client. 
*
------------------------------------------------------*/

{
	static char *version = "$Id: acli_scl.c,v 1.14 2013/01/29 10:55:46 pc2dpdy Exp $ : acli_ascl02" ;

	XDY_Datagroup   vl_datagroup_specif;
#ifndef _TIBCO_EMS
	T_STR		pl_Datagroup;
#else
	XDY_Datagroup		pl_Datagroup;
#endif

	CS_CHAR		*rpc_name = ACLIC_ASCL02_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	CS_DATAFMT      vl_datafmt;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	CS_DATETIME	vl_HorodateSyb;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	int		vl_Num;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	vl_SPstatus;
	XDY_Autoroute	vl_Autoroute;
	XDY_PR		vl_PRDebut;
	XDY_PR		vl_PRFin;
	XDY_Horodate	vl_Horodate;
	T_INT2		vl_Plage_lue;
	XDY_Octet	vl_Plage;
	
	XZIST_Ligne_INRETS_PR *vl_Localisation;
	XZIST_Ligne_INRETS_Trafic *vl_Trafic;
	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : ascl02 : Debut d execution");
			
	/*A
	** Recuperation du contenu du message XDM_AS_
	*/ 
	if (TipcMsgRead(pa_Data->msg,	T_IPC_FT_STR,	&pl_Datagroup,
					T_IPC_FT_CHAR,	&vl_Autoroute,
					T_IPC_FT_INT4,	&vl_PRDebut,
					T_IPC_FT_INT4,	&vl_PRFin,
					T_IPC_FT_REAL8,	&vl_Horodate,
					T_IPC_FT_INT2, &vl_Plage_lue,
					NULL))
	{
		 XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : ascl02 a recu DG=%s Autoroute =%u du PRDebut %d au PR fin %d a horodate %lf duree=%d ",
						pl_Datagroup,
						vl_Autoroute,
						vl_PRDebut,
						vl_PRFin,
						vl_Horodate,
						(int)vl_Plage_lue);
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : ascl02 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 Sortie");
		return;	 
	}

	/*A
	** Initialisation de la commande				
	*/

	strcpy(vl_datagroup_specif,pl_Datagroup);
	vl_Plage=(XDY_Octet)vl_Plage_lue;
	/*
	 *A conversion de la date au format sybase
	 */
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate,&vl_HorodateSyb)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: asql_date_Ux2Sybase(vl_Horodate,&vl_HorodateSyb) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d",vl_retcode);
		return;
	}
			
	
	/*A
	** Initialisation de la commande				
	*/
	vl_Num=0;
	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
		return;
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return;
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: asql_ctparam_tinyint_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return;
	}
        
	
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&vl_PRDebut,"@va_PRDebut_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: asql_ctparam_int_input(PRDebut) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return;
	}
        
	
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&vl_PRFin,"@va_PRFin_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: asql_ctparam_tinyint_input(PRFin) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return;
	}
        
	
		
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_HorodateSyb,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: asql_ctparam_date_input(datetime) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d",vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return;
	}	

	if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&vl_Plage,"@va_Plage_in"))!= CS_SUCCEED)

	{
		   XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: asql_ctparam_tinyint_input(vl_Plage) a echoue");
		   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
		   asql_clean_cmd(pl_connection, pl_cmd);
		   return;
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
	        return;
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
						vl_Num++;
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,
								"%s: ROW RESULTS nb lignes:%d select %d",
								version,vl_Nb_Ligne_Lue,vl_Num);
						if (vl_Num==1) /* premier select */
						{
						  if (vl_Nb_Ligne_Lue!=0)
						  {
						   vl_Trafic = (XZIST_Ligne_INRETS_Trafic *)malloc(vl_Nb_Ligne_Lue* sizeof(XZIST_Ligne_INRETS_Trafic));
						   if (vl_Trafic == NULL)
						   {
						   	XZST_03EcritureTrace(XZSTC_WARNING,"%s : ascl02: allocation memoire a echouee ",version);
						      	asql_clean_cmd(pl_connection, pl_cmd);
						      	return;
						   }

						   for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						   {
							vl_Trafic[i].numero = *(XDY_Entier *)vl_Tab_Ligne[i][0].pt_value;
							/*
							 *A conversion de la date sybase en unix
							 */
							vl_retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)vl_Tab_Ligne[i][1].pt_value, &vl_Horodate);
							if (vl_retcode != XDC_OK)
							{
								XZST_03EcritureTrace(XZSTC_WARNING,
									"acl02 : asql_Sybase2Ux(va_Horodate_in,&vl_Horodate) a echoue");
								XZST_03EcritureTrace(XZSTC_INTERFACE, 
									"OUT : acl02 retourne %d",vl_retcode);
								return;
							}

							vl_Trafic[i].Horodate = (XDY_Horodate)vl_Horodate;
							vl_Trafic[i].Niveau = *(XDY_Octet *)vl_Tab_Ligne[i][2].pt_value;
						   }
						   vl_retcode=XZIS12_Donnees_INRETS_TRA( pl_Datagroup,
										vl_Autoroute,
										vl_PRDebut,
										vl_PRFin,
										vl_Horodate,
										vl_Nb_Ligne_Lue,
										vl_Trafic);
						   if (vl_retcode != XDC_OK)
								XZST_03EcritureTrace(XZSTC_WARNING,
									"%s : XZIS12 retourne XDC_NOK",version);

						   free(vl_Trafic);
						  }
						   vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,
										vl_Nb_Col,&vl_Tab_Ligne);
						   if (vl_retcode != XDC_OK)
						   {
						      XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
						      asql_clean_cmd(pl_connection, pl_cmd);
						      return;
						   }
						}
						else /* deuxieme select */
						{
						  if (vl_Nb_Ligne_Lue!=0)
						  {
						   vl_Localisation = (XZIST_Ligne_INRETS_PR *)malloc( vl_Nb_Ligne_Lue*sizeof(XZIST_Ligne_INRETS_PR));
						   if (vl_Localisation == NULL)
						   {
						   	XZST_03EcritureTrace(XZSTC_WARNING,"%s : ascl02: allocation memoire a echouee ",version);
						      	asql_clean_cmd(pl_connection, pl_cmd);
						      	return;
						   }

						   for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						   {
							vl_Localisation[i].numero = *(XDY_Entier *)vl_Tab_Ligne[i][0].pt_value;
							vl_Localisation[i].PRDebut = *(XDY_Entier *)vl_Tab_Ligne[i][1].pt_value;
							vl_Localisation[i].PRFin = *(XDY_Entier *)vl_Tab_Ligne[i][2].pt_value;
						   }
						   vl_retcode=XZIS11_Donnees_INRETS_PR( pl_Datagroup,
										vl_Autoroute,
										vl_PRDebut,
										vl_PRFin,
										vl_Horodate,
										vl_Nb_Ligne_Lue,
										vl_Localisation);
						   if (vl_retcode != XDC_OK)
								XZST_03EcritureTrace(XZSTC_WARNING,
									"%s : XZIS11 retourne XDC_NOK",version);

						   free(vl_Localisation);
						  }
						  else 
						  {
						  	vl_retcode=XZIS11_Donnees_INRETS_PR( pl_Datagroup,
						  					vl_Autoroute,
						  					vl_PRDebut,
						  					vl_PRFin,
						  					vl_Horodate,
						  					0,
						  					(XZIST_Ligne_INRETS_PR *) NULL);
						  	if (vl_retcode != XDC_OK)
						  			XZST_03EcritureTrace(XZSTC_WARNING,
						  				"%s : XZIS11 retourne XDC_NOK",version);
						  }
						  
						   vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,
										vl_Nb_Col,&vl_Tab_Ligne);
						   if (vl_retcode != XDC_OK)
						   {
						      XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
						      asql_clean_cmd(pl_connection, pl_cmd);
						      return;
						   }
						}
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"ascl02 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"ascl02 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"ascl02 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne 1");
 						   asql_clean_cmd(pl_connection, pl_cmd);
	        				   return;
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
						asql_clean_cmd(pl_connection, pl_cmd);
	        				return;
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
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
					asql_clean_cmd(pl_connection, pl_cmd);
	        			return;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl02: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
					asql_clean_cmd(pl_connection, pl_cmd);
	        			return;
				}
	}

 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl02 retourne %d", vl_retcode);
 	asql_clean_cmd(pl_connection, pl_cmd);
	return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAS03SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ascl03 	(	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) 

/*
*
* PARAMETRES EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
*
* Retourne la liste des PR debut, PR fin des zones d'influence
* dans l'ordre des PR de debut (croissant si sens sud sinon decroissant)
* puis la liste des mesures avec la date dans l'ordre des PR de debut
* (croissant si sens sud sinon decroissant) et des dates croissantes
*
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
* Select dans les tables MESURE_NIVEAU
*
* MECANISMES :
*	Fonction declenchee sur reception de message XDM_AS_
*	Le message RTwks doit contenir un XDY_Datagroup, XDY_Entier,XDY_PR,
*  					XDY_PR,XDY_Horodate.
*	On appele XZAS;03 qui renvoie des lignes colonnes (Mesures traffic 6min)
*  a renvoyer dans un message sur le DG recu dans le message qui a declenche ce client. 
*
------------------------------------------------------*/

{
	static char *version = "$Id: acli_scl.c,v 1.14 2013/01/29 10:55:46 pc2dpdy Exp $ : acli_ascl03" ;

	XDY_Datagroup   vl_datagroup_specif;
#ifndef _TIBCO_EMS
	T_STR		pl_Datagroup;
#else
	XDY_Datagroup		pl_Datagroup;
#endif

	CS_CHAR		*rpc_name = ACLIC_ASCL03_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	CS_DATAFMT      vl_datafmt;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	CS_DATETIME	vl_HorodateSyb;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	vl_SPstatus;
	XDY_PR 		vl_Num_fmc;
	XDY_Autoroute	vl_Autoroute;
	XDY_PR		vl_PRDebut;
	XDY_PR		vl_PRFin;
	XDY_Horodate	vl_Horodate;
	
	XZIST_Ligne_PRBouchons *vl_Bouchon;

	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : ascl03 : Debut d execution");
			
	/*A
	** Recuperation du contenu du message XDM_AS_
	*/ 
	if (TipcMsgRead(pa_Data->msg,	T_IPC_FT_STR,	&pl_Datagroup,
					T_IPC_FT_CHAR,	&vl_Autoroute,
					T_IPC_FT_INT4,	&vl_PRDebut,
					T_IPC_FT_INT4,	&vl_PRFin,
					T_IPC_FT_REAL8,	&vl_Horodate,
					NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : ascl03 a recu DG=%s Autoroute =%u du PRDebut %d au PR fin %d a horodate %lf ",
						pl_Datagroup,
						vl_Autoroute,
						vl_PRDebut,
						vl_PRFin,
						vl_Horodate);
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : ascl03 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 Sortie");
		return;	 
	}

	/*A
	** Initialisation de la commande				
	*/

	strcpy(vl_datagroup_specif,pl_Datagroup);
			
	/*
	 *A conversion de la date au format sybase
	 */
	if ((vl_retcode = asql_date_Ux2Sybase(vl_Horodate,&vl_HorodateSyb)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: asql_date_Ux2Sybase(vl_Horodate,&vl_HorodateSyb) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d",vl_retcode);
		return;
	}
			
	
	/*A
	** Initialisation de la commande				
	*/
	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
		return;
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
		return;
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_tinyint_input(pl_cmd,(CS_TINYINT *)&vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: asql_ctparam_tinyint_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return;
	}
        
	
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&vl_PRDebut,"@va_PRDebut_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: asql_ctparam_int_input(PRDebut) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return;
	}
        
	
	
	if ((vl_retcode = asql_ctparam_int_input(pl_cmd,(CS_INT *)&vl_PRFin,"@va_PRFin_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: asql_ctparam_tinyint_input(PRFin) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
	   asql_clean_cmd(pl_connection, pl_cmd);
	   return;
	}
        
	
		
	if ((vl_retcode = asql_ctparam_date_input(pl_cmd,(CS_DATETIME *)&vl_HorodateSyb,"@va_HorodateDebut_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: asql_ctparam_date_input(datetime) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d",vl_retcode);
		printf("error de ct_param pour va_Horodate_in = %lf\n",vl_Horodate);
		asql_clean_cmd (pl_connection,pl_cmd);
		return;
	}	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
		asql_clean_cmd(pl_connection, pl_cmd);
	        return;
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
						XZST_03EcritureTrace(XZSTC_INFO,
								"%s: ROW RESULTS nb lignes:%d ",
								version,vl_Nb_Ligne_Lue);
						if (vl_Nb_Ligne_Lue!=0)
						{
						vl_Bouchon = (XZIST_Ligne_PRBouchons *) malloc(vl_Nb_Ligne_Lue*sizeof(XZIST_Ligne_PRBouchons));
						if (vl_Bouchon == NULL)
						{
						      XZST_03EcritureTrace(XZSTC_WARNING,"%s : ascl03: allocation memoire a echouee ",version);
						      asql_clean_cmd(pl_connection, pl_cmd);
						      return;
						}

						for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						{
							/*DEM509*/ 
							/* Recuperation du numero de la FMC */
							vl_Bouchon[i].vl_Num_fmc = *(XDY_Entier*)vl_Tab_Ligne[i][0].pt_value;
							
							/*
							 *A conversion de la date sybase en unix
							 */
							vl_retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)vl_Tab_Ligne[i][1].pt_value, &vl_Horodate);
							if (vl_retcode != XDC_OK)
							{
								XZST_03EcritureTrace(XZSTC_WARNING,
									"acl03 : asql_Sybase2Ux(va_Horodate_in,&vl_Horodate) a echoue");
								XZST_03EcritureTrace(XZSTC_INTERFACE, 
									"OUT : acl03 retourne %d",vl_retcode);
								free(vl_Bouchon);
						      		asql_clean_cmd(pl_connection, pl_cmd);
								return;
							}
							vl_Bouchon[i].HorodateDebut = (XDY_Horodate)vl_Horodate;

							/*
							 *A conversion de la date sybase en unix
							 */
							vl_retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)vl_Tab_Ligne[i][2].pt_value, &vl_Horodate);
							if (vl_retcode != XDC_OK)
							{
								XZST_03EcritureTrace(XZSTC_WARNING,
										"acl03 : asql_Sybase2Ux(va_Horodate_in,&vl_Horodate) a echoue");
								XZST_03EcritureTrace(XZSTC_INTERFACE, 
									"OUT : acl03 retourne %d",vl_retcode);
								free(vl_Bouchon);
						      		asql_clean_cmd(pl_connection, pl_cmd);
								return;
							}
							vl_Bouchon[i].HorodateFin = (XDY_Horodate)vl_Horodate;
	
							vl_Bouchon[i].PRDebut = *(XDY_Entier*)vl_Tab_Ligne[i][3].pt_value;
							vl_Bouchon[i].PRFin = *(XDY_Entier*)vl_Tab_Ligne[i][4].pt_value;
							vl_Bouchon[i].Niveau = *(XDY_Octet*)vl_Tab_Ligne[i][5].pt_value;
						}

						vl_retcode=XZIS13_Donnees_Bouchons( pl_Datagroup,
										vl_Autoroute,
										vl_PRDebut,
										vl_PRFin,
										vl_Horodate,
										vl_Nb_Ligne_Lue,
										vl_Bouchon);
						if (vl_retcode != XDC_OK)
								XZST_03EcritureTrace(XZSTC_WARNING,
									"%s : XZIS12 retourne XDC_NOK",version);
					
						}
						else
						{
							vl_retcode=XZIS13_Donnees_Bouchons( pl_Datagroup,
											vl_Autoroute,
											vl_PRDebut,
											vl_PRFin,
											vl_Horodate,
											0,
											(XZIST_Ligne_PRBouchons *) NULL);
							if (vl_retcode != XDC_OK)
									XZST_03EcritureTrace(XZSTC_WARNING,
										"%s : XZIS12 retourne XDC_NOK",version);
						}
						free(vl_Bouchon);
						vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,
										vl_Nb_Col,&vl_Tab_Ligne);
						if (vl_retcode != XDC_OK)
						{
						      XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
						      asql_clean_cmd(pl_connection, pl_cmd);
						      return;
						}
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"ascl03 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"ascl03 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"ascl03 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne 1");
 						   asql_clean_cmd(pl_connection, pl_cmd);
	        				   return;
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
						asql_clean_cmd(pl_connection, pl_cmd);
	        				return;
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
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
					asql_clean_cmd(pl_connection, pl_cmd);
	        			return;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"ascl03: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
					asql_clean_cmd(pl_connection, pl_cmd);
	        			return;
				}
	}

 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ascl03 retourne %d", vl_retcode);
 	asql_clean_cmd(pl_connection, pl_cmd);
	return;
}
