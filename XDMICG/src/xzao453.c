/*E*/
/*  Fichier : $Id: xzao453.c,v 1.2 2007/11/19 17:14:36 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2007/11/19 17:14:36 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzat453.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* JMG   13/11/07        : ajout site de gestion DEM715

------------------------------------------------------ */

/* fichiers inclus */

#include "xzao.h"
#include "xzao453.h"

/* declaration de fonctions internes */

/* definition de fonctions externes */


static char *version = "$Id: xzao453.c,v 1.2 2007/11/19 17:14:36 pc2dpdy Exp $ $Revision: 1.2 $ $Date: 2007/11/19 17:14:36 $ : xzao453.c" ;

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO453SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO453_Liste_BAD_Machine (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt  	va_NomMachine_in,
				XDY_FonctionInt va_FonctionUtilisateurBAD_in,
				XDY_FonctionInt va_FonctionUtilisateurModule_in,
				XDY_Entier	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
* XDY_Fonction  va_FonctionUtilisateurBAD_in
* XDY_Fonction  va_FonctionUtilisateurModule_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des BAD de la Machine 
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
* Pour chaque BAD appel de la fonction utilisateur avec XZAOT_BAD
*    Pour chaque Module de BAD appel de la fonction utilisateur avec XZAOT_Module
*
* MECANISMES :
*
------------------------------------------------------*/

{

	
	CS_CHAR *rpc_name = XZAOC_XZAO453_RPC_NAME;
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
	
	XZAOT_BAD	vl_BAD;
	XZAOT_ModuleBAD	vl_Module;
	XDY_Eqt		vl_NumBAD;
	XDY_Eqt		vl_NumeroMod;
	XDY_Eqt		vl_NumeroBAD;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO453_Liste_BAD_Machine : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao453: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao453: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao453: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao453: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao453: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao453: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao453 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao453 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao453 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao453: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao453: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao453: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Traitement des resultats de la procedure stockee XZAO;453
	** et affectation des parametres de sortie
	*/
	
	*va_Resultat_out = 0;
	
	vl_BAD.NumeroBAD = 0;
	
	vl_NumeroBAD	 = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           /*B
           ** Si le numero de la BAD est different du precedent
           ** Remplissage de la structure XZAOT_BAD
           ** Appel de la fonction utilisateur FonctionUtilisateurBAD
           */
           
           *va_Resultat_out  = *va_Resultat_out + 1;
           
           vl_NumBAD = * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
           
           if ( vl_BAD.NumeroBAD != vl_NumBAD )
           {
              vl_NumeroBAD ++;
              vl_NumeroMod = 0;
	      
	      vl_BAD.NumeroBAD 	= vl_NumBAD;
              vl_BAD.Type = * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
              strcpy(vl_BAD.NomServeur, (char *) vl_Tab_Ligne[i][2].pt_value);
              vl_BAD.Liaison	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
              strcpy(vl_BAD.Rgs, (char *) vl_Tab_Ligne[i][4].pt_value);
              vl_BAD.SiteGestion	= * (XDY_Octet *) vl_Tab_Ligne[i][7].pt_value;
              
              
	      if ( va_FonctionUtilisateurBAD_in == NULL)
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao453 : va_FonctionUtilisateurBAD est nulle");
	         asql_clean_cmd(connection, cmd);
	   	 return (XDC_NOK);
	      }
	      else
	      {
	         if ( ( retcode = (*va_FonctionUtilisateurBAD_in) ( vl_BAD, vl_NumeroBAD ) ) != XDC_OK )
	         {
	            XZST_03EcritureTrace(XZSTC_WARNING,"xzao453 : appel de va_FonctionUtilisateurBAD s'est mal deroule");
	            asql_clean_cmd(connection, cmd);
	   	    return (XDC_NOK);
	         }
	      }
	   }      
	   
	   /*B
           ** Remplissage de la structure XZAOT_Module
           ** Appel de la fonction utilisateur FonctionUtilisateurModule
           */ 
           
           vl_NumeroMod ++;
           
           vl_Module.NumeroMod	= * (XDY_Eqt *) vl_Tab_Ligne[i][5].pt_value;
           vl_Module.NumeroBAD 	= vl_NumBAD;
           strcpy(vl_Module.Code, (char *) vl_Tab_Ligne[i][6].pt_value);
                	
	   if ( va_FonctionUtilisateurModule_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzao453 : va_FonctionUtilisateurModule est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = ( *va_FonctionUtilisateurModule_in ) ( vl_Module, vl_NumeroMod ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao453 : appel de va_FonctionUtilisateurModule s'est mal deroule");
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao453 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO453_Liste_BAD_Machine retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




