/*E*/
/*Fichier : $Id: xzau16.c,v 1.3 2021/05/04 13:28:02 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2021/05/04 13:28:02 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau16.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       	19 Mai 1995     : Creation
* volcic        29 Nov 1995     : Modif allocation Tab_donees2 (v1.2)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzau16.h"

static char *version = "$Id: xzau16.c,v 1.3 2021/05/04 13:28:02 pc2dpdy Exp $ : xzau16.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAU16SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAU16_Liste_Utilisateurs (	XDY_FonctionInt va_FonctionUtilisateur_in )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Fonction  va_FonctionUtilisateurDistrict_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des Districts 
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
* Pour chaque Utilisateur appel de la fonction utilisateur avec pl_Login_MDP
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzau16.c,v 1.3 2021/05/04 13:28:02 pc2dpdy Exp $ : XZAU16_Liste_Utilisateurs" ;
	
	CS_CHAR *rpc_name = XZAUC_XZAU16_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode=0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	char		pl_Login_MDP[18] = "";
	
	/*A
	** Definition des variables locales
	*/
		
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAU16_Liste_Utilisateurs" );
			
			
	/*A
	** Initialisation de la commande				
	*/
XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAU16_Liste_Utilisateurs 0" );

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzau16: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAU16_Liste_Utilisateurs retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAU16_Liste_Utilisateurs 1" );

	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzau16: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAU16_Liste_Utilisateurs retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAU16_Liste_Utilisateurs 2" );
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzau16: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAU16_Liste_Utilisateurs retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzau16: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzau16 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzau16 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzau16 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAU16_Liste_Utilisateurs retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzau16: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAU16_Liste_Utilisateurs retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzau16: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAU16_Liste_Utilisateurs retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzau16: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAU16_Liste_Utilisateurs retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return ( XDC_NOK );
				}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateur pour
	** chaque type de District
	*/
XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAU16_Liste_Utilisateurs 3" );
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAU16_Liste_Utilisateurs 5 Boucle" );
	   /*B
           ** Remplissage de la variable pl_Login_MDP correspondant a la ligne resultat
           ** du select de la procadure stockee XZAU;16
           */
           
           strcpy( pl_Login_MDP, (char *) vl_Tab_Ligne[i][0].pt_value );
           
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateur
	   */
	  
	   if ( va_FonctionUtilisateur_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzau16 : va_FonctionUtilisateur_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*va_FonctionUtilisateur_in) ( pl_Login_MDP, i ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzau16 : appel de va_FonctionUtilisateur_in s'est mal deroule");
	         asql_clean_cmd(connection, cmd);
	      	 return (XDC_NOK);
	      }
	   }	
	   
	}
XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAU16_Liste_Utilisateurs 6" );
	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzau16 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAU16_Liste_Utilisateurs 7" );
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAU16_Liste_Utilisateurs retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




