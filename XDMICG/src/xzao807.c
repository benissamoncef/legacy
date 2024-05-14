/*E*/
/*Fichier : $Id: xzao807.c,v 1.1 2018/04/18 18:00:13 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/04/18 18:00:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao807.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   04/03/18        : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* declaration de fonctions internes */

/* definition de fonctions externes */


static char *version = "$Id: xzao807.c,v 1.1 2018/04/18 18:00:13 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2018/04/18 18:00:13 $ : xzao807.c" ;

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO807SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO807_Liste_CFE_Machine (	XDY_NomEqt  	va_NomMachine_in,
				XDY_FonctionInt va_FonctionUtilisateurCFE_in,
				XDY_Entier	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Machine	va_NomMachine_in		
* XDY_Fonction  va_FonctionUtilisateurCFE_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des CFE de la Machine 
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
* Pour chaque CFE appel de la fonction utilisateur avec XZAOT_CFE
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao807.c	$Id: xzao807.c,v 1.1 2018/04/18 18:00:13 pc2dpdy Exp $ %D% : XZAO807_Liste_CFE_Machine" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO807_RPC_NAME;
	CS_CONTEXT      *context;
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
	
	XZAOT_CFE	vl_CFE;
	XDY_Eqt		vl_NumCFE;
	XDY_Eqt		vl_NumeroCFE;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO807_Liste_CFE_Machine : Machine = %s", va_NomMachine_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao807: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO807_Liste_CFE_Machine retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao807: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO807_Liste_CFE_Machine retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao807: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO807_Liste_CFE_Machine retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao807: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO807_Liste_CFE_Machine retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao807: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao807 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao807 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao807 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO807_Liste_CFE_Machine retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao807: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO807_Liste_CFE_Machine retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao807: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO807_Liste_CFE_Machine retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao807: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO807_Liste_CFE_Machine retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Traitement des resultats de la procedure stockee XZAO;807
	** et affectation des parametres de sortie
	*/
	
	*va_Resultat_out = 0;
	
	vl_CFE.NumeroCFE = 0;
	
	vl_NumeroCFE	 = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           /*B
           ** Si le numero de la CFE est different du precedent
           ** Remplissage de la structure XZAOT_CFE
           ** Appel de la fonction utilisateur FonctionUtilisateurCFE
           */
           
           *va_Resultat_out  = *va_Resultat_out + 1;
           
           vl_NumCFE = * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
           
           if ( vl_CFE.NumeroCFE != vl_NumCFE )
           {
              vl_NumeroCFE ++;
	      
	      vl_CFE.NumeroCFE 	= vl_NumCFE;
              strcpy(vl_CFE.NomServeur, (char *) vl_Tab_Ligne[i][1].pt_value);
              strcpy(vl_CFE.AdresseIP, (char *) vl_Tab_Ligne[i][2].pt_value);
              vl_CFE.Port	= * (XDY_Entier *) vl_Tab_Ligne[i][3].pt_value;
              vl_CFE.SiteGestion = * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
              
              
	      if ( va_FonctionUtilisateurCFE_in == NULL)
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao807 : va_FonctionUtilisateurCFE est nulle");
	         asql_clean_cmd(connection, cmd);
	   	 return (XDC_NOK);
	      }
	      else
	      {
	         if ( ( retcode = (*va_FonctionUtilisateurCFE_in) ( vl_CFE, vl_NumeroCFE ) ) != XDC_OK )
	         {
	            XZST_03EcritureTrace(XZSTC_WARNING,"xzao807 : appel de va_FonctionUtilisateurCFE s'est mal deroule");
	            asql_clean_cmd(connection, cmd);
	   	    return (XDC_NOK);
	         }
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao807 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO807_Liste_CFE_Machine retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




