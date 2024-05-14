/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae207.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* nature FMC (dans FMC_NAT)
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 835
* ABE   19/08/20        Correction variable de sortie char sur curseur pour tagen/tada2 DEM-SAE130 V1.2
* JPL	21/02/24	Liberation dans tous les cas de la memoire allouee pour "select" (SAE-581)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae207.h"

static char *version = "xzae207.c 1.3" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE207SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE207_Lit_Nature ( XDY_Entier  va_evenement_in,
				   XDY_Octet   va_cle_in,
				      XDY_Octet	*va_code_nature_out,
				      char		*va_libelle_nature_out)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Fonction  pa_FonctionUtilisateur_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* retourne la liste des fmc a exporter vers strada
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Pour chaque zone de parcours appel de la fonction utilisateur avec XDY_Destination
*
* MECANISMES :
*
------------------------------------------------------*/

{
	CS_CHAR *rpc_name = XZAE207C_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int             vl_num_Tab_donnees2 = XZAE207C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAE207C_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne = NULL;
	int		i = 0;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: %d / %d",va_evenement_in,va_cle_in);
				
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(pl_connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207_Lit_Nature retourne %d", retcode);
		asql_clean_cmd(pl_connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207_Lit_Nature retourne %d", retcode);
		asql_clean_cmd(pl_connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/
        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_evenement_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: asql_ctparam_int_input(va_NumEvenement_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207_Lit_Nature retourne %d", retcode);
	   asql_clean_cmd(pl_connection, cmd);
	   return (XDC_NOK);
	}
        
        if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_cle_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: asql_ctparam_tinyint_input(cle) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207_Lit_Nature retourne %d", retcode);
	   asql_clean_cmd(pl_connection, cmd);
	   return (XDC_NOK);
	}
       
	Tab_donnees2[i].pt_value = (CS_VOID *)va_code_nature_out;

        if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_code_nature_out,"@va_code_nature_out")) != CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: asql_ctparam_tinyint_output(code nature) a echoue ");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207_Lit_Nature retourne %d", retcode);
           asql_clean_cmd(pl_connection, cmd);
           return (XDC_NOK);
        }
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_libelle_nature_out;

	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_libelle_nature_out,"@va_libelle_nature_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: asql_ctparam_char_output(libelle nature) a echoue ");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207_Lit_Nature retourne %d", retcode);
		asql_clean_cmd(pl_connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if ((retcode = ct_send(cmd)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207_Lit_Nature retourne %d", retcode);
		asql_clean_cmd(pl_connection, cmd);
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
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae207: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae207 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae207 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae207 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAE207 retourne 1");
 						   asql_clean_cmd(pl_connection, cmd);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207_Lit_Nature retourne %d", retcode);
						asql_clean_cmd(pl_connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae207: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzae207: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207_Lit_Nature retourne %d", retcode);
					asql_clean_cmd(pl_connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE207 retourne %d", retcode);
					asql_clean_cmd(pl_connection, cmd);
					return (XDC_NOK);
				}
	}
	
	//Récupération de la varialbe de sortie en char ave cle curseur
	strcpy(va_code_nature_out,  (XDY_Octet *)vl_Tab_Ligne[0][0].pt_value);
	strcpy(va_libelle_nature_out,  (char *)vl_Tab_Ligne[0][1].pt_value);
	
	/*A
	** Appel de la fonction utilisateur pour chaque FMC 
	*/
	XZST_03EcritureTrace(XZSTC_WARNING,"xzae207: code nature %d / libelle nature %s",va_code_nature_out,va_libelle_nature_out);


	/*
	** Desallocation du tableau des lignes renvoyees par "select" dans la procedure
	*/

	asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INFO, "OUT : XZAE207 retourne %d", retcode);
 	asql_clean_cmd(pl_connection, cmd);
	return(XDC_OK);
}
