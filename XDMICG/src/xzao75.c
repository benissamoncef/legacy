/*E*/
/*Fichier : $Id: xzao75.c,v 1.4 1995/12/13 14:50:26 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/12/13 14:50:26 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao75.c
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
* volcic       27 Dec 1994     : Modification de XZAOT_TypePicto (v1.3)
* volcic       29 Nov 1995     : Modif allocation Tab_donees2 (v1.4)
* GGY    		22/06/23		: Ajout parametre va_TypePnc_in et va_FichierPnc_in (DEM-473)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao75.c,v 1.4 1995/12/13 14:50:26 volcic Exp $ : xzao75.c" ;
	

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO75SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO75_Lire_Types_Picto (	XDY_Basedd	va_Basedd_in,
				XDY_FonctionInt pa_FonctionUtilisateurTypPicto_in,
				XDY_Entier	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Retourne la liste des types de Picto
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* INTERFACE :
*
* Pour chaque type de Picto appel de la fonction utilisateur
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao75.c,v 1.4 1995/12/13 14:50:26 volcic Exp $ : XZAO75_Lire_Types_Picto" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO75_RPC_NAME;
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
	
	XZAOT_TypPicto	vl_TypePicto;
   	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO75_Lire_Types_Picto : Basedd = %s",
			va_Basedd_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao75: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO75_Lire_Types_Picto retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao75: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO75_Lire_Types_Picto retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao75: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO75_Lire_Types_Picto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao75: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO75_Lire_Types_Picto retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao75: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao75 : PARAMETER RESULTS");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao75: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO75_Lire_Types_Picto retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao75: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO75_Lire_Types_Picto retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao75: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO75_Lire_Types_Picto retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurTypPicto pour
	** chaque type de Picto
	*/
	
	*va_Resultat_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;
	   
	   /*B
           ** Remplissage de la structure XZAOT_TypPicto coorrespondant a la ligne resultat
           ** du select de la procadure stockee XZAO;75
           */
           
           vl_TypePicto.NumType		 = * (XDY_Octet *) vl_Tab_Ligne[i][0].pt_value;
           vl_TypePicto.NombreCaracteres = * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
           strcpy(vl_TypePicto.FichierPanneau, (char *) vl_Tab_Ligne[i][2].pt_value);
		   vl_TypePicto.TypePnc = * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
           strcpy(vl_TypePicto.FichierPnc, (char *) vl_Tab_Ligne[i][4].pt_value);

           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurTypPicto
	   */
	  
	   if ( pa_FonctionUtilisateurTypPicto_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurTypPicto est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateurTypPicto_in) ( vl_TypePicto ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurTypPicto s'est mal deroule");
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
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO75_Lire_Types_Picto retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




