/*E*/
/* Fichier : $Id: xzao143.c,v 1.3 2018/11/06 20:07:58 devgfi Exp $        $Revision: 1.3 $        $Date: 2018/11/06 20:07:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao143.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994     : Creation
* volcic	01 Nov 1994     : Modification entete (v1.2)
* JPL		06/11/18 : Gestion du parametre supplementaire 'Supprime' (DEM 1305)  1.3
* GGY		07/09/23 : Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao143.c,v 1.3 2018/11/06 20:07:58 devgfi Exp $ : xzao143" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO143SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO143_Ajouter_PIRAU (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfPIRAU		va_PIRAU_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_NomEqt		va_NomServeur_in
* XDY_Octet			va_LiaisonMaitre_in
* XDY_Octet			va_LiaisonEsclave_in
* XDY_Rgs			va_RGS_in
* XDY_Booleen		va_Supprime_in
* XDY_AdresseIP		va_AdresseIpEsclave_in
* XDY_PortIP		va_PortIpEsclave_in
* XDY_AdresseIP		va_AdresseIpMaitre_in
* XDY_PortIP 		va_PortIpMaitre_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une PIRAU en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* INTERFACE :
*
* Insertion ou modification dans la table EQT_PIRAU (PIRAU)
*
* MECANISMES :
------------------------------------------------------*/

{

	static char *version = "$Id: xzao143.c,v 1.3 2018/11/06 20:07:58 devgfi Exp $ : XZAO143_Ajouter_PIRAU" ;

	CS_CHAR *rpc_name = XZAOC_XZAO143_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO143_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_NomEqt		vl_Nom;
   	XDY_NomEqt		vl_NomServeur;
   	XDY_Octet		vl_LiaisonMaitre;
   	XDY_Octet		vl_LiaisonEsclave;
   	XDY_Rgs			vl_RGS;
   	XDY_Booleen		vl_Supprime;
	XDY_AdresseIP	vl_AdresseIpEsclave;
	XDY_PortIP		vl_PortIpEsclave;
	XDY_AdresseIP	vl_AdresseIpMaitre;
	XDY_PortIP 		vl_PortIpMaitre;
   	
	/*A
	** Initialisation des variables locales
	*/
	
	strcpy( vl_Nom, va_PIRAU_in.Nom );
	strcpy( vl_NomServeur, va_PIRAU_in.NomServeur );
	vl_LiaisonMaitre  = va_PIRAU_in.LiaisonMaitre;
	vl_LiaisonEsclave = va_PIRAU_in.LiaisonEsclave;
	strcpy( vl_RGS, va_PIRAU_in.RGS );
   	vl_Supprime = va_PIRAU_in.Supprime;
	strcpy( vl_AdresseIpEsclave, va_PIRAU_in.AdresseIpEsclave );
	vl_PortIpEsclave = va_PIRAU_in.PortIpEsclave;
	strcpy( vl_AdresseIpMaitre, va_PIRAU_in.AdresseIpMaitre );
	vl_PortIpMaitre = va_PIRAU_in.PortIpEsclave;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO143_Ajouter_PIRAU : Nom = %s\tNomServeur = %s\tLiaisonMaitre = %d\tLiaisonEsclave = %d\tRGS = %s\tAdresseIpEsclave = %s\tPortIpEsclave = %d\tAdresseIpMaitre = %s\tPortIpEsclave = %d\n",
			va_PIRAU_in.Nom,
			va_PIRAU_in.NomServeur,
			va_PIRAU_in.LiaisonMaitre,
			va_PIRAU_in.LiaisonEsclave,
			va_PIRAU_in.RGS,
			va_PIRAU_in.AdresseIpEsclave,
			va_PIRAU_in.PortIpEsclave,
			va_PIRAU_in.AdresseIpMaitre,
			va_PIRAU_in.PortIpEsclave );
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Nom,"@va_Nom_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_char_input(Nom) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomServeur,"@va_NomServeur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_char_input(NomServeur) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_LiaisonMaitre,"@va_LiaisonMaitre_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_tinyint_input(LiaisonMaitre) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_LiaisonEsclave,"@va_LiaisonEsclave_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_tinyint_input(LiaisonEsclave) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_RGS,"@va_RGS_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_char_input(RGS) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_AdresseIpEsclave,"@va_AdresseIpEsclave_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_char_input(AdresseIpEsclave) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PortIpEsclave,"@va_PortIpEsclave_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_int_input(va_PortIpEsclave_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_AdresseIpMaitre,"@va_AdresseIpMaitre_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_char_input(va_AdresseIpMaitre_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PortIpMaitre,"@va_PortIpMaitre_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_int_input(PortIpMaitre) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
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
						retcode = asql_fetch_data (cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao143: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao143 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao143 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao143 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   				    	   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao143 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao143 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao143 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao143 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao143: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO143_Ajouter_PIRAU retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




