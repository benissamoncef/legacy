/*E*/
/*Fichier : $Id: xzao77.c,v 1.6 1995/12/13 14:50:31 volcic Exp $      Release : $Revision: 1.6 $        Date : $Date: 1995/12/13 14:50:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao77.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       11 Oct 1994     : Creation
* volcic       27 Dec 1994     : Modification des casts de NumeroAlerte et NumeroAlarme (v1.3)
* volcic       10 Avr 1995     : Ajout de la constante XZAOC_NB_ETOR (v1.5)
* volcic       08 Aou 1995     : Suppression XZAOC_NB_ETOR et mise dans xzao77.h (v1.6)
* volcic       29 Nov 1995     : Modif allocation Tab_donees2 (v1.7)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao77.c,v 1.6 1995/12/13 14:50:31 volcic Exp $ : xzao77" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO77SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO77_Lire_Liste_Mini_GTC (	XDY_Basedd	va_Basedd_in,
					XDY_NomEqt  	va_NomMachine_in,
					XDY_FonctionInt va_FonctionUtilisateurMiniGTC_in,
					XDY_Entier	*va_Resultat_out )
/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
* XDY_Fonction  va_FonctionUtilisateurMiniGTC_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des MiniGTC de la Machine 
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
* Pour chaque MiniGTC appel de la fonction utilisateur avec XZAOT_MiniGTC
*    Pour chaque pt de mesure appel de la fonction utilisateur avec XZAOT_ConfPtMes
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao77.c,v 1.6 1995/12/13 14:50:31 volcic Exp $ : XZAO77_Lire_Liste_Mini_GTC" ;

	CS_CHAR *rpc_name = XZAOC_XZAO77_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type = 0;
	CS_SMALLINT     msg_id = 0;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i = 0;
	int		j = 0;
	XDY_Entier	SPstatus = 0;
	XDY_Entier	vl_NbGTC = 0;
	
	/*A
	** Definition des variables locales
	*/
	
	XZAOT_MiniGTC	vl_MiniGTC;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO77_Lire_Liste_Mini_GTC : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao77: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO77_Lire_Liste_Mini_GTC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao77: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO77_Lire_Liste_Mini_GTC retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao77: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO77_Lire_Liste_Mini_GTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao77: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO77_Lire_Liste_Mini_GTC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao77: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO77_Lire_Liste_Mini_GTC retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao77: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao77 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao117 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao117 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao77: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO77_Lire_Liste_Mini_GTC retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao77: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO77_Lire_Liste_Mini_GTC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao77: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO77_Lire_Liste_Mini_GTC retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurMiniGTC pour
	** chaque type de Picto
	*/
	
	*va_Resultat_out = 0;
	j = 0;
	vl_NbGTC = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;
	   
	   /*B
           ** Remplissage de la structure XZAOT_MiniGTC coorrespondant a la ligne resultat
           ** du select de la procadure stockee XZAO;77
           */
           
           vl_MiniGTC.NumeroMiniGTC 	= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
           strcpy(vl_MiniGTC.NomServeur, (char *) vl_Tab_Ligne[i][1].pt_value);
           vl_MiniGTC.Liaison		= * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value;
           strcpy(vl_MiniGTC.Rgs, (char *) vl_Tab_Ligne[i][3].pt_value);
           
           vl_MiniGTC.ListeETOR[j].NumeroAlarme	= * (XDY_Mot *) vl_Tab_Ligne[i][5].pt_value;
           vl_MiniGTC.ListeETOR[j].NumeroAlerte	= * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
           vl_MiniGTC.ListeETOR[j].NumeroEqt	= * (XDY_Eqt *) vl_Tab_Ligne[i][7].pt_value;
           strcpy(vl_MiniGTC.ListeETOR[j].NomETOR, (char *) vl_Tab_Ligne[i][8].pt_value);
           
           j ++;
	  
	   /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurMiniGTC
	   ** si j = XZAOC_NB_ETOR <=> Liste des ETOR complete pour une Mini GTC
	   */
	  
	   if ( j == XZAOC_NB_ETOR )
	   {
	      vl_NbGTC ++;
	      
	      if ( va_FonctionUtilisateurMiniGTC_in == NULL)
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"va_FonctionUtilisateurMiniGTC est nulle");
	         asql_clean_cmd(connection, cmd);
		 return (XDC_NOK);
	      }
	      else
	      {
	         if ( ( retcode = (*va_FonctionUtilisateurMiniGTC_in) ( vl_MiniGTC, vl_NbGTC ) ) != XDC_OK )
	         {
	            XZST_03EcritureTrace(XZSTC_WARNING,"appel de va_FonctionUtilisateurMiniGTC s'est mal deroule");
	            asql_clean_cmd(connection, cmd);
		    return (XDC_NOK);
	         }
	      j = 0;
	      
	      }	
	   }
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
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO77_Lire_Liste_Mini_GTC retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




