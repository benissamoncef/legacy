/*E*/
/*Fichier : @(#)xzao203.c	1.10      Release : 1.10        Date : 01/22/98
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao203.c
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
* volcic       10 Nov 1994     : Modification Structures Sequences (v1.3)
* volcic       18 Nov 1994     : Ajout Elements dans la retour des select scenarios (v1.4)
* volcic       22 Nov 1994     : Ajout asql_cmd_alloc et asql_clean (v1.5)
* volcic       29 Jan 1995     : Modif recup liste tube (v1.6)
* volcic       29 Nov 1995     : Modif allocation Tab_donees2 (v1.7)
* mismer       24 Jan 1996     : Modif argument de sortie pour select longueur seq echangeur (v1.8)
* mismer       24 Avr 1997     : Ajout colonne alerte + type gabarit,vitesse et niche (v1.9) (DEM/1421)
* mismer       06 Nov 1997     : Modif pour hp-ux 10.20 (v1.10) (DEM/xxxx)
* PFE   17/11/06        DEM 591 : on supprime les free si pas de alloc pb en version HPUX 11.11
* JMG	20/04/11	correction pb desalloc 64bits 1.12
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao203.c	1.10 01/22/98 : xzao203.c" ;

/* declaration de fonctions internes */


/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO203SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO203_Liste_Eqt_GTC_NICE (	XDY_Basedd		va_Basedd_in,
					XDY_NomEqt  		va_NomMachine_in,
					XDY_FonctionInt  	va_FonctionUtilisateurTube_in,
					XDY_FonctionInt  	va_FonctionUtilisateurEchangeur_in,
					XDY_FonctionInt  	va_FonctionUtilisateurSeqTube_in,
					XDY_FonctionInt  	va_FonctionUtilisateurSeqEchangeur_in,
					XDY_FonctionInt  	va_FonctionUtilisateurCapteurTube_in,
					XDY_FonctionInt  	va_FonctionUtilisateurFAV_in,
					XDY_FonctionInt  	va_FonctionUtilisateurPanneau_in,
					XZAOT_GTC_NICE		*va_GTC_NICE_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_Basedd_in		
* XDY_Machine		va_NomMachine_in		
* XDY_FonctionInt  	va_FonctionUtilisateurTube_in
* XDY_FonctionInt  	va_FonctionUtilisateurEchangeur_in
* XDY_FonctionInt  	va_FonctionUtilisateurSeqTube_in
* XDY_FonctionInt  	va_FonctionUtilisateurSeqEchangeur_in
* XDY_FonctionInt  	va_FonctionUtilisateurCapteurTube_in
* XDY_FonctionInt  	va_FonctionUtilisateurFAV_in
* XDY_FonctionInt  	va_FonctionUtilisateurPanneau_in
*				
* PARAMETRES EN SORTIE :
*
* XZAOT_GTC_NICE	va_GTC_NICE_out
*
* VALEUR RENDUE :
*
* Retourne la GTC Nice de la Machine 
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
* Pour chaque Tube appel de la fonction utilisateur avec XZAOT_Tube
* Pour chaque Echangeur appel de la fonction utilisateur avec XZAOT_Echangeur
* Pour chaque SeqTube appel de la fonction utilisateur avec XZAOT_SeqTube
* Pour chaque SeqEchangeur appel de la fonction utilisateur avec XZAOT_SeqEchangeur
* Pour chaque CapteurTube appel de la fonction utilisateur avec XZAOT_CapteurTube
* Pour chaque FAVTube appel de la fonction utilisateur avec XZAOT_FAVTube
* Pour chaque Panneau appel de la fonction utilisateur avec XZAOT_Panneau
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao203.c	1.10 01/22/98 : XZAO203_Liste_Eqt_GTC_NICE" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO203_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO203_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAOC_XZAO203_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue = -1;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	XDY_Entier	vl_Num = 0;
	XDY_Entier	vl_Resultat = 0;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XZAOT_Tube		vl_Tube;
	XZAOT_DemiEchangeur	vl_Echangeur;
	XZAOT_SeqTube		vl_SeqTube;
	XZAOT_SeqEch		vl_SeqEchangeur;
	XZAOT_CapteurTube	vl_Capteur;
	XZAOT_FAVTube		vl_FAV;
	XZAOT_PnGTC		vl_PnGTC;
	
	XDY_Eqt			vl_Numero;
	XDY_NomEqt		vl_NomServeur;
	XDY_Mot			vl_LongueurITOR;
	XDY_Mot			vl_AdresseITOR;
	XDY_Mot			vl_LongueurETOR;
	XDY_Mot			vl_AdresseETOR;
	XDY_Mot			vl_LongTelealarme;
	XDY_Mot			vl_AdresseTelealarme;
	XDY_Mot			vl_LongTelecomEch;
	XDY_Mot			vl_AdresseTelecomEch;
	XDY_Mot			vl_LongTelecomTub;
	XDY_Mot			vl_AdresseTelecomTub;
	XDY_Mot			vl_LongTelesignEch;
	XDY_Mot			vl_AdresseTelesignEch;	
	XDY_Mot			vl_LongTelesignTub;
	XDY_Mot			vl_AdresseTelesignTub;
	XDY_Mot			vl_LongSeqEch;
	XDY_Mot			vl_AdresseSeqEch;
	XDY_Mot			vl_LongSeqTub;
	XDY_Mot			vl_AdresseSeqTub;
	XDY_Mot			vl_LongAlerteEch;
	XDY_Mot			vl_AdresseAlerteEch;
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO203_Liste_Eqt_GTC_NICE : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
					
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)&va_GTC_NICE_out->Numero;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->Numero,"@va_Numero_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(Numero) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[1].pt_value = (CS_VOID *)&va_GTC_NICE_out->NomServeur;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)&va_GTC_NICE_out->NomServeur,"@va_NomServeur_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_char_output(NomServeur) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[2].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongueurITOR;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongueurITOR,"@va_LongueurITOR_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongueurITOR) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[3].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseITOR;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseITOR,"@va_AdresseITOR_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseITOR) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[4].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongueurETOR;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongueurETOR,"@va_LongueurETOR_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongueurETOR) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[5].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseETOR;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseETOR,"@va_AdresseETOR_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseETOR) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[6].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongTelealarme;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongTelealarme,"@va_LongTelealarme_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongTelealarme) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[7].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseTelealarme;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseTelealarme,"@va_AdresseTelealarme_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseTelealarme) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[8].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongTelecomEch;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongTelecomEch,"@va_LongTelecomEch_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongTelecomEch) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[9].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseTelecomEch;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseTelecomEch,"@va_AdresseTelecomEch_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseTelecomEch) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[10].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongTelecomTub;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongTelecomTub,"@va_LongTelecomTub_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongTelecomTub) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[11].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseTelecomTub;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseTelecomTub,"@va_AdresseTelecomTub_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseTelecomTub) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[12].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongTelesignEch;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongTelesignEch,"@va_LongTelesignEch_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongTelesignEch) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[13].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseTelesignEch;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseTelesignEch,"@va_AdresseTelesignEch_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseTelesignEch) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[14].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongTelesignTub;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongTelesignTub,"@va_LongTelesignTub_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongTelesignTub) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[15].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseTelesignTub;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseTelesignTub,"@va_AdresseTelesignTub_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseTelesignTub) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[16].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongSeqEch;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongSeqEch,"@va_LongSeqEch_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongSeqEch) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[17].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseSeqEch;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseSeqEch,"@va_AdresseSeqEch_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseSeqEch) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[18].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongSeqTub;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongSeqTub,"@va_LongSeqTub_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongSeqTub) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[19].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseSeqTub;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseSeqTub,"@va_AdresseSeqTub_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseSeqTub) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[20].pt_value = (CS_VOID *)&va_GTC_NICE_out->LongAlerteTub;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->LongAlerteTub,"@va_LongAlerteTub_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(LongAlerteTub) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[21].pt_value = (CS_VOID *)&va_GTC_NICE_out->AdresseAlerteTub;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_GTC_NICE_out->AdresseAlerteTub,"@va_AdresseAlerteTub_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: asql_ctparam_smallint_output(AdresseAlerteTub) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
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
						vl_Num ++;
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao203: ROW RESULTS");
						vl_Resultat = 0;
	         							
						switch (vl_Num)
						{
						
						   case 1 :  for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	   						     {
             							/*B
             							** Si le numero de la TUB est different du precedent
              							** Remplissage de la structure XZAOT_Tube
              							** Appel de la fonction utilisateur FonctionUtilisateurTube
              							*/
           
              							vl_Resultat ++;
              							
              							vl_Tube.Numero	 	  = * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
                 						vl_Tube.AdresseEtat 	  = * (XDY_Mot *) vl_Tab_Ligne[i][1].pt_value;
                 						vl_Tube.AdresseCommande	  = * (XDY_Mot *) vl_Tab_Ligne[i][2].pt_value;
                 						vl_Tube.AdresseDiscord 	  = * (XDY_Mot *) vl_Tab_Ligne[i][3].pt_value;
                 						vl_Tube.AdresseExtincteur = * (XDY_Mot *) vl_Tab_Ligne[i][4].pt_value;
                 						vl_Tube.AdresseLit 	  = * (XDY_Mot *) vl_Tab_Ligne[i][5].pt_value;
                 						vl_Tube.AdresseOPCO 	  = * (XDY_Mot *) vl_Tab_Ligne[i][6].pt_value;
                 						vl_Tube.AdresseTelealarme = * (XDY_Mot *) vl_Tab_Ligne[i][7].pt_value;
                 						vl_Tube.AdresseGabarit    = * (XDY_Mot *) vl_Tab_Ligne[i][8].pt_value;
                 						vl_Tube.AdresseVitesse    = * (XDY_Mot *) vl_Tab_Ligne[i][9].pt_value;
                 						vl_Tube.AdresseNiche      = * (XDY_Mot *) vl_Tab_Ligne[i][10].pt_value;
                 						
              
	         						if ( va_FonctionUtilisateurTube_in == NULL)
	         						{
	           							XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : va_FonctionUtilisateurTube est nulle");
	            							asql_clean_cmd(connection, cmd);
	   								return (XDC_NOK);
	         						}
	         						else
	         						{
	            							if ( ( retcode = (*va_FonctionUtilisateurTube_in) ( vl_Tube, vl_Resultat ) ) != XDC_OK )
	            							{
	               								XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : appel de va_FonctionUtilisateurTube s'est mal deroule");
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
	   								XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
          								asql_clean_cmd(connection, cmd);
	   								return (XDC_NOK);
								}	
 							     
 							     break;
	         						
	         				   
	         				   
	         				   case 2 :  for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	   						     {
             							/*B
              							** Remplissage de la structure XZAOT_Echangeur
              							** Appel de la fonction utilisateur FonctionUtilisateurEchangeur
              							*/ 
           							
           							vl_Resultat ++;
           							
              							vl_Echangeur.NumeroDemiEch	= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
              							vl_Echangeur.AdresseEtat 	= * (XDY_Mot *) vl_Tab_Ligne[i][1].pt_value;
              							vl_Echangeur.AdresseCommande 	= * (XDY_Mot *) vl_Tab_Ligne[i][2].pt_value;
              							vl_Echangeur.AdresseDiscordance = * (XDY_Mot *) vl_Tab_Ligne[i][3].pt_value;
              							vl_Echangeur.AdresseTelealarme 	= * (XDY_Mot *) vl_Tab_Ligne[i][4].pt_value;
              							
              							
	     							if ( va_FonctionUtilisateurEchangeur_in == NULL)
	      							{
	     							   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : va_FonctionUtilisateurEchangeur est nulle");
	      							   asql_clean_cmd(connection, cmd);
	   							   return (XDC_NOK);
	      							}
	     							 else
	      							{
	        							 if ( ( retcode = (*va_FonctionUtilisateurEchangeur_in) ( vl_Echangeur, vl_Resultat ) ) != XDC_OK )
	        							 {
	         							   	XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : appel de va_FonctionUtilisateurEchangeur s'est mal deroule");
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
	   								XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
          								asql_clean_cmd(connection, cmd);
	   							  	return (XDC_NOK);
								}
	      					             
	      					             
	      					             
	      					             break;
	      					
	      					
	      					   
	      					   case 3 :  for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	   						     {
             							/*B
              							** Remplissage de la structure XZAOT_SeqTube
              							** Appel de la fonction utilisateur FonctionUtilisateurSeqTube
              							*/ 
           
              							vl_Resultat ++;
              							
              							vl_SeqTube.NumTube 	= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
              							vl_SeqTube.NumSequence 	= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
              							vl_SeqTube.Adresse 	= * (XDY_Mot *) vl_Tab_Ligne[i][2].pt_value;
              							vl_SeqTube.BitCommande 	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
              							vl_SeqTube.BitDisc 	= * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
              							
              							if ( va_FonctionUtilisateurSeqTube_in == NULL)
	      							{
	     							   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : va_FonctionUtilisateurSeqTube est nulle");
	      							   asql_clean_cmd(connection, cmd);
	   							   return (XDC_NOK);
	      							}
	     							 else
	      							{
	        							 if ( ( retcode = (*va_FonctionUtilisateurSeqTube_in) ( vl_SeqTube, vl_Resultat ) ) != XDC_OK )
	        							 {
	         							   	XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : appel de va_FonctionUtilisateurSeqTube s'est mal deroule");
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
	   								XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
          								asql_clean_cmd(connection, cmd);
	   							   	return (XDC_NOK);
								}
	      					             
	      					             
	      					             
	      					             break;
	      					
	      					
	      					   
	      					   case 4 :  for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	   						     {
             							/*B
              							** Remplissage de la structure XZAOT_SeqEchangeur
              							** Appel de la fonction utilisateur FonctionUtilisateurSeqEchangeur
              							*/ 
           
              							vl_Resultat ++;
              							
              							vl_SeqEchangeur.NumDemiEch 	= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
              							vl_SeqEchangeur.NumSequence 	= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
              							vl_SeqEchangeur.Adresse 	= * (XDY_Mot *) vl_Tab_Ligne[i][2].pt_value;
              							vl_SeqEchangeur.BitCommande 	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
              							vl_SeqEchangeur.BitDisc 	= * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
              							
              							
              							if ( va_FonctionUtilisateurSeqEchangeur_in == NULL)
	      							{
	     							   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : va_FonctionUtilisateurSeqEchangeur est nulle");
	      							   asql_clean_cmd(connection, cmd);
	   							   return (XDC_NOK);
	      							}
	     							 else
	      							{
	        							 if ( ( retcode = (*va_FonctionUtilisateurSeqEchangeur_in) ( vl_SeqEchangeur, vl_Resultat ) ) != XDC_OK )
	        							 {
	         							   	XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : appel de va_FonctionUtilisateurSeqEchangeur s'est mal deroule");
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
	   								XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
          								asql_clean_cmd(connection, cmd);
	   							   	return (XDC_NOK);
								}
	      					             
	      					             
	      					             
	      					             break;
	      					
	      					
	      					   
	      					   case 5 :  for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	   						     {
             							/*B
              							** Remplissage de la structure XZAOT_CaptTun
              							** Appel de la fonction utilisateur FonctionUtilisateurCapteurTube
              							*/ 
           
              							vl_Resultat ++;
              							
              							vl_Capteur.NumeroTube	= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
              							vl_Capteur.NumeroCapteur= * (XDY_Eqt *) vl_Tab_Ligne[i][1].pt_value;
              							vl_Capteur.Type		= * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value;
              							vl_Capteur.NumBit	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
              							vl_Capteur.NumAlerte	= * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
              							
	     							if ( va_FonctionUtilisateurCapteurTube_in == NULL)
	      							{
	     							   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : va_FonctionUtilisateurCapteurTube est nulle");
	      							   asql_clean_cmd(connection, cmd);
	   							   return (XDC_NOK);
	      							}
	     							 else
	      							{
	        							 if ( ( retcode = (*va_FonctionUtilisateurCapteurTube_in) ( vl_Capteur, vl_Resultat ) ) != XDC_OK )
	        							 {
	         							   	XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : appel de va_FonctionUtilisateurCapteurTube s'est mal deroule");
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
	   								XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
          								asql_clean_cmd(connection, cmd);
	   							   	return (XDC_NOK);
								}
	      					             
	      					             
	      					             
	      					             break;
	      					
	      					
	      					   
	      					   case 6 :  for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	   						     {
             							/*B
              							** Remplissage de la structure XZAOT_FAVTube
              							** Appel de la fonction utilisateur FonctionUtilisateurFAV
              							*/ 
           
              							vl_Resultat ++;
              							
              							vl_FAV.NumeroTube		= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
              							vl_FAV.NumeroFAV	= * (XDY_Eqt *) vl_Tab_Ligne[i][1].pt_value;
              							vl_FAV.RangComm		= * (XDY_Octet *) vl_Tab_Ligne[i][2].pt_value;
              							vl_FAV.RangComm2	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
              							vl_FAV.RangEtat		= * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
              							vl_FAV.RangEtat2	= * (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value;
              							vl_FAV.BitDiscordance	= * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
              							vl_FAV.BitDiscordance2	= * (XDY_Octet *) vl_Tab_Ligne[i][7].pt_value;
              							
	     							if ( va_FonctionUtilisateurFAV_in == NULL)
	      							{
	     							   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : va_FonctionUtilisateurFAV est nulle");
	      							   asql_clean_cmd(connection, cmd);
	   							   return (XDC_NOK);
	      							}
	     							 else
	      							{
	        							 if ( ( retcode = (*va_FonctionUtilisateurFAV_in) ( vl_FAV, vl_Resultat ) ) != XDC_OK )
	        							 {
	         							   	XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : appel de va_FonctionUtilisateurFAV s'est mal deroule");
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
	   								XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
          								asql_clean_cmd(connection, cmd);
	   							   	return (XDC_NOK);
								}
	      					             
	      					             
	      					             
	      					             break;
	      					
	      					   
	      					   
	      					   case 7 :  for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	   						     {
             							/*B
              							** Remplissage de la structure XZAOT_PnGTC
              							** Appel de la fonction utilisateur FonctionUtilisateurPnGTC
              							*/ 
           
              							vl_Resultat ++;
              							
              							vl_PnGTC.NumMaitre	= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
              							vl_PnGTC.TypeMaitre 	= * (XDY_TypeEqt *) vl_Tab_Ligne[i][1].pt_value;
              							vl_PnGTC.NumeroPnGTC	= * (XDY_Eqt *) vl_Tab_Ligne[i][2].pt_value;
              							vl_PnGTC.RangComm	= * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
              							vl_PnGTC.RangComm2	= * (XDY_Octet *) vl_Tab_Ligne[i][4].pt_value;
              							vl_PnGTC.RangEtat	= * (XDY_Octet *) vl_Tab_Ligne[i][5].pt_value;
              							vl_PnGTC.RangEtat2	= * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
              							vl_PnGTC.BitDiscordance	= * (XDY_Octet *) vl_Tab_Ligne[i][7].pt_value;
              							vl_PnGTC.BitDiscordance2= * (XDY_Octet *) vl_Tab_Ligne[i][8].pt_value;
              							
	     							if ( va_FonctionUtilisateurPanneau_in == NULL)
	      							{
	     							   XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : va_FonctionUtilisateurPanneau est nulle");
	      							   asql_clean_cmd(connection, cmd);
	   							   return (XDC_NOK);
	      							}
	     							 else
	      							{
	        							 if ( ( retcode = (*va_FonctionUtilisateurPanneau_in) ( vl_PnGTC, vl_Resultat ) ) != XDC_OK )
	        							 {
	         							   	XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : appel de va_FonctionUtilisateurPanneau s'est mal deroule");
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
	   								XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
          								asql_clean_cmd(connection, cmd);
	   							  	return (XDC_NOK);
								}
	      					             
	      					             
	      					             
	      					             break;
	      					
	      					       }
	      					         
	      					       break;
	      					
	      					
	      				case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao203 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao203 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao203 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao203: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
/*	
	if (vl_Nb_Ligne_Lue >= 0 )
	{
	   retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	   if (retcode != XDC_OK)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzao203 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
              asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	}	
 */	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO203_Liste_Eqt_GTC_NICE retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




