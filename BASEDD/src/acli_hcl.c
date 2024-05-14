/*E*/
/*Fichier : $Id: acli_hcl.c,v 1.10 2020/11/03 16:04:49 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/11/03 16:04:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE AHCL * FICHIER acli_hcl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*	contient un callBack de TACLI
*
*  ahcl11 : positionne XZAH10G_Init a XDC_VRAI.
*
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	31 Oct 1994	: Creation
* gaborit	11 jan 1995	: ajout ahcl21 et ahcl26 (1.2)
* gaborit	16 jan 1995	: ajout ahcl27 (1.3)
* T.Milleville  11 Jul1995	: Suppression reprogrammation Timer 
*	car delai a l'init V1.4
* T.Milleville  17 Jul1995	: Suppression Lecture des arguments du 
*	msg RADT_DAI + remise lecture V1.5
* P.NIEPCERON   19 Oct 1998	: Suppression de Satir dem/1700 1.7
* JMG	21/02/2012 : ajout hcl980 dem/1015
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_hcl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_hcl.c,v 1.10 2020/11/03 16:04:49 pc2dpdy Exp $ : acli_hcl" ;

/* declaration de fonctions internes */
extern XDY_NomSite vg_NomSite;
/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  ahcl11 : positionne XZAH10G_Init a XDC_VRAI.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ahcl11(	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) 

/*
* ARGUMENTS EN ENTREE :
*
*	Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*	Initialise la variable globale XZAH10G_Init a XDC_VRAI.
*	Permet de realsier un raffraichissement global de l IHM.
*	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_hcl.c,v 1.10 2020/11/03 16:04:49 pc2dpdy Exp $ : acli_acl11" ;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"In  : ahcl11 : Debut d execution");
	/*A
	** Recuperation du contenu du message XDM_A_INIT_IHM
	*/ 
	if (TipcMsgRead(pa_Data->msg,NULL))
	{
		XZAH10G_Init = XDC_VRAI;
		XZST_03EcritureTrace(XZSTC_DEBUG1,"tacli : ahcl11 : initialisation globale de l IHM demandee");	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : ahcl11 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ahcl11 Sortie");
		return;	 
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : ahcl11 : Fin d execution ");
        return ;

}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  ahcl21 : appelle xzah21 pour envoyer les voies inversees radt dai
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ahcl21(	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) 

/*
* ARGUMENTS EN ENTREE :
*
*	Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*   appele sur timer
*
* FONCTION 
*
*  ahcl21 : appelle xzah21 pour envoyer les voies inversees radt dai
*	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_hcl.c,v 1.10 2020/11/03 16:04:49 pc2dpdy Exp $ : acli_ahcl21" ;
	
	int		vl_retcode;
        XDY_Datagroup           vl_datagroup;
        XDY_NomSite             pl_NomSite;
        int                     vl_JourSemaine=0;
        double                  vl_HorodateSec=0;
        int                     vl_Nb_Secondes=0;
        int                     vl_ValRet;
#ifndef _TIBCO_EMS
	T_STR			vl_Cle	= "";
#else
	char			vl_Cle[128]	= "";
#endif
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"In  : %s : Debut d execution",version);
	/*A
	** Recuperation du contenu du message XDM_AH_RADT_DAI
	*/ 
	if (TipcMsgRead(pa_Data->msg,
		T_IPC_FT_STR,   &vl_Cle,
		NULL))
	{

		XZST_03EcritureTrace(XZSTC_DEBUG1,"%s : appele par timer",version);
		vl_retcode=XZAH21_VoiesDaiRadt(XDC_CHAINE_VIDE);
		if (vl_retcode!=XDC_OK)
			{
			XZST_03EcritureTrace(XZSTC_WARNING ," %s : XZAH21 retourne NOK ",version);
			}
		else
			XZST_03EcritureTrace(XZSTC_DEBUG1 ," %s : XZAH21 retourne OK ",version);
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," %s : TipcMsgRead a echoue",version);	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s Sortie",version);
		return;	 
	}


        /*A
        ** Reprogrammation du timer
        */

	strcpy(pl_NomSite, vg_NomSite);

        /*A
        ** Constitution du nom Data Group a partir du nom du site
        */

        XDG_EncodeDG2(vl_datagroup,XDG_AH_RADT_DAI,pl_NomSite);


        if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZSM_07LireHorodate a echoue",version);
                acli_sortir();
        }

        vl_Nb_Secondes = vl_HorodateSec + AHCLC_TIMERDELAI_AHCL21;

        vl_ValRet = XZSM_01DemanderProgrammation(
                                                vl_Nb_Secondes,
                                                0,
                                                vl_datagroup,
                                                XDM_AH_RADT_DAI,
                                                AHCLC_TIMERCLE_AHCL21,
                                                XZSMC_SEC_VRAI );
        if (vl_ValRet != XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZSM_01 : Probleme de programmation TIMER ",version);
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s : Sortie",version);
                acli_sortir();
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_DEBUG1,"%s : XZSM_01: SUCCES",version);
        }



	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s : Fin d execution ",version);
        return ;

}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  ahcl26 : appelle xzah21 pour envoyer les voies inversees radt dai
*           sur une demande d'init
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ahcl26(	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) 

/*
* ARGUMENTS EN ENTREE :
*
*	Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*   appele sur timer
*
* FONCTION 
*
*  ahcl26 : appelle xzah21 pour envoyer les voies inversees radt dai
*	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_hcl.c,v 1.10 2020/11/03 16:04:49 pc2dpdy Exp $ : acli_ahcl26" ;
	
	XDY_NomMachine 	vl_NomMachine;
	int		vl_retcode;
#ifndef _TIBCO_EMS
	T_STR		pl_NomMachine;
#else
	XDY_NomMachine		pl_NomMachine;
#endif
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"In  : %s : Debut d execution",version);
	/*A
	** Recuperation du contenu du message XDM_AH_INITLT
	*/ 
	if (TipcMsgRead(pa_Data->msg,	T_IPC_FT_STR,   &pl_NomMachine,
					NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"%s : appel� pour init du lt %s",version,pl_NomMachine);
		strcpy(vl_NomMachine,pl_NomMachine);
		vl_retcode=XZAH21_VoiesDaiRadt(vl_NomMachine);
		if (vl_retcode!=XDC_OK)
			{
			XZST_03EcritureTrace(XZSTC_WARNING ," %s : XZAH21 retourne NOK ",version);
			}
		else
			XZST_03EcritureTrace(XZSTC_DEBUG1 ," %s : XZAH21 retourne OK ",version);
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," %s : TipcMsgRead a echoue",version);	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s Sortie",version);
		return;	 
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s : Fin d execution ",version);
        return ;

}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	ahcl99 : recoit le message RTWks XDM_AA_alerte, recupere les informations
*	les met en forme Sybase et appelle la SP XZAE141.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ahcl99 	(	T_IPC_CONN 			va_Cnx,
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
	static char *version = "@(#)acli_acl.c	1.4 05/14/97 : acli_hcl99" ;
	
	/* Variables locales pour messages RTWks */
#ifndef _TIBCO_EMS
	T_STR 		vl_reference;
	T_STR		vl_type;
	T_STR		vl_date;
	T_STR		vl_ligne;
	T_INT4		vl_version;
#else
	char 		vl_reference[512];
	char		vl_type[512];
	char		vl_date[512];
	char		vl_ligne[512];
	XDY_Entier		vl_version;
#endif
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = ACLIC_XZAE141_RPC_NAME;	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	XDY_Entier      vl_status=0;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s",version);
	
	/*A
	** Recuperation du contenu du message XDM_AS_EVT_STRADA
	*/ 
	if (TipcMsgRead(pa_Data->msg,	T_IPC_FT_STR, &vl_reference, 
					T_IPC_FT_INT4, &vl_version,
					T_IPC_FT_STR, &vl_date,
					T_IPC_FT_STR, &vl_ligne,
					T_IPC_FT_STR, &vl_type, 
					NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"tacli : ahcl99 a recu  date = %s\n ref = %s\n version = %d\n Type= %s\n Texte = %s\n",
									vl_date,
									vl_reference,
									vl_version,
									vl_type,
									vl_ligne);	
	}
	else
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : ahcl99 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ahcl99 Sortie");
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
	 	XZST_03EcritureTrace(XZSTC_FATAL,"ahcl99 : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ahcl99 Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"hcl99: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_reference,"@va_reference_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"hcl99 : asql_ct_param_char_input(reference) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
	   	return ;
}	
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_version,"@va_version_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"hcl99 : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_date,"@va_date_in"))!= CS_SUCCEED)      
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"hcl99 : asql_ct_param_char_input(date) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);

                return ;
        }
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_ligne,"@va_ligne_in"))!= CS_SUCCEED)      
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"hcl99 : asql_ct_param_char_input(ligne) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);

                return ;
        }
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_type,"@va_type_in"))!= CS_SUCCEED)      
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"hcl99 : asql_ct_param_char_input(type) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);

                return ;
        }
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"hcl99 : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 sort ");		
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
						XZST_03EcritureTrace(XZSTC_INFO,"hcl99: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"hcl99 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd,&vl_status );
						XZST_03EcritureTrace(XZSTC_INFO,"hcl99 : XZAA01SP retourne %d",vl_status);
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_FATAL,"hcl99 : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");
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
						XZST_03EcritureTrace(XZSTC_FATAL,"hcl99: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
						return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"hcl99 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"hcl99 : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"hcl99 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"hcl99: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"hcl99: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : hcl99 : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
	XZST_03EcritureTrace(XZSTC_WARNING,"hcl99: POK_EVT_STRADA");
	XZIT12_Evt_Strada();
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_WARNING,"OUT %s",version);
        return;
}




/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	ahcl980 : recoit le message RTWks XDM_AA_COMM_FMC recupere les informations
*	les met en forme Sybase et appelle la SP XZAE980.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ahcl980 	(	T_IPC_CONN 			va_Cnx,
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
	static char *version = "$Id: acli_hcl.c,v 1.10 2020/11/03 16:04:49 pc2dpdy Exp $ $Revision: 1.10 $ $Date: 2020/11/03 16:04:49 $ : acli_hcl980" ;
	
	/* Variables locales pour messages RTWks */
#ifndef _TIBCO_EMS
	T_INT4	*tl_numeros;
	T_INT4	*tl_syntheses;
	T_INT4	*tl_tendances;
	T_INT4	vl_taille;
	T_INT4	vl_tailleS;
	T_INT4	vl_tailleT;
#else
	XDY_Entier	tl_numeros[512];
	XDY_Entier	tl_syntheses[512];
	XDY_Entier	tl_tendances[512];
	XDY_Entier	vl_taille;
	XDY_Entier	vl_tailleS;
	XDY_Entier	vl_tailleT;
#endif

	int vl_indice;
	XDY_Evt                vl_evt;
	XDY_Booleen            vl_en_cours;
	XDY_Synthese_Commu vl_synthese;
	XDY_Tendance_Commu vl_tendance;
	/*A
	** Recuperation du contenu du message XDM_AS_COMMU_FMC
	*/ 
	if (!TipcMsgRead(pa_Data->msg, T_IPC_FT_INT4_ARRAY, &tl_numeros, &vl_taille,
					T_IPC_FT_INT4_ARRAY, &tl_syntheses, &vl_tailleS,
					T_IPC_FT_INT4_ARRAY, &tl_tendances, &vl_tailleT,
					NULL))
	{
		/*B 
		** Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," tacli : ahcl980 : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : ahcl980 Sortie");
		return;	 
	}

	 XZST_03EcritureTrace(XZSTC_DEBUG1 ,"TAILLE %d",vl_taille);
	for (vl_indice=0; vl_indice < vl_taille; vl_indice++) {
		XZST_03EcritureTrace(XZSTC_DEBUG1 ," tacli : ahcl980 EVT %d ",
						*tl_numeros);
		if (XZAE980_Recherche_FMC_Liee(*tl_numeros,
					*tl_syntheses,*tl_tendances,
					&vl_evt,&vl_en_cours,
					&vl_synthese,&vl_tendance) == XDC_OK) {
		/*retour sur XDG_COMMUNAUTAIRE*/
#ifndef _TIBCO_EMS
		if (TipcSrvMsgWrite(XDG_COMMUNAUTAIRE,
					XDM_IdentMsg(XDM_COMMU_FMC),
					XDM_FLG_SRVMSGWRITE,
					T_IPC_FT_INT4,*tl_numeros,
					T_IPC_FT_INT4,vl_evt.numero,
					T_IPC_FT_INT2,vl_evt.cle,
					T_IPC_FT_BOOL,vl_en_cours,
					T_IPC_FT_INT4,vl_synthese,
					T_IPC_FT_INT4,vl_tendance,
					NULL))
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,
					"message XDM_COMMU_FMC envoye %d %d %d %d %d %d ",
					*tl_numeros,
					vl_evt.numero,
					vl_evt.cle,
					vl_en_cours,
					vl_synthese,
					vl_tendance);
			TipcSrvFlush();
		}
#else
		if (TipcSrvMsgWrite(XDG_COMMUNAUTAIRE,
					XDM_IdentMsg(XDM_COMMU_FMC),
					XDM_FLG_SRVMSGWRITE,
					T_IPC_FT_INT4,tl_numeros[vl_indice],
					T_IPC_FT_INT4,vl_evt.numero,
					T_IPC_FT_INT2,vl_evt.cle,
					T_IPC_FT_BOOL,vl_en_cours,
					T_IPC_FT_INT4,vl_synthese,
					T_IPC_FT_INT4,vl_tendance,
					NULL))
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,
					"message XDM_COMMU_FMC envoye %d %d %d %d %d %d ",
					tl_numeros[vl_indice],
					vl_evt.numero,
					vl_evt.cle,
					vl_en_cours,
					vl_synthese,
					vl_tendance);
			TipcSrvFlush();
		}
#endif
		else {
			 XZST_03EcritureTrace(XZSTC_WARNING ,"message  XDM_COMMU_FMC non envoye");
		}
		}
#ifndef _TIBCO_EMS
		tl_numeros++;
		tl_syntheses++;
		tl_tendances++;
#endif

	}

	XZST_03EcritureTrace(XZSTC_DEBUG1," %s",version);
        return;
}
