/*E*/
/* Fichier : $Id: xzae20.c,v 1.26 2021/05/26 14:45:25 devgfi Exp $      Release : $Revision: 1.26 $        Date : $Date: 2021/05/26 14:45:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzae20.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface des evenements
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       20 Aug 1994     : Creation
* volcic       14 Oct 1994     : Ajout 4 parametres de sorties (v 1.4)
* volcic       25 Nov 1994     : Ajout asql_clean_cmd et asql_cmd_alloc (v 1.5)
* MERCIER	5 Dec 1994	: Ajout de 2 arguments en sortie de xzae20 V.10
* va_AbrPtCar_out et va_NumPtCar_out
* volcic        5 Dec 1994     : Initialisation de Nb_Ligne_Lue et Nb_Col (v 1.11)
* gaborit      19 jan 1995     : modif bug envoi commentaires (v 1.13)
* gaborit      19 Avr 1995     : modif traces (v 1.14)
* torregrossa  29 Jun 1995     : ajout la recherche de la sortie pour 
*                                un point caract. de type echangeur et retourne
*                                le numero de la sortie a la place du numero de
*                                l'echangeur (v 1.15)
* gaborit      19 jui 1995	: modif asql_clean (1.16)
* torregrossa  23 Jan 1996     : ajout la recherche de la position (V1.17)
* torregrossa  25 sep 1996     : ajout du parametre degats domaine (FMC10) (V1.18)
* torregrossa  25 sep 1996     : ajout du parametre type anterieur (FMC3) (V1.19)
* torregrossa  20 nov 1996     : ajout du parametre fausse_alerte (V1.20)
* torregrossa  20 nov 1996     : ajout du parametre BAU_etroite BAU_I_etroite (V1.21)
* torregrossa  15 Dec 1997     : ajout du parametre degrade (1531) (V1.22)
* niepceron    13 JAn 1999     : ajout du parametre intergest (1624) (V1.24)
* JPL		25/07/12 : Suppression lecture numero echangeur (devenu libelle) par xzao312 (DEM 1033)  1.25
* JPL		26/05/21 : Initialisations supprimant des avertissements mineurs a la compilation  1.26
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae20.h"

static char *version = "$Id: xzae20.c,v 1.26 2021/05/26 14:45:25 devgfi Exp $ : xzae20" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit les informations generiques d'une fiche main courante È l'heure precisee
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE20_Decrire_Evt_PA (	XDY_Horodate		va_Horodate,
				XDY_Evt			va_NumEvt,
				XDY_Fonction		va_FonctionUtilisateurCommentaire,
				XZAET_Evenement 	*va_Description,
				XZAET_Voies 		*va_VoiesCirculees)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate		va_Horodate
* XDY_Evt		va_NumEvt
* XDY_Fonction		va_FonctionUtilisateurCommentaire	
*
* ARGUMENTS EN SORTIE :
*
* XZAET_Evenement 	va_Description
* XZAET_Voies 		va_VoiesCirculees
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* Appelle XZAE40SP
*    Pour chaque commentaires, appelle la fonction :
*       FonctionUtilisateurCommentaire avec la structure XZAET_Commentaire
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzae20.c,v 1.26 2021/05/26 14:45:25 devgfi Exp $ : XZAE20_Decrire_Evt_PA" ;
	
	CS_CHAR *rpc_name =XZAEC_XZAE20_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAEC_XZAE20_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAEC_XZAE20_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables intermediaires pour la conversion des structures XZAET_Evenement et XZAET_Voies
	*/

	XDY_Entier	va_Numero_out = 0;
	XDY_Octet	va_Cle_out = 0;
	XDY_Octet	va_Site_out = 0;
	XDY_Octet	va_Type_out = 0;
	XDY_Mot		va_TypeAnterieur_out = 0;
	CS_DATETIME	va_DebutPrevu_out;
	CS_DATETIME	va_Debut_out;
	CS_DATETIME	va_FinPrevu_out;
	CS_DATETIME	va_Fin_out;
	CS_DATETIME	va_Validation_out;
	XDY_Entier	va_NumCause_out = 0;
	XDY_Octet	va_CleCause_out = 0;
	XDY_Octet	va_TypeCause_out = 0;
	XDY_Alerte	va_NumAlerte_out = 0;
	CS_DATETIME	va_HoroAlerte_out;
	XDY_Nom		va_TypeAlerte_out;
	XDY_Nom		va_Origine_out;
	XDY_Booleen	va_Confirme_out = XDC_FAUX;
	XDY_Autoroute	va_Autoroute_out = XDC_AUT_INCONNUE;
	XDY_PR		va_PR_out = 0;
	XDY_Sens	va_Sens_out = XDC_SENS_INCONNU;
	XDY_Octet	va_PointCar_out = XDC_POINT_CARACT_AUCUN;
	XDY_Nom		va_NomPtCar_out = "";
	XDY_Abrev	va_AbrPtCar_out = "";
	XDY_Octet	va_NumPtCar_out = 0;
	XDY_Octet	va_Position_out = 0;
	XDY_Octet	va_Degats_out = 0;
	XDY_Octet	va_FausseAlerte_out = 0;
	XDY_Octet	va_BAU_Etroite_out = 0;
	XDY_Octet	va_BAU_I_Etroite_out = 0;
	XDY_Octet	vl_Degrade = 0;
	XDY_Entier	vl_NumInit = 0;
	XDY_Octet	vl_CleInit = 0;
	XDY_Octet	vl_SiteAlerte = 0;
	XDY_Octet	vl_Datex = XDC_FAUX;
	XDY_Octet	vl_Gravite = 0;
	XDY_Booleen	vl_Bloquant = XDC_FAUX;
	XDY_Octet	vl_Trafic = 0;
	XDY_Octet	vl_Duree = 0;
	
	XDY_Voie	va_VR_out = XDC_VOIE_INCONNUE;
	XDY_Voie	va_VM2_out = XDC_VOIE_INCONNUE;
	XDY_Voie	va_VM1_out = XDC_VOIE_INCONNUE;
	XDY_Voie	va_VL_out = XDC_VOIE_INCONNUE;
	XDY_Voie	va_BAU_out = XDC_VOIE_INCONNUE;
	XDY_Voie	va_VR_I_out = XDC_VOIE_INCONNUE;
	XDY_Voie	va_VM2_I_out = XDC_VOIE_INCONNUE;
	XDY_Voie	va_VM1_I_out = XDC_VOIE_INCONNUE;
	XDY_Voie	va_VL_I_out = XDC_VOIE_INCONNUE;
	XDY_Voie	va_BAU_I_out = XDC_VOIE_INCONNUE;

	XZAET_Commentaire	vl_Commentaire;
	
	XDY_Autoroute	va_AutorouteCause_out = 0;
	XDY_PR		va_PRCause_out = 0;
	XDY_Sens	va_SensCause_out = 0;
	CS_DATETIME	va_HoroCause_out;
	
	
	/*A
	** Definition des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	XDY_Entier	va_NumEvt_in = 0;
	XDY_Octet	va_CleEvt_in = 0;
	
	/*A
	** Definition des variables intermediaires pour la conversion des dates au format Sybase
	*/
	
	CS_DATETIME	va_Horodate_in;
	
	XDY_Horodate	va_DebutPrevu = 0.0;
	XDY_Horodate	va_Debut = 0.0;
	XDY_Horodate	va_FinPrevu = 0.0;
	XDY_Horodate	va_Fin = 0.0;
	XDY_Horodate	va_Validation = 0.0;
	XDY_Horodate	va_HoroAlerte = 0.0;
	
	
        /*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		return (XDC_NOK);
	}
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_NumEvt_in = va_NumEvt.numero;
	va_CleEvt_in = va_NumEvt.cle;
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAE20_Decrire_Evt_PA : va_Days_in=%d va_Time_in=%d va_NumEvt_in=%d va_CleEvt_in=%d",va_Horodate_in.dtdays,va_Horodate_in.dttime,va_NumEvt_in,va_CleEvt_in);
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&va_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_int_input(NumEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_input(CleEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Numero_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&va_Numero_out,"@va_Numero_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_int_output(Numero) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Cle_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Cle_out,"@va_Cle_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Cle) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Site_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Site_out,"@va_Site_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Site) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Type_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Type_out,"@va_Type_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Type) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_DebutPrevu_out;
	
	if ((retcode = asql_ctparam_date_output(cmd,(CS_DATETIME *)&va_DebutPrevu_out,"@va_DebutPrevu_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_date_output(DebutPrevu) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Debut_out;
	
	if ((retcode = asql_ctparam_date_output(cmd,(CS_DATETIME *)&va_Debut_out,"@va_Debut_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_date_output(Debut) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_FinPrevu_out;
	
	if ((retcode = asql_ctparam_date_output(cmd,(CS_DATETIME *)&va_FinPrevu_out,"@va_FinPrevu_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_date_output(FinPrevu) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Fin_out;
	
	if ((retcode = asql_ctparam_date_output(cmd,(CS_DATETIME *)&va_Fin_out,"@va_Fin_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_date_output(Fin) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Validation_out;
	
	if ((retcode = asql_ctparam_date_output(cmd,(CS_DATETIME *)&va_Validation_out,"@va_Validation_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_date_output(Validation) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_NumCause_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&va_NumCause_out,"@va_NumCause_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_int_output(NumCause) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_CleCause_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_CleCause_out,"@va_CleCause_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(CleCause) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_TypeCause_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_TypeCause_out,"@va_TypeCause_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(TypeCause) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_NumAlerte_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&va_NumAlerte_out,"@va_NumAlerte_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_int_output(NumAlerte) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_HoroAlerte_out;
	
	if ((retcode = asql_ctparam_date_output(cmd,(CS_DATETIME *)&va_HoroAlerte_out,"@va_HoroAlerte_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_date_output(HoroAlerte) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)va_TypeAlerte_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_TypeAlerte_out,"@va_TypeAlerte_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_char_output(TypeAlerte) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)va_Origine_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Origine_out,"@va_Origine_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_char_output(Origine) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Confirme_out;
	
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_Confirme_out,"@va_Confirme_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_bit_output(Confirme) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Autoroute_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Autoroute_out,"@va_Autoroute_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Autoroute) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_PR_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&va_PR_out,"@va_PR_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_int_output(PR) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Sens_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Sens_out,"@va_Sens_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Sens) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_PointCar_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_PointCar_out,"@va_PointCar_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(PointCar) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)va_NomPtCar_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_NomPtCar_out,"@va_NomPtCar_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_char_output(NomPtCar) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	


	Tab_donnees2[i].pt_value = (CS_VOID *)&va_VR_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_VR_out,"@va_VR_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(VR) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_VM2_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_VM2_out,"@va_VM2_out")) != CS_SUCCEED)	
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_param(va_VM2_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_VM1_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_VM1_out,"@va_VM1_out")) != CS_SUCCEED)	
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_param(va_VM1_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_VL_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_VL_out,"@va_VL_out")) != CS_SUCCEED)	
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_param(va_VL_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_BAU_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_BAU_out,"@va_BAU_out")) != CS_SUCCEED)	
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_param(va_BAU_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_VR_I_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_VR_I_out,"@va_VR_I_out")) != CS_SUCCEED)	
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_param(va_VR_I_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_VM2_I_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_VM2_I_out,"@va_VM2_I_out")) != CS_SUCCEED)	
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_param(va_VM2_I_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_VM1_I_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_VM1_I_out,"@va_VM1_I_out")) != CS_SUCCEED)	
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_param(va_VM1_I_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_VL_I_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_VL_I_out,"@va_VL_I_out")) != CS_SUCCEED)	
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_param(va_VL_I_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_BAU_I_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_BAU_I_out,"@va_BAU_I_out")) != CS_SUCCEED)	
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_param(va_BAU_I_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_AutorouteCause_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_AutorouteCause_out,"@va_AutorouteCause_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(AutorouteCause) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
        
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_PRCause_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&va_PRCause_out,"@va_PRCause_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_int_output(PRCause) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
        
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_SensCause_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_SensCause_out,"@va_SensCause_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(SensCause) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
        
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_HoroCause_out;
	
	if ((retcode = asql_ctparam_date_output(cmd,(CS_DATETIME *)&va_HoroCause_out,"@va_HoroCause_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_date_output(HoroCause) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Position_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Position_out,"@va_Position_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Position) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decririe_Travaux_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
        
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Degats_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Degats_out,"@va_Degats_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Degats) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_TypeAnterieur_out;
	
	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_TypeAnterieur_out,"@va_TypeAnterieur_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_smallint_output(TypeAnterieur) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_FausseAlerte_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_FausseAlerte_out,"@va_FausseAlerte_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_smallint_output(FausseAlerte) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_BAU_Etroite_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_BAU_Etroite_out,"@va_BAU_Etroite_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_smallint_output(BAU_Etroite) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_BAU_I_Etroite_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_BAU_I_Etroite_out,"@va_BAU_I_Etroite_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_smallint_output(BAU_I_Etroite) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)va_AbrPtCar_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_AbrPtCar_out,"@va_AbrPtCar_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_char_output(AbrPtCar) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_NumPtCar_out;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_NumPtCar_out,"@va_NumPtCar_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(NumPtCar) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_Degrade;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_Degrade,"@va_Degrade_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Degrade) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_NumInit;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&vl_NumInit,"@va_NumEvtInit_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_int_output(NumEvtInit) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_CleInit;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_CleInit,"@va_CleEvtInit_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(CleEvtInit) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_SiteAlerte;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_SiteAlerte,"@va_SiteAlerte_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(SiteAlerte) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_Datex;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_Datex,"@va_Datex_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Datex) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_Trafic;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_Trafic,"@va_Trafic_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Trafic) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_Gravite;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_Gravite,"@va_Gravite_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Gravite) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_Bloquant;
	
	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&vl_Bloquant,"@va_Bloquant_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_bit_output(Bloquant) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&vl_Duree;
	
	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&vl_Duree,"@va_Duree_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_ctparam_tinyint_output(Duree) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	i = i + 1;
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzae22: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae22 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae20 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae20 : STATUS REUSULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				
				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_fetch_data* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}
	
	
	/*A
	** Conversion des dates du format Sybase au format Unix
	*/
	
	if ((retcode = asql_date_Sybase2Ux(va_DebutPrevu_out,&va_DebutPrevu))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Sybase2Ux(va_DebutPrevu_out,&va_DebutPrevu) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	if ((retcode = asql_date_Sybase2Ux(va_Debut_out,&va_Debut))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Sybase2Ux(va_Debut_out,&va_Debut) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	if ((retcode = asql_date_Sybase2Ux(va_FinPrevu_out,&va_FinPrevu))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Sybase2Ux(va_FinPrevu_out,&va_FinPrevu) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	if ((retcode = asql_date_Sybase2Ux(va_Fin_out,&va_Fin))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Sybase2Ux(va_Fin_out,&va_Fin) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	if ((retcode = asql_date_Sybase2Ux(va_Validation_out,&va_Validation))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Sybase2Ux(va_Validation_out,&va_Validation) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	if ((retcode = asql_date_Sybase2Ux(va_HoroAlerte_out,&va_HoroAlerte))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Sybase2Ux(va_HoroAlerte_out,&va_HoroAlerte) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Affectation des structures parametres de sortie
	*/
	
	va_Description->Numero		= va_Numero_out;
	va_Description->Cle		= va_Cle_out;
	va_Description->Site		= va_Site_out;
	va_Description->Type		= va_Type_out;
	va_Description->DebutPrevu 	= va_DebutPrevu;
	va_Description->Debut		= va_Debut;
	va_Description->FinPrevue	= va_FinPrevu;
	va_Description->Fin		= va_Fin;
	va_Description->Validation	= va_Validation;
	va_Description->NumCause	= va_NumCause_out;
	va_Description->CleCause	= va_CleCause_out;
	va_Description->TypeCause	= va_TypeCause_out;
	va_Description->NumAlerte	= va_NumAlerte_out;
	va_Description->HoroAlerte	= va_HoroAlerte;
	strcpy(va_Description->TypeAlerte, va_TypeAlerte_out);
	strcpy(va_Description->Origine, va_Origine_out);
	va_Description->Confirme	= va_Confirme_out;
	va_Description->Autoroute	= va_Autoroute_out;
	va_Description->PR		= va_PR_out;
	va_Description->Sens		= va_Sens_out;
	va_Description->PointCar	= va_PointCar_out;
	strcpy(va_Description->NomPtCar, va_NomPtCar_out);
	strcpy(va_Description->AbrPtCar, va_AbrPtCar_out);
	va_Description->NumPtCar	= va_NumPtCar_out;
	va_Description->Position	= va_Position_out;
	
	va_Description->Datex		= vl_Datex;
	va_Description->Trafic		= vl_Trafic;
	va_Description->Gravite		= vl_Gravite;
	va_Description->Bloquant	= vl_Bloquant;
	va_Description->Duree		= vl_Duree;
	
	va_VoiesCirculees->VR		= va_VR_out;
	va_VoiesCirculees->VM2		= va_VM2_out;
	va_VoiesCirculees->VM1		= va_VM1_out;
	va_VoiesCirculees->VL		= va_VL_out;
	va_VoiesCirculees->BAU		= va_BAU_out;
	va_VoiesCirculees->VR_I		= va_VR_I_out;
	va_VoiesCirculees->VM2_I	= va_VM2_I_out;
	va_VoiesCirculees->VM1_I	= va_VM1_I_out;
	va_VoiesCirculees->VL_I		= va_VL_I_out;
	va_VoiesCirculees->BAU_I	= va_BAU_I_out;
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAE20_Decrire_Evt_PA :\nNumero=%d\nCle=%d\nSite=%d\nType=%d\nDebutPrevu=%lf\nDebut=%lf\nFinPrevu=%lf\nFin=%lf\nValidation=%lf\nNumCause=%d\nCleCause=%d\nTypeCause=%d\nNumAlerte=%d\nHoroAlerte=%lf\nTypeAlerte=%s\nOrigine=%s\nConfirme=%s\nAutoroute=%d\nPR=%ld\nSens=%d\nPointCar=%d\nNomPtCar=%s\nPosition=%d\nAbrPtCar=%s\nNumPtCar=%d\nDegats=%d\ndatex=%d,trafic=%d,gravite=%d,bloq=%d,duree=%d",
			va_Numero_out,
			va_Cle_out,
			va_Site_out,
			va_Type_out,
			va_DebutPrevu,
			va_Debut,
			va_FinPrevu,
			va_Fin,
			va_Validation,
			va_NumCause_out,
			va_CleCause_out,
			va_TypeCause_out,
			va_NumAlerte_out,
			va_HoroAlerte,
			va_TypeAlerte_out,
			va_Origine_out,
			va_Confirme_out,
			va_Autoroute_out,
			va_PR_out,
			va_Sens_out,
			va_PointCar_out,
			va_NomPtCar_out,
			va_Position_out,
			va_AbrPtCar_out,
			va_NumPtCar_out,
			va_Degats_out,
			vl_Datex,vl_Trafic,vl_Gravite,vl_Bloquant,vl_Duree);
			
        
        XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAE20_Decrire_Evt_PA :\nva_VoiesCirculees.VR = %d\nva_VoiesCirculees.VM2 = %d\nva_VoiesCirculees.VM1 = %d\nva_VoiesCirculees.VL = %d\nva_VoiesCirculees.BAU = %d\nva_VoiesCirculees.VR_I  = %d\nva_VoiesCirculees.VM2_I = %d\nva_VoiesCirculees.VM1_I = %d\nva_VoiesCirculees.VL_I = %d\nva_VoiesCirculees.BAU_I = %d\n",
	va_VR_out ,
	va_VM2_out,
	va_VM1_out,
	va_VL_out,
	va_BAU_out,
	va_VR_I_out,
	va_VM2_I_out,
	va_VM1_I_out,
	va_VL_I_out,
	va_BAU_I_out);
	
	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurCommentaire pour
	** chaque commentaire (250 caracteres) 
	*/
	
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
	    
	        vl_Commentaire.Numero= *((XDY_Octet*)(vl_Tab_Ligne[i][0].pt_value));
	      
		if ((retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)( vl_Tab_Ligne[i][1].pt_value),
							&(vl_Commentaire.Horodate)))!= XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Sybase2Ux(vl_Commentaire.Horodate) a echoue");
		}
		
		if ((retcode = asql_date_Sybase2Ux(*(CS_DATETIME*)( vl_Tab_Ligne[i][2].pt_value),
							&(vl_Commentaire.HoroPerime)))!= XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Sybase2Ux(vl_Commentaire.Horodate) a echoue");
		}
	      
	        strcpy(vl_Commentaire.Texte, vl_Tab_Ligne[i][3].pt_value );
	      
	        /*B
	        ** Appel de la fonction utilisateur FonctionUtilisateurCommentaire
	        */
	  
	        if ( va_FonctionUtilisateurCommentaire == NULL )
	        {
		XZST_03EcritureTrace(XZSTC_WARNING,"va_FonctionUtilisateurCommentaire est nulle");
	        asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
		
	        }
	      else
	      {
	         ( *va_FonctionUtilisateurCommentaire ) ( vl_Commentaire ); 
	         
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
 	asql_clean_cmd(connection, cmd);	



	/*A
	** Recherche du numero de sortie de l'echangeur si le point
	** caracteristique est un echangeur
	*/
/*
	strcpy (va_Description->Sortie, "");
	va_Description->Bifurcation = XDC_FAUX;
	if (va_PointCar_out == XDC_POINT_CARACT_ECHANGEUR) 
	{
		retcode = XZAOXXX_Lire_Sortie_Ech(va_Description->NumPtCar,
						  va_Description->Sortie,
						  &va_Description->Bifurcation);
		if (retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,
						"XZAOXXX_Lire_Sortie_Ech retourne XDC_NOK");
		}
	}
*/
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE20_Decrire_Evt_PA retourne %d", retcode);
	return(XDC_OK);
}

