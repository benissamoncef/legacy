/*  Fichier : 	$Id: xzat511.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat511.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	13/11/23	:	Creation (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
#include "xzat.h"

static char *version = "@(#)xzat511.c : xzat511" ;

/* declaration de fonctions internes */
/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAT511SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAT511_Utilisation_PIC  ( 	XDY_Horodate		va_Horodate_in,
								XDY_Mot				va_NumEqt_in,
								XDY_Octet 			*va_DispoPIC_out,
								XDY_Bandeau_PIC		*va_Bandeau_out,
								XDY_Booleen			*va_Flash_out,
								XDY_Picto_PIC		*va_Picto_out,								
								XDY_Entier 			*va_NumEvt_out,
								XDY_Octet 			*va_CleEvt_out,
								XDY_Octet 			*va_Priorite_out,
								XDY_Entier 			*va_DistanceEvt_out,
								XDY_Entier 			*va_Action_out,
								XDY_Octet 			*va_SiteAction_out)
/*-----------------------------------------------------
* Sequence d'appel
* CL	XZAT511_Utilisation_PIC
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Mot		va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet			va_DispoPIC_out
* XDY_Bandeau_PIC	va_Bandeau_out
* XDY_Booleen		va_Flash_out
* XDY_Picto			va_Picto_out
* XDY_Entier		va_NumEvt_out
* XDY_Octet			va_CleEvt_out
* XDY_Octet			va_Priorite_out
* XDY_Entier		va_DistanceEvt_out
* XDY_Entier		va_Action_out
* XDY_Site			va_SiteAction_out

* Service rendu
* Retourne l'utilisation reelle d'un PIC a une horodate donnee.
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
* select dans les tables EQUIPEMENT, E_PIC, ACTION et INDISPONIBILITE
* 
------------------------------------------------*/
{
	static char *version = "@(#)xzat511.c : XZAT511_Utilisation_PIC" ;

	CS_CHAR *rpc_name 	= XZATC_XZAT511_RPC_NAME;
	CS_CONTEXT      	*context;
	CS_CONNECTION   	*connection=XZAGV_Connexion;
	CS_COMMAND			*cmd;
	CS_DATAFMT      	datafmt;
	CS_RETCODE      	retcode = 0;
	CS_INT          	res_type;
	CS_SMALLINT     	msg_id;
	int 				num_Tab_donnees2 = XZATC_XZAT511_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZATC_XZAT511_NB_PARM_RETURN];
	XDY_Entier 			SPstatus;
	CS_DATETIME			vl_Horodate;
	XDY_Entier 			vl_Action;
	XDY_Octet			vl_SiteAction;
	int             	vl_Nb_Ligne_Lue = 0;
	int             	vl_Nb_Col = 0;
	tg_row_result   	vl_Tab_Ligne;
	
	/* Conversion des chaines de caracteres en entree du format Unix au format Sybase */
	if ((retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		return (XDC_NOK);
	}
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAT511_Utilisation_PIC : Jours = %d\tHeure = %d\tNumEqt = %d\n" , vl_Horodate.dtdays,vl_Horodate.dttime,va_NumEqt_in );
			
	/* Initialisation de la commande */
	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Envoie de la commande RPC pour notre stored procedure. */
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/* Initialisation de la structure pour chacun des parametres d I/O a faire passer a la rpc. */
	if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat51: asql_ctparam_date_input(Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
        
	Tab_donnees2[0].pt_value = (CS_VOID *)va_DispoPIC_out;
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_DispoPIC_out,"@va_DispoPIC_out")) != CS_SUCCEED)	
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_tinyint_output(va_DispoPIC_out) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	Tab_donnees2[1].pt_value = (CS_VOID *)va_Bandeau_out;
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Bandeau_out,"@va_Bandeau_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_char_output(va_Bandeau_out) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[2].pt_value = (CS_VOID *)va_Flash_out;
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)va_Flash_out,"@va_Flash_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_bit_output(va_Flash_out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[3].pt_value = (CS_VOID *)va_Picto_out;
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Picto_out,"@va_Picto_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_char_output(va_Picto_out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[4].pt_value = (CS_VOID *)va_NumEvt_out;
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_NumEvt_out,"@va_NumEvt_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_int_output(va_NumEvt_out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	Tab_donnees2[5].pt_value = (CS_VOID *)va_CleEvt_out;
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_CleEvt_out,"@va_CleEvt_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_tinyint_output(CleEvt) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[6].pt_value = (CS_VOID *)va_Priorite_out;
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Priorite_out,"@va_Priorite_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_tinyint_output(va_Priorite_out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[7].pt_value = (CS_VOID *)va_DistanceEvt_out;
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistanceEvt_out,"@va_DistanceEvt_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_int_output(va_DistanceEvt_out) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	Tab_donnees2[8].pt_value = (CS_VOID *)&vl_Action;
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&vl_Action,"@va_Action_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_int_output(vl_Action) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	Tab_donnees2[9].pt_value = (CS_VOID *)&vl_SiteAction;
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_SiteAction,"@va_SiteAction_out")) != CS_SUCCEED)	
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: asql_ctparam_tinyint_output(vl_SiteAction) a echoue "); 
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/* Envoie de la commande au Sql server */
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	  	return (XDC_NOK);
	}

	/* Traitement du resultat */
	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/* Impression de l entete en fonction du type de resultat. */
				switch ((int)res_type)
				{
					case  CS_ROW_RESULT:
						retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat511: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat511 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat511 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat511 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						if (SPstatus != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne 1");
 							asql_clean_cmd(connection, cmd);
							return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat511 : asql_fetch_* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
				break;
			
			case CS_MSG_RESULT:

				retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: ct_res_info(msgtype) a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	  				return (XDC_NOK);
				}
				XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat511 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
				break;

			case CS_CMD_SUCCEED:
				/* Pas de colonnes retournees. */
				XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat511 : CS_CMD_SUCCEED \n");
				break;

			case CS_CMD_DONE:
				/* Commande effectuee : positionnement des parametres de retour. */
				XZST_03EcritureTrace(XZSTC_INTERFACE,"xzat511 : CS_CMD_DONE \n");
				break;

			case CS_CMD_FAIL:
				/* Le serveur SQL a detecte une ereeur en executant notre commande. */
				XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
	  			return (XDC_NOK);

			default:
				/* Il s est passe quelque chose d inattendu. */
				XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
	  			return (XDC_NOK);
		}
	}

	if (vl_Nb_Ligne_Lue > 0) 
	{
		*va_DispoPIC_out  = * (XDY_Octet *) vl_Tab_Ligne[0][0].pt_value;
		strcpy(va_Bandeau_out,(char *) vl_Tab_Ligne[0][1].pt_value);
		strcpy(va_Flash_out, (char *) vl_Tab_Ligne[0][2].pt_value);
		strcpy(va_Picto_out, (char *) vl_Tab_Ligne[0][3].pt_value);
		*va_NumEvt_out  = * (XDY_Entier *) vl_Tab_Ligne[0][4].pt_value;
		*va_CleEvt_out  = * (XDY_Octet *) vl_Tab_Ligne[0][5].pt_value;
		*va_Priorite_out = * (XDY_Octet *) vl_Tab_Ligne[0][6].pt_value;
		*va_DistanceEvt_out = * (XDY_Octet *) vl_Tab_Ligne[0][7].pt_value;
		*va_Action_out = * (XDY_Entier *) vl_Tab_Ligne[0][8].pt_value;
		*va_SiteAction_out = * (XDY_Octet *) vl_Tab_Ligne[0][9].pt_value;

		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: %s // %s // %s",(char *) vl_Tab_Ligne[0][1].pt_value,(char *) vl_Tab_Ligne[0][2].pt_value,(char *) vl_Tab_Ligne[0][3].pt_value);
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: %s // %s ",va_Bandeau_out,va_Flash_out);
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat511: %s",va_Picto_out);
		XZST_03EcritureTrace(XZSTC_INTERFACE,"XZAT51 : %d %d",*va_Priorite_out,*va_Action_out );
	}

	/* Retourne le resultat et les parametres de sorties renseignes */
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT511_Utilisation_PIC retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
