/*E*/
/*Fichier : $Id: xzao82.c,v 1.6 2020/11/03 17:57:21 pc2dpdy Exp $      Release : $Revision: 1.6 $      Date : $Date: 2020/11/03 17:57:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao82.c
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
* JMG	13/11/07	: ajout site de gestion DEM715
* LCL	11/06/19	: DAI/IP DEM1333 1.5
* CGR	11/09/20	: Ajout deuxieme adresse IP DEM-SAE175 1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao82.c	1.3 12/13/95 : xzao82.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO82SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO82_Lire_Liste_DAI (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt  	va_NomMachine_in,
				XDY_FonctionInt va_FonctionUtilisateurDAI_in,
				XDY_FonctionInt va_FonctionUtilisateurAnalyseur_in,
				XDY_Entier	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
* XDY_Fonction  va_FonctionUtilisateurDAI_in
* XDY_Fonction  va_FonctionUtilisateurAnalyseur_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des DAI de la Machine 
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
* Pour chaque DAI appel de la fonction utilisateur avec XZAOT_DAI
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao82.c	1.3 12/13/95 : XZAO82_Lire_Liste_DAI" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO82_RPC_NAME;
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
	
	XZAOT_DAI	vl_DAI;
	XZAOT_Analyseur	vl_Analyseur;
	XDY_Eqt		vl_NumDAI = 0;
	XDY_Entier	vl_CptAna = 0;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO82_Lire_Liste_DAI : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne %d", retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao82: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao82 : PARAMETER RESULTS");
						fprintf(stdout, "\nPARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao82 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao82 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Traitement des resultats de la procedure stockee XZAO;82
	** et affectation des parametres de sortie
	*/
	
	*va_Resultat_out = 0;
	
	vl_CptAna = 0;
	
	vl_DAI.NumeroDAI =0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           
           (*va_Resultat_out)  ++;
	          
	   /*B
           ** Si le numero de la DAI est different du precedent
           ** Remplissage de la structure XZAOT_DAI
           ** Appel de la fonction utilisateur FonctionUtilisateurDAI
           */
           
           vl_NumDAI = * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
            
           if ( vl_DAI.NumeroDAI != vl_NumDAI )
           {
              vl_CptAna = 0;
	      
	      vl_DAI.NumeroDAI 		= vl_NumDAI;
              strcpy(vl_DAI.NomServeur, (char *) vl_Tab_Ligne[i][1].pt_value);
              strcpy(vl_DAI.Rgs, (char *) vl_Tab_Ligne[i][2].pt_value);
              vl_DAI.LiaisonMaitre	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
              vl_DAI.LiaisonEsclave	= * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
              vl_DAI.SiteGestion	= * (XDY_Octet *) vl_Tab_Ligne[i][8].pt_value;
              strcpy(vl_DAI.AdresseIP, (char *) vl_Tab_Ligne[i][9].pt_value);
              vl_DAI.Port         	= * (XDY_PortIP *) vl_Tab_Ligne[i][10].pt_value;
              strcpy(vl_DAI.AdresseIPE, (char *) vl_Tab_Ligne[i][11].pt_value);
              vl_DAI.PortE         	= * (XDY_PortIP *) vl_Tab_Ligne[i][12].pt_value;
	      strcpy(vl_DAI.Adresse2IP, (char *) vl_Tab_Ligne[i][13].pt_value);
	      vl_DAI.Port2 		= * (XDY_PortIP *) vl_Tab_Ligne[i][14].pt_value;
	      strcpy(vl_DAI.Adresse2IPE, (char *) vl_Tab_Ligne[i][15].pt_value);
	      vl_DAI.Port2E 		= * (XDY_PortIP *) vl_Tab_Ligne[i][16].pt_value;
              
              
	      if ( va_FonctionUtilisateurDAI_in == NULL)
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: va_FonctionUtilisateurDAI est nulle");
	         asql_clean_cmd(connection, cmd);
	   	 return (XDC_NOK);
	      }
	      else
	      {
	         if ( ( retcode = (*va_FonctionUtilisateurDAI_in) ( vl_DAI, *va_Resultat_out ) ) != XDC_OK )
	         {
	            XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: appel de va_FonctionUtilisateurDAI s'est mal deroule");
	            asql_clean_cmd(connection, cmd);
	   	    return (XDC_NOK);
	         }
	      }
	      
	   }   
	   
	   /*B
           ** Remplissage de la structure XZAOT_Analyseur
           ** Appel de la fonction utilisateur FonctionUtilisateurAnalyseur
           */ 
           
           vl_CptAna ++;
	   
	   vl_Analyseur.NumeroEqt	= * (XDY_Eqt *) vl_Tab_Ligne[i][5].pt_value;
           vl_Analyseur.NumeroAnalyseur	= * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
           vl_Analyseur.NombreVoies	= * (XDY_Octet *) vl_Tab_Ligne[i][7].pt_value;
           vl_Analyseur.NumeroDAI 	= vl_NumDAI;
           vl_Analyseur.SiteGestion	= * (XDY_Octet *) vl_Tab_Ligne[i][8].pt_value;
              	
	   if ( va_FonctionUtilisateurAnalyseur_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: va_FonctionUtilisateurAnalyseur est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*va_FonctionUtilisateurAnalyseur_in) ( vl_Analyseur, vl_CptAna ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: appel de va_FonctionUtilisateurAnalyseur s'est mal deroule");
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao82: asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO82_Lire_Liste_DAI retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




