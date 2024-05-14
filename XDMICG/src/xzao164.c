/*E*/
/*Fichier : $Id: xzao164.c,v 1.4 1995/12/13 14:49:51 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/12/13 14:49:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao164.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       11 Oct 1994     : Creation
* volcic       01 Nov 1994     : Modification entete (v1.2)
* volcic       22 Nov 1994     : Ajout asql_clean et asql_cmd_alloc (v1.3)
* volcic       29 Nov 1995     : Modif allocation Tab_donees2 (v1.4)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao164.c,v 1.4 1995/12/13 14:49:51 volcic Exp $ : xzao164.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO164SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO164_Liste_Types_NAV (	XDY_Basedd	va_Basedd_in,
				XDY_FonctionInt va_FonctionUtilisateurNAV_in,
				XDY_FonctionInt va_FonctionUtilisateurScenario_in,
				XDY_Entier	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Fonction  va_FonctionUtilisateurNAV_in
* XDY_Fonction  va_FonctionUtilisateurScenario_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des NAV de la Machine 
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
* Pour chaque NAV appel de la fonction utilisateur avec XZAOT_TypeNAV
*    Pour chaque Scenario de NAV appel de la fonction utilisateur avec XZAOT_ScenConnu
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao164.c,v 1.4 1995/12/13 14:49:51 volcic Exp $ : XZAO164_Liste_Types_NAV" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO164_RPC_NAME;
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
	
	XZAOT_TypeNAV	vl_NAV;
	XZAOT_ScenConnu	vl_Scenario;
	XDY_Eqt		vl_NumNAV;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO164_Liste_Types_NAV : Basedd = %s",
			va_Basedd_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao164: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO164_Liste_Types_NAV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao164: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO164_Liste_Types_NAV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao164: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO164_Liste_Types_NAV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao164: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO164_Liste_Types_NAV retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao164: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao164 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao164 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao164 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO164_Liste_Types_NAV retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao164: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO164_Liste_Types_NAV retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao164: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO164_Liste_Types_NAV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao164: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO164_Liste_Types_NAV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Traitement des resultats de la procedure stockee XZAO;164
	** et affectation des parametres de sortie
	*/
	
	*va_Resultat_out  = 0;
	
	vl_NAV.NumeroType = 0;
	
	   for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	   {
              /*B
              ** Si le numero de la NAV est different du precedent
              ** Remplissage de la structure XZAOT_NAV
              ** Appel de la fonction utilisateur FonctionUtilisateurNAV
              */
           
              vl_NumNAV = * (XDY_Octet *) vl_Tab_Ligne[i][0].pt_value;
           
              if ( vl_NAV.NumeroType != vl_NumNAV )
              {
                 (*va_Resultat_out) ++;
	      
	         vl_NAV.NumeroType 	= vl_NumNAV;
                 vl_NAV.FlashMod4	= * (XDY_Booleen *) vl_Tab_Ligne[i][1].pt_value;
                 vl_NAV.NbBarrieres	= * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value;
                 
              
	         if ( va_FonctionUtilisateurNAV_in == NULL)
	         {
	            XZST_03EcritureTrace(XZSTC_WARNING,"xzao164 : va_FonctionUtilisateurNAV est nulle");
	            asql_clean_cmd(connection, cmd);
		    return (XDC_NOK);
	         }
	         else
	         {
	            if ( ( retcode = (*va_FonctionUtilisateurNAV_in) ( vl_NAV, *va_Resultat_out ) ) != XDC_OK )
	            {
	               XZST_03EcritureTrace(XZSTC_WARNING,"xzao164 : appel de va_FonctionUtilisateurNAV s'est mal deroule");
	               asql_clean_cmd(connection, cmd);
		       return (XDC_NOK);
	            }
	         }
	      
	      }   
	   
	      /*B
              ** Remplissage de la structure XZAOT_Scenario
              ** Appel de la fonction utilisateur FonctionUtilisateurScenario
              */ 
           
              vl_Scenario.NumeroScen	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
              vl_Scenario.AutoFichier	= * (XDY_Booleen *) vl_Tab_Ligne[i][4].pt_value;
              strcpy ( vl_Scenario.Type, (char *) vl_Tab_Ligne[i][5].pt_value ); 
               	
	      if ( va_FonctionUtilisateurScenario_in == NULL)
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao164 : va_FonctionUtilisateurScenario est nulle");
	         asql_clean_cmd(connection, cmd);
		 return (XDC_NOK);
	      }
	      else
	      {
	         if ( ( retcode = (*va_FonctionUtilisateurScenario_in) ( vl_Scenario, *va_Resultat_out ) ) != XDC_OK )
	         {
	            XZST_03EcritureTrace(XZSTC_WARNING,"xzao164 : appel de va_FonctionUtilisateurScenario s'est mal deroule");
	            asql_clean_cmd(connection, cmd);
		    return (XDC_NOK);
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao164 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO164_Liste_Types_NAV retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




