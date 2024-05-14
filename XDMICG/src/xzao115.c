/*E*/
/*Fichier : @(#)xzao115.c	1.3      Release : 1.3        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao115.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       	11 Oct 1994     : Creation
* volcic	28 Oct 1994	: Modification entete (v1.2)
* volcic        29 Nov 1995     : Modif allocation Tab_donees2 (v1.3)
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.5
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao115.c	1.3 12/13/95 : xzao115.c" ;
	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO115SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO115_Lire_SystemeVideo (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt  	va_NomMachine_in,
				XDY_FonctionInt pa_FonctionUtilisateurLTVideo_in,
				XDY_FonctionInt pa_FonctionUtilisateurCamera_in,
				XDY_FonctionInt pa_FonctionUtilisateurMoniteur_in,
				XDY_FonctionInt pa_FonctionUtilisateurMagnetoscope_in,
				XZAOT_SysVideo 	*va_SystVideo_out)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_Basedd_in		
* XDY_Machine		va_NomMachine_in		
* XDY_Fonction  	pa_FonctionUtilisateurLTVideo_in
* XDY_Fonction  	pa_FonctionUtilisateurCamera_in
* XDY_Fonction  	pa_FonctionUtilisateurMoniteur_in
* XDY_Fonction  	pa_FonctionUtilisateurMagnetoscope_in
*				
* PARAMETRES EN SORTIE :
*
* XZAOT_SystVideo 	va_SysVideo_out
*
* VALEUR RENDUE :
*
* Retourne la liste des SystemeVideo de la Machine 
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
* Pour chaque LTVideo appel de la fonction utilisateur avec XZAOT_LTVideo
* Pour chaque Camera appel de la fonction utilisateur avec XZAOT_Camera
* Pour chaque Moniteur appel de la fonction utilisateur avec XZAOT_Moniteur
* Pour chaque Magnetoscope appel de la fonction utilisateur avec XZAOT_Magnetoscope
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao115.c	1.3 12/13/95 : XZAO115_Lire_SystemeVideo" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO115_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO115_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAOC_XZAO115_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Mot		vl_Video = 0;
	char	vl_Serveur[11]="";
   	XDY_Octet	vl_Liaison = 0;
   	char		vl_Rgs[4]="";
   	
   	XDY_Octet	vl_Type;
   	
   	XDY_Entier	vl_NbEqtLTV = 0;
   	XDY_Entier	vl_NbEqtCam = 0;
   	XDY_Entier	vl_NbEqtMon = 0;
   	XDY_Entier	vl_NbEqtMag = 0;
   	
   	
   	XZAOT_LTVideo	vl_LTVideo;
	XZAOT_Camera	vl_Camera;
	XZAOT_Moniteur	vl_Moniteur;
	XZAOT_Magneto	vl_Magneto;
	
	
	strcpy(va_SystVideo_out->NomServeur, " ");
        va_SystVideo_out->Numero 	= 0;
        va_SystVideo_out->Liaison	= 0;
        strcpy(va_SystVideo_out->RGS, " ");
        
        
        XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO115_Lire_SystemeVideo : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao147: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO147_Liste_ITPC retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)vl_Serveur;

	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)vl_Serveur,"@va_Serveur_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: asql_ctparam_char_output(Serveur) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
        
	
	Tab_donnees2[1].pt_value = (CS_VOID *)&vl_Video;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&vl_Video,"@va_SysVideo_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: asql_ctparam_smallint_output(SysVideo) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

		
	Tab_donnees2[2].pt_value = (CS_VOID *)&vl_Liaison;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_Liaison,"@va_Liaison_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: asql_ctparam_tinyint_output(Liaison) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	
	Tab_donnees2[3].pt_value = (CS_VOID *)vl_Rgs;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)vl_Rgs,"@va_Rgs_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: asql_ctparam_char_output(Rgs) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao115: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao115 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao115 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao115 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeRAU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao115: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	
	
	/*A
	** Appel des fonctions utilisateurs 
	*/
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           vl_Type = * (XDY_Octet *) vl_Tab_Ligne[i][0].pt_value;
           
           /*B
           ** Selon le type de l'equipement, remplissage de la structure correspondante
           ** Appel de la fonction utilisateur correspondante
           */
           
           switch ( vl_Type )
           {
              case XDC_EQT_LTV :	vl_NbEqtLTV ++;
              
           				vl_LTVideo.Numero 		= * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
              				vl_LTVideo.LiaisonsFixes	= * (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value;
           				vl_LTVideo.LiaisonsCycles	= * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
           
           				if ( pa_FonctionUtilisateurLTVideo_in == NULL)
	   				{
	      				   XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurLTVideo_in est nulle");
	   				   asql_clean_cmd(connection, cmd);
	   				   return (XDC_NOK);
	   				}
	   				else
	   				{
	      				   if ( ( retcode = (*pa_FonctionUtilisateurLTVideo_in ) ( vl_LTVideo, vl_NbEqtLTV ) ) != XDC_OK )
	      				   {
	         			      XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurLTVideo_in s'est mal deroule");
	      				      asql_clean_cmd(connection, cmd);
	   				      return (XDC_NOK);
	      				   }
	   				}	
	   
	   				break;
	   				
	   				
	      case XDC_EQT_CAM :	vl_NbEqtCam ++;
              
           				vl_Camera.Numero 	= * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
              				vl_Camera.NumeroLTVideo = * (XDY_Eqt *) vl_Tab_Ligne[i][1].pt_value;
              				strcpy ( vl_Camera.Code, (char *) vl_Tab_Ligne[i][3].pt_value );
	     				vl_Camera.Cycle 	= * (XDY_Booleen *) vl_Tab_Ligne[i][7].pt_value;
              				vl_Camera.Mobile 	= * (XDY_Booleen *) vl_Tab_Ligne[i][8].pt_value;
              				vl_Camera.Positionnable = * (XDY_Booleen *) vl_Tab_Ligne[i][9].pt_value;
              				
           				if ( pa_FonctionUtilisateurCamera_in == NULL)
	   				{
	      				   XZST_03EcritureTrace(XZSTC_WARNING,"xzao115 : pa_FonctionUtilisateurCamera_in est nulle");
	   				   asql_clean_cmd(connection, cmd);
	   				   return (XDC_NOK);
	   				}
	   				else
	   				{
	      				   if ( ( retcode = (*pa_FonctionUtilisateurCamera_in ) ( vl_Camera, vl_NbEqtCam ) ) != XDC_OK )
	      				   {
	         			      XZST_03EcritureTrace(XZSTC_WARNING,"xzao115 : appel de pa_FonctionUtilisateurCamera_in s'est mal deroule");
	      				      asql_clean_cmd(connection, cmd);
	   				      return (XDC_NOK);
	      				   }
	   				}	
	   	
	   				break;
           
           
              case XDC_EQT_MNT :	vl_NbEqtMon ++;
              
           				vl_Moniteur.Numero = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
              				strcpy ( vl_Moniteur.Code, (char *) vl_Tab_Ligne[i][3].pt_value );
              				strcpy ( vl_Moniteur.MachineOperateur, (char *) vl_Tab_Ligne[i][4].pt_value );
              				
              				if ( pa_FonctionUtilisateurMoniteur_in == NULL)
	   				{
	      				   XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurMoniteur_in est nulle");
	   				   asql_clean_cmd(connection, cmd);
	   				   return (XDC_NOK);
	   				}
	   				else
	   				{
	      				   if ( ( retcode = (*pa_FonctionUtilisateurMoniteur_in ) ( vl_Moniteur, vl_NbEqtMon ) ) != XDC_OK )
	      				   {
	         			      XZST_03EcritureTrace(XZSTC_WARNING,"xzao115 : appel de pa_FonctionUtilisateurMoniteur_in s'est mal deroule");
	      				      asql_clean_cmd(connection, cmd);
	   				      return (XDC_NOK);
	      				   }
	   				}	
	   	
	   				break;
           
	      case XDC_EQT_MAG :	vl_NbEqtMag ++;
	      
	      				vl_Magneto.Numero = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
              				strcpy ( vl_Magneto.Code, (char *) vl_Tab_Ligne[i][3].pt_value );
              				
              				if ( pa_FonctionUtilisateurMagnetoscope_in == NULL)
	   				{
	      				   XZST_03EcritureTrace(XZSTC_WARNING,"xzao115 : pa_FonctionUtilisateurMagnetoscope_in est nulle");
	   				   asql_clean_cmd(connection, cmd);
	   				   return (XDC_NOK);
	   				}
	   				else
	   				{
	      				   if ( ( retcode = (*pa_FonctionUtilisateurMagnetoscope_in ) ( vl_Magneto, vl_NbEqtMag ) ) != XDC_OK )
	      				   {
	         			      XZST_03EcritureTrace(XZSTC_WARNING,"xzao115 : appel de pa_FonctionUtilisateurMagnetoscope_in s'est mal deroule");
	      				      asql_clean_cmd(connection, cmd);
	   				      return (XDC_NOK);
	      				   }
	   				}	
	   	
	   				break;

		default	:	strcpy(va_SystVideo_out->NomServeur,(char *) vl_Tab_Ligne[i][4].pt_value);
				strcpy(va_SystVideo_out->RGS, (char *) vl_Tab_Ligne[i][3].pt_value);
				va_SystVideo_out->Numero = * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
				va_SystVideo_out->Liaison = * (XDY_Eqt *) vl_Tab_Ligne[i][1].pt_value;
				break;
           }
              
	}
	
	
	/*A
        ** Remplissage de la structure XZAOT_SysVideo correspondant aux parametres resultat
        ** de la procedure stockee XZAO;115
        */
           
        /*strcpy(va_SystVideo_out->NomServeur, vl_Serveur);
        va_SystVideo_out->Numero 	= vl_Video;
        va_SystVideo_out->Liaison	= vl_Liaison;
        strcpy(va_SystVideo_out->RGS, vl_Rgs);*/
        
        
        /*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao115 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO115_Lire_SystemeVideo retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




