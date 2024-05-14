/*E*/
/*  Fichier : 	%W%  Release : %I% Date : %G%
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat71.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/01/18 : creation GABEB 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"
#include "xzat71.h"

static char *version = "@(#)xzat71.c	1.2 12/19/94 : xzat71" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAT71SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAT71_Etat_CAB  ( 	XDY_Horodate	va_Horodate_in,
				XDY_Mot		va_NumEqt_in,
				XDY_Octet 	*va_Dispo_out)

				

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* CL	XZAT71_Etat_CAB
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Eqt	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet	va_DispoPRV_out
* 
* Service rendu
* Retourne l'utilisation reelle d'une PRV � une horodate donn�e.
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* select dans les tables EQUIPEMENT, E_PRV, ACTION et INDISPONIBILITE
* 
------------------------------------------------*/


{

	static char *version = "@(#)xzat71.c	1.2 12/19/94 : XZAT71_Etat_CAB" ;

	CS_CHAR *rpc_name = XZATC_XZAT71_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZATC_XZAT71_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	XDY_Entier 	vl_Action;
	XDY_Octet	vl_SiteAction;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	tg_row_result   vl_Tab_Ligne;
	char vl_vitesse[10]="";
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAT71_Etat_CAB : Jours = %d\tHeure = %d\tNumEqt = %d\n" , vl_Horodate.dtdays,vl_Horodate.dttime,va_NumEqt_in );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzat71: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat71: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat71: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat71: asql_ctparam_smallint_input(NumEqt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Dispo_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Dispo_out,"@va_Dispo_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat71: asql_ctparam_tinyint_output(Dispo) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat71: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
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
						retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat71: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat71 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat71 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat71 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	  					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzat71 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	  				return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzat71: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
	  					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat71 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat71 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat71 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat71: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	  				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat71: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	  				return (XDC_NOK);
				}
	}
	if (vl_Nb_Ligne_Lue > 0) {
		*va_Dispo_out  = * (XDY_Octet *) vl_Tab_Ligne[0][0].pt_value;
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT71_Etat_CAB retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}




