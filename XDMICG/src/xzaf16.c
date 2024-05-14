/*E*/
/* Fichier : $Id: xzaf16.c,v 1.1 1995/01/23 19:28:13 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/23 19:28:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzaf16.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       23/01/95     : Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf16.h"

static char *version = "$Id: xzaf16.c,v 1.1 1995/01/23 19:28:13 torregrossa Exp $ XZAF16_Lire_Niveaux_Station";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Rechercher les niveaux d'une station de comptage
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAF16_Lire_Niveaux_Station (	XDY_Mot		va_Station,
					XDY_Horodate	va_Horodate,
					XDY_Sens	*va_Sens1,
					XDY_Octet	*va_Niveau1,
					XDY_Sens	*va_Sens2,
					XDY_Octet	*va_Niveau2)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot		va_Station
* XDY_Horodate		va_Horodate
*
* ARGUMENTS EN SORTIE :
*
* XDY_Sens		va_Sens1
* XDY_Octet		va_Niveau1
* XDY_Sens		va_Sens2
* XDY_Octet		va_Niveau2
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* Appelle XZAF16SP
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzaf16.c,v 1.1 1995/01/23 19:28:13 torregrossa Exp $ XZAF16_Lire_Niveaux_Station";
	
	CS_CHAR 	*vl_RpcName =XZAFC_XZAF16_RPC_NAME;
	CS_CONNECTION   *vl_Connection=XZAGV_Connexion;
	CS_COMMAND	*vl_Cmd;
	CS_RETCODE      vl_Retcode;
	CS_INT          vl_ResType;
	CS_SMALLINT     vl_MsgId;
	int 		vl_NumTabDonnees2 = XZAFC_XZAF16_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	int		i;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables intermediaires pour la conversion des dates au format Sybase
	*/
	CS_DATETIME	vl_Horodate_in;
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAF16_Lire_Niveaux_Station : va_Station=%d va_Horodate=%lf &va_Sens1=%x &va_Niveau1=%x &va_Sens2=%x &va_Niveau2=%x",
			va_Station, va_Horodate, va_Sens1, va_Niveau1, va_Sens2, va_Niveau2);
	
	if (va_Sens1 == NULL || va_Sens2 == NULL || va_Niveau1 == NULL || va_Niveau2 == NULL)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: pointeur en entree NULL");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
					XDC_NOK);
		return (XDC_NOK);
	}
	
        /*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_Retcode = asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: asql_date_Ux2Sybase(va_Horodate,&vl_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
					vl_Retcode);
		return (XDC_NOK);
	}
	
	
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/

	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2) * vl_NumTabDonnees2);

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_Retcode = ct_command(vl_Cmd, CS_RPC_CMD, vl_RpcName, 
				CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_Retcode = asql_ctparam_smallint_input(vl_Cmd,(CS_SMALLINT *)&va_Station,
						"@va_Station_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: asql_ctparam_smallint_input(Station) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_date_input(vl_Cmd,(CS_DATETIME *)&vl_Horodate_in,
						"@va_Horodate_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Sens1;
	if ((vl_Retcode = asql_ctparam_tinyint_output(vl_Cmd,(CS_TINYINT *)va_Sens1,
						"@va_Sens1_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: asql_ctparam_tinyint_output(Sens1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	Tab_donnees2[1].pt_value = (CS_VOID *)va_Niveau1;
	if ((vl_Retcode = asql_ctparam_tinyint_output(vl_Cmd,(CS_TINYINT *)va_Niveau1,
						"@va_Niveau1_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: asql_ctparam_tinyint_output(Niveau1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	Tab_donnees2[2].pt_value = (CS_VOID *)va_Sens2;
	if ((vl_Retcode = asql_ctparam_tinyint_output(vl_Cmd,(CS_TINYINT *)va_Sens2,
						"@va_Sens2_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: asql_ctparam_tinyint_output(Sens2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	Tab_donnees2[3].pt_value = (CS_VOID *)va_Niveau2;
	if ((vl_Retcode = asql_ctparam_tinyint_output(vl_Cmd,(CS_TINYINT *)va_Niveau2,
						"@va_Niveau2_out"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: asql_ctparam_tinyint_output(Niveau2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	
	/*A
	** Traitement du resultat
	*/

	while ((vl_Retcode = ct_results(vl_Cmd, &vl_ResType)) == CS_SUCCEED)
	{
		switch ((int)vl_ResType)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_ResType)
				{
					case  CS_ROW_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf16: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_Retcode = asql_fetch_param_result (vl_Cmd,
										vl_NumTabDonnees2, 
										Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf16 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf16 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf16 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne 1");
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
						   free(Tab_donnees2);
						   return (XDC_NOK);
						}
						break;
				}
				
				
				if (vl_Retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: asql_fetch_data a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_Retcode = ct_res_info(vl_Cmd, CS_MSGTYPE,(CS_VOID *)&vl_MsgId, CS_UNUSED, NULL);
					if (vl_Retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", vl_Retcode);
						asql_clean_cmd(vl_Connection, vl_Cmd);
						free(Tab_donnees2);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf16: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF16_Lire_Niveaux_Station retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : xzaf16 : sens1=%d sens2=%d niveau1=%d niveau2=%d \n",
			*va_Sens1, *va_Sens2, *va_Niveau1, *va_Niveau2);
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAF16_Lire_Niveaux_Station retourne %d\n", XDC_OK);
			
 	
 	asql_clean_cmd(vl_Connection, vl_Cmd);
	free(Tab_donnees2);

	return(XDC_OK);
}

