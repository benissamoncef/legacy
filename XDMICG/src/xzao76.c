/*E*/
/*Fichier : $Id: xzao76.c,v 1.6 2019/01/07 11:46:50 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2019/01/07 11:46:50 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao76.c
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
* volcic       18 Sep 1994     : Modification Structure RADT (v1.3)
* volcic       29 Nov 1995     : Modif allocation Tab_donees2 (v1.4)
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG   31/01/18        : ajout IP et suppression ";" DEM1306 1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao76.c	1.4 12/13/95 : xzao76.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO76SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO76_Lire_Liste_RADT (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt  	va_NomMachine_in,
				XDY_FonctionInt pa_FonctionUtilisateurRADT_in,
				XDY_FonctionInt pa_FonctionUtilisateurPtMesure_in,
				XDY_Entier	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
* XDY_Fonction  pa_FonctionUtilisateurRADT_in
* XDY_Fonction  pa_FonctionUtilisateurPtMesure_in,
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des RADT de la Machine 
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
* Pour chaque RADT appel de la fonction utilisateur avec XZAOT_RADT
*    Pour chaque pt de mesure appel de la fonction utilisateur avec XZAOT_ConfPtMes
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao76.c	1.4 12/13/95 : XZAO76_Lire_Liste_RADT" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO76_RPC_NAME;
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
	
	XZAOT_RADT	vl_RADT;
   	XZAOT_PtMes	vl_PtMes;
   	XDY_Eqt		vl_Station;
	XDY_Entier	vl_CptPtMes;
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO76_Lire_Liste_RADT : Basedd = %s",
			va_Basedd_in );
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao76: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao76: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao76: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao76: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao76: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao76: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao76 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao76 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao76 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao76: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao76: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao76: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	/*!
	** Manque les points de mesures
	*/
	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurRADT pour
	** chaque type de Picto
	*/
	
	*va_Resultat_out = 0;
	
	vl_CptPtMes = 0;
	      
	vl_RADT.NumeroStation =0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;
	   
	   /*B
           ** Si le numero de la RADT est different du precedent
           ** Remplissage de la structure XZAOT_RADT
           ** Appel de la fonction utilisateur FonctionUtilisateurRADT
           */
           
           vl_Station = * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
            
           if ( vl_RADT.NumeroStation != vl_Station )
           {
              vl_CptPtMes = 0;
	      
	      /*B
              ** Remplissage de la structure XZAOT_RADT coorrespondant a la ligne resultat
              ** du select de la procadure stockee XZAO;76
              */
           
              vl_RADT.NumeroStation	= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
              strcpy(vl_RADT.NomServeur, (char *) vl_Tab_Ligne[i][1].pt_value);
              vl_RADT.Liaison		= * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value;
              strcpy(vl_RADT.RGS, (char *) vl_Tab_Ligne[i][3].pt_value);
              strcpy(vl_RADT.Seuils, (char *) vl_Tab_Ligne[i][4].pt_value);
              strcpy(vl_RADT.Alertes, (char *) vl_Tab_Ligne[i][5].pt_value);
              vl_RADT.DonneesIndividuelles	= * (XDY_Booleen *) vl_Tab_Ligne[i][6].pt_value;
              vl_RADT.NumStationAmontS1	= * (XDY_Eqt *) vl_Tab_Ligne[i][7].pt_value;
              vl_RADT.NumStationAvalS1	= * (XDY_Eqt *) vl_Tab_Ligne[i][8].pt_value;
              vl_RADT.NumStationAmontS2	= * (XDY_Eqt *) vl_Tab_Ligne[i][9].pt_value;
              vl_RADT.NumStationAvalS2	= * (XDY_Eqt *) vl_Tab_Ligne[i][10].pt_value;
              vl_RADT.SiteGestion		= * (XDY_Octet *) vl_Tab_Ligne[i][16].pt_value;
		strcpy(vl_RADT.AdresseIP, (char *) vl_Tab_Ligne[i][18].pt_value);
		vl_RADT.Port = * (XDY_PortIP *) vl_Tab_Ligne[i][19].pt_value;
		vl_RADT.Type  = * (XDY_Octet *) vl_Tab_Ligne[i][17].pt_value;
		strcpy(vl_RADT.IdLCR, (char *) vl_Tab_Ligne[i][20].pt_value);
              /*B
	      ** Appel de la fonction utilisateur FonctionUtilisateurRADT
	      */
	  
	      if ( pa_FonctionUtilisateurRADT_in == NULL)
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurRADT est nulle");
	         asql_clean_cmd(connection, cmd);
	         return (XDC_NOK);
	      }
	      else
	      {
	         if ( ( retcode = (*pa_FonctionUtilisateurRADT_in) ( vl_RADT, *va_Resultat_out ) ) != XDC_OK )
	         {
	            XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurRADT s'est mal deroule");
	            asql_clean_cmd(connection, cmd);
	            return (XDC_NOK);
	         }
	      }
	   }
	   
	   vl_CptPtMes ++;
	         
	   vl_PtMes.NumeroStation	= vl_Station;
	   vl_PtMes.Sens		= * (XDY_Octet *) vl_Tab_Ligne[i][11].pt_value;
	   vl_PtMes.Boucle		= * (XDY_Octet *) vl_Tab_Ligne[i][12].pt_value;
	   vl_PtMes.BoucleInv		= * (XDY_Octet *) vl_Tab_Ligne[i][13].pt_value;
	   vl_PtMes.Voie		= * (XDY_Octet *) vl_Tab_Ligne[i][14].pt_value;
	   vl_PtMes.Poids		= * (XDY_Booleen *) vl_Tab_Ligne[i][15].pt_value;
	   
	   /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurPtMesure
	   */
	  
	   if ( pa_FonctionUtilisateurPtMesure_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurPtMesure est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateurPtMesure_in) ( vl_PtMes, vl_CptPtMes ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurPtMesure s'est mal deroule");
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO76_Lire_Liste_RADT retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




