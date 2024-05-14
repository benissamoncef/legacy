/*E*/
/*Fichier : $Id: xzao125.c,v 1.3 1995/12/13 14:49:43 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/12/13 14:49:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao125.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic        11 Oct 1994     : Creation
* volcic	01 Nov 1994	: Modification entete (v1.2)
* volcic        29 Nov 1995     : Modif allocation Tab_donees2 (v1.3)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao125.c,v 1.3 1995/12/13 14:49:43 volcic Exp $ : xzao125.c" ;
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO125SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO125_Liste_Eqt_Synoptique (	XDY_Basedd	va_Basedd_in,
					XDY_Nom		va_NomTroncon_in,
					XDY_Mot		va_NumeroPortion_in,
					XDY_Octet	va_TypeEqt_in,
				  	XDY_FonctionInt va_FonctionUtilisateurEqtSyn_in,
					XDY_Entier 	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Nom	va_NomTroncon_in
* XDY_Mot	va_NumeroPortion_in
* XDY_Octet	va_TypeEqt_in
* XDY_Fonction  va_FonctionUtilisateurEqt_Synoptique_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
*  Liste des Eqt d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
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
* Pour chaque Eqt_Synoptique appel de la fonction utilisateur avec XZAOT_Eqt_Synoptique
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzao125.c,v 1.3 1995/12/13 14:49:43 volcic Exp $ : Liste_Eqt_Synoptique" ;

	CS_CHAR *rpc_name = XZAOC_XZAO125_RPC_NAME;
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
	char	vl_troncon[100]="";
	
	/*A
	** Definition des variables locales
	*/
	
	XZAOT_EqtSyn	vl_EqtSyn;
	
	vl_troncon[0]='\0';
	if (va_NomTroncon_in!=NULL)
		strcpy(vl_troncon,va_NomTroncon_in);

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO125_Liste_Eqt_Synoptique : Basedd = %s\tTroncon = %s\tPortion = %d\tTypeEqt = %d",
			va_Basedd_in,
			va_NomTroncon_in,
			va_NumeroPortion_in,
			va_TypeEqt_in );
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_troncon,"@va_NomTroncon_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: asql_ctparam_char_input(NomTroncon) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumeroPortion_in,"@va_NumeroPortion_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: asql_ctparam_smallint_input(NumeroPortion) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_TypeEqt_in,"@va_TypeEqt_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: asql_ctparam_tinyint_input(TypeEqt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
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
						retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao125: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao125 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao125 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao125 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao125: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurEqt_Synoptique pour
	** chaque type de Eqt_Synoptique
	** Affectation des parametres de sortie
	*/
	
	va_Resultat_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           va_Resultat_out ++;
	   
	   /*B
           ** Remplissage de la structure XZAOT_EqtSyn coorrespondant a la ligne resultat
           ** du select de la procedure stockee XZAO;125
           */


           vl_EqtSyn.NumeroEqt		= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
           strcpy(vl_EqtSyn.NomEqt, (char *) vl_Tab_Ligne[i][1].pt_value);
           vl_EqtSyn.NumeroAutoroute	= * (XDY_Autoroute *) vl_Tab_Ligne[i][2].pt_value;
           vl_EqtSyn.PR			= * (XDY_PR *) vl_Tab_Ligne[i][3].pt_value;
           vl_EqtSyn.Sens		= * (XDY_Sens *) vl_Tab_Ligne[i][4].pt_value;

            	XZST_03EcritureTrace( XZSTC_WARNING,"XZAO125_Liste_Eqt_Synoptique : vl_EqtSyn.NumeroEqt = %d\tvl_EqtSyn.NomEqt = %s\tvl_EqtSyn.NumeroAutoroute = %d\tvl_EqtSyn.PR = %d\tvl_EqtSyn.Sens = %d",
			vl_EqtSyn.NumeroEqt,
			vl_EqtSyn.NomEqt,
			vl_EqtSyn.NumeroAutoroute,
			vl_EqtSyn.PR,
			vl_EqtSyn.Sens );          
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurEqtSyn
	   */
	  
	   if ( va_FonctionUtilisateurEqtSyn_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"va_FonctionUtilisateurEqtSyn_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*va_FonctionUtilisateurEqtSyn_in) ( vl_EqtSyn ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de va_FonctionUtilisateurEqtSyn s'est mal deroule");
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
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO125_Liste_Eqt_Synoptique retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




