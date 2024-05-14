/*E*/
/*Fichier : $Id: xzao117.c,v 1.9 2019/05/21 11:06:46 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2019/05/21 11:06:46 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao117.c
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
* JMG		07/05/19 : GAPLA DEM1334 1.3
* GGY		07/09/23 : Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao117.c,v 1.9 2019/05/21 11:06:46 pc2dpdy Exp $ : xzao117" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO117SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO117_Lire_SystemeRAU (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt	va_NomMachine_in,
				XDY_FonctionInt va_FonctionUtilisateurPAU_in,
				XZAOT_PIRAU	*va_PIRAU_out,
				XDY_Entier	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des Systemes RAU de la Machine 
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
* Pour chaque PAU appel de la fonction utilisateur
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao117.c,v 1.9 2019/05/21 11:06:46 pc2dpdy Exp $ : XZAO117_Lire_SystemeRAU" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO117_RPC_NAME;
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
	int		i,ret;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XZAOT_PAU	vl_PAU;
   	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO117_Lire_SystemeRAU : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao117: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao117: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao117: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao117: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao117: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao117: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao117 : PARAMETER RESULTS");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao117: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					XZST_03EcritureTrace(XZSTC_INFO,"xzao117 : ct_result retourne CS_MSG_RESULT l id du message est : %d.", msg_id);
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
					XZST_03EcritureTrace(XZSTC_INFO,"xzao117 : CS_CMD_DONE");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao117: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao117: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurTypPMV pour
	** chaque type de PMV
	*/
	
	*va_Resultat_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;
	
           /*B
           ** Remplissage de la structure XZAOT_TypPMV coorrespondant a la ligne resultat
           ** du select de la procadure stockee XZAO;117
           */
           
           va_PIRAU_out->Numero 	= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
           strcpy ( va_PIRAU_out->ServeurTerminaux, (char *) vl_Tab_Ligne[i][1].pt_value );
           va_PIRAU_out->LiaisonMaitre 	= * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value;
           va_PIRAU_out->LiaisonEsclave	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
           strcpy ( va_PIRAU_out->Rgs, (char *) vl_Tab_Ligne[i][4].pt_value );
           
           strcpy ( va_PIRAU_out->AdresseIpEsclave, (char *) vl_Tab_Ligne[i][5].pt_value );
           va_PIRAU_out->PortIpEsclave	= * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
           strcpy ( va_PIRAU_out->AdresseIpMaitre, (char *) vl_Tab_Ligne[i][7].pt_value );
           va_PIRAU_out->PortIpMaitre	= * (XDY_Octet *) vl_Tab_Ligne[i][8].pt_value;

           vl_PAU.Numero	= * (XDY_Eqt *) vl_Tab_Ligne[i][9].pt_value;
           strcpy ( vl_PAU.CodePAU, * (XDY_CodeEqt *) vl_Tab_Ligne[i][10].pt_value );
           strcpy ( vl_PAU.CodeAntenne, * (XDY_CodeEqt *) vl_Tab_Ligne[i][11].pt_value );
           vl_PAU.Type	= * (XDY_Octet *) vl_Tab_Ligne[i][12].pt_value;
           
           
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurPAU
	   */
	  
	   if ( va_FonctionUtilisateurPAU_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzao117 : va_FonctionUtilisateurPAU_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode =  (*va_FonctionUtilisateurPAU_in) ( vl_PAU, *va_Resultat_out ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzao117 : appel de va_FonctionUtilisateurPAU s'est mal deroule");
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao117 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}






