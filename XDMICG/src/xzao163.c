/*E*/
/*Fichier : $Id: xzao163.c,v 1.6 2007/11/19 17:14:36 pc2dpdy Exp $      Release : $Revision: 1.6 $      Date : $Date: 2007/11/19 17:14:36 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao163.c
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
* volcic       29 Nov 1995     : Modif allocation Tab_donees2 (v1.3)
* Mismer       20 Jan 1998     : Ajout type d'equipement (v1.4)
* JMG   13/11/07        : ajout site de gestion DEM715
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* declaration de fonctions internes */

/* definition de fonctions externes */


static char *version = "@(#)xzao163.c	1.5 01/22/98 : xzao163.c" ;

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO163SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO163_Liste_NAV_Machine (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt  	va_NomMachine_in,
				XDY_FonctionInt va_FonctionUtilisateurNAV_in,
				XDY_FonctionInt va_FonctionUtilisateurModule_in,
				XDY_Entier	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
* XDY_Fonction  va_FonctionUtilisateurNAV_in
* XDY_Fonction  va_FonctionUtilisateurModule_in
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
* Pour chaque NAV appel de la fonction utilisateur avec XZAOT_NAV
*    Pour chaque Module de NAV appel de la fonction utilisateur avec XZAOT_Module
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao163.c	1.5 01/22/98 : XZAO163_Liste_NAV_Machine" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO163_RPC_NAME;
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
	
	XZAOT_NAV	vl_NAV;
	XZAOT_Module	vl_Module;
	XDY_Eqt		vl_NumNAV;
	XDY_Eqt		vl_NumeroMod;
	XDY_Eqt		vl_NumeroNAV;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO163_Liste_NAV_Machine : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao163: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao163: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao163: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao163: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao163: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao163: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao163 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao163 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao163 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao163: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao163: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao163: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Traitement des resultats de la procedure stockee XZAO;163
	** et affectation des parametres de sortie
	*/
	
	*va_Resultat_out = 0;
	
	vl_NAV.NumeroNAV = 0;
	
	vl_NumeroNAV	 = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           /*B
           ** Si le numero de la NAV est different du precedent
           ** Remplissage de la structure XZAOT_NAV
           ** Appel de la fonction utilisateur FonctionUtilisateurNAV
           */
           
           *va_Resultat_out  = *va_Resultat_out + 1;
           
           vl_NumNAV = * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
           
           if ( vl_NAV.NumeroNAV != vl_NumNAV )
           {
              vl_NumeroNAV ++;
              vl_NumeroMod = 0;
	      
	      vl_NAV.NumeroNAV 	= vl_NumNAV;
              vl_NAV.NumeroType = * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
              strcpy(vl_NAV.NomServeur, (char *) vl_Tab_Ligne[i][2].pt_value);
              vl_NAV.Liaison	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
              strcpy(vl_NAV.Rgs, (char *) vl_Tab_Ligne[i][4].pt_value);
              vl_NAV.Type = * (XDY_Octet *) vl_Tab_Ligne[i][7].pt_value;
              vl_NAV.SiteGestion = * (XDY_Octet *) vl_Tab_Ligne[i][8].pt_value;
              
              
	      if ( va_FonctionUtilisateurNAV_in == NULL)
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao163 : va_FonctionUtilisateurNAV est nulle");
	         asql_clean_cmd(connection, cmd);
	   	 return (XDC_NOK);
	      }
	      else
	      {
	         if ( ( retcode = (*va_FonctionUtilisateurNAV_in) ( vl_NAV, vl_NumeroNAV ) ) != XDC_OK )
	         {
	            XZST_03EcritureTrace(XZSTC_WARNING,"xzao163 : appel de va_FonctionUtilisateurNAV s'est mal deroule");
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
           vl_Module.NumeroNAV 	= vl_NumNAV;
           strcpy(vl_Module.Code, (char *) vl_Tab_Ligne[i][6].pt_value);
                	
	   if ( va_FonctionUtilisateurModule_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzao163 : va_FonctionUtilisateurModule est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = ( *va_FonctionUtilisateurModule_in ) ( vl_Module, vl_NumeroMod ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao163 : appel de va_FonctionUtilisateurModule s'est mal deroule");
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao163 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO163_Liste_NAV_Machine retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




