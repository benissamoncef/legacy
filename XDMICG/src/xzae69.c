/*E*/
/*  Fichier : 	$Id: xzae69.c,v 1.6 1995/04/24 15:11:04 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1995/04/24 15:11:04 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae69.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       12 Oct 1994     : Creation
* volcic       13 Oct 1994     : Modifications pour tests SAIDEC (v1.2)
* volcic       13 Oct 1994     : Modifications pour tests SAIDEC (v1.3)
* volcic       13 Oct 1994     : Modifications pour tests SAIDEC (v1.4)
* GABORIT       24 avr 95       : suppr taddonnees2 (1.5)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae69.h"

static char *version = "$Id: xzae69.c,v 1.6 1995/04/24 15:11:04 gaborit Exp $ : xzae69" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE69SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE69_Liste_Causes (	XDY_Evt		va_NumEvt,
				XDY_Horodate	va_Horodate,
				XDY_Fonction	va_FonctionUtilisateur_in )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate
* XDY_Evt		va_NumEvt
* XDY_Fonction		va_FonctionUtilisateur	
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Liste des consequences d'un evenement (en profondeur)
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* En formation, preciser l'horodate
*
* FONCTION :
*
* Appel la fonction utilisateur pour chaque consequence trouvee
*
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR *rpc_name = XZAEC_XZAE69_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Evt		vl_Evt;
	XDY_Mot		vl_Type;
	
   	/*A
	** Definition des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	XDY_Entier	va_NumEvt_in;
	XDY_Octet	va_CleEvt_in;
	
	/*A
	** Definition des variables intermediaires pour la conversion des dates au format Sybase
	*/
	
	CS_DATETIME	va_Horodate_in;
	
	
	/*A
	** Conversion des dates en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in))!= XDC_OK)
        {

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
		return (XDC_NOK);
	}  
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_NumEvt_in     = va_NumEvt.numero;
	va_CleEvt_in     = va_NumEvt.cle;
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAE69_Liste_Causes : va_Horodate=%d va_Time=%d va_NumEvt_in=%d va_CleEvt_in=%d",va_Horodate_in.dtdays,va_Horodate_in.dttime,va_NumEvt_in,va_CleEvt_in);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzae69: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE69_Ajouter_PMVPicto retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzae69: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAE69_Ajouter_PMVPicto retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in")) != CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: asql_ctparam_int_input(va_NumEvt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in")) != CS_SUCCEED)
	 
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: asql_ctparam_tinyint_input(va_CleEvt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&va_Horodate_in,"@va_Horodate_in")) != CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: asql_ctparam_date_input(va_Horodate_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
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
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae69: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzae69 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae69 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae69 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzae69 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae69: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   			 	return (XDC_NOK);
		}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateur pour chaque consequence trouvee
	** Affectation des parametres de sortie
	*/
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           /*B
           ** Remplissage des variables argument de la fonction utilisateur
           ** correspondant a la ligne resultat du select de la procadure stockee XZAE;69
           */
           
           vl_Evt.numero	= * (XDY_Entier *) vl_Tab_Ligne[i][0].pt_value;
           vl_Evt.cle		= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
           vl_Type		= * (XDY_Mot *) vl_Tab_Ligne[i][2].pt_value;
           
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateur
	   */
	  
	   if ( va_FonctionUtilisateur_in == NULL)
	   
	      XZST_03EcritureTrace(XZSTC_WARNING,"va_FonctionUtilisateur_in est nulle");
	   
	   else
	   
	      ( *va_FonctionUtilisateur_in ) ( vl_Evt, vl_Type );
	   	
	   
	}
	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE69_Liste_Causes retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




