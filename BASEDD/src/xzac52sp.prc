/*E*/
/*  Fichier : $Id: xzac52sp.prc,v 1.14 2021/03/08 14:35:58 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2021/03/08 14:35:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac52sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	21/09/94	: Creation
* C.T.	22/09/94	: Modif numero de commande 
*                         pour ACRP06 est negatif (V 1.3)
* C.T.	10/11/94	: Modif bugs applix (V 1.4)
* C.T.	28/03/95	: Modif numero de sequence de fin (V 1.5)
* B.G.	12/04/95	: modif dernier EQT_DSP (1.6)
* B.G.  12/04/95        : modif dernier EQT_DSP (1.7)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.8)
* JMG           02/10/09        : secto DEM887 1.9
* PNI           19/04/16 : Correction des appels dans PRC des XZAC sur les serveurs sites v1.10 DEM1159
*  JMG           03/03/17 : regio DEM1220
* LCL	30/06/17	: Modif PRA
* LCL	19/11/19	: MOVIS DEM-SAE93 1.13
* GRO   12/10/2020	: MOVIS DEM-SAE93 1.14
* CGR	05/01/21	: MOVIS ajout PRA DEM-SAE93 1.14
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Remet le tunnel a l'etat normal
*
* Sequence d'appel
* SP	XZAC52_Commande_Fin_Tunnel
*
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* XDY_Booleen	va_Manuelle_in
*
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: action non trouvee ou site incorrect
*                 update impossible
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
* La commande est executee si Manuelle est a XDC_Faux
*
* Fonction
* Verifier la non indisponibilite de l'equipement.
* Inserer dans la table ACTION la date de fin de l'actio.
* Dans le cas de commande automatique :
* 	ACRP52 :	Appeler XZET03 pour commander l'equipement
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAC;52
	@va_NumeroAction_in		int = null,		
	@va_Site_in			T_SITE = null,
	@va_HeureLancementFin_in	datetime = null,
	@va_Manuelle_in			bit,
	@va_NomSiteLocal_in		char(2)	= null,
	@va_distant_in 			int = XDC_NON										
as
	declare	@vl_NumEqt T_EQUIPEMENT, @vl_Status int, @vl_Sequence tinyint,
		@vl_Commande int, @vl_Site tinyint,
		@vl_serveur_pra char(6)

	/*! transfo en param�tres nulls bug applix */
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null

	/*A controle des parametres obligatoires */
	if  @va_NumeroAction_in = null or @va_Site_in = null or 
	    @va_HeureLancementFin_in = null 						
		return XDC_ARG_INV

	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select  @vl_Site = numero
		from CFG..RES_DIS
	        where serveur = @@servername
	else
	begin	
		if @va_distant_in = XDC_NON
                begin
                	select  @vl_Site = numero
	        	from CFG..RES_DIS
			where code = @va_NomSiteLocal_in		
                end
                else
                begin
                        select @vl_Site = @va_Site_in			
                end	
	end

	/* Init serveur PRA */
	select @vl_serveur_pra = serveur_pra from CFG..RES_PRA where numero=@va_Site_in

	if @va_NomSiteLocal_in = XDC_CI	and @va_distant_in = XDC_NON
	/* Traitement au district d'exploitation pour Escota, en local sinon */
	begin
		  if @vl_serveur_pra = XDC_SQLVC
			  exec @vl_Status = SQL_DM.PRC..XZAC;52 @va_NumeroAction_in,  @va_Site_in, @va_HeureLancementFin_in, @va_Manuelle_in, 
				@va_NomSiteLocal_in, XDC_OUI

		  if @vl_serveur_pra = XDC_SQLDP
			  exec @vl_Status = SQL_DS.PRC..XZAC;52 @va_NumeroAction_in,  @va_Site_in, @va_HeureLancementFin_in, @va_Manuelle_in, 
				@va_NomSiteLocal_in, XDC_OUI

		  if @vl_serveur_pra = XDC_SQLCI
			  exec @vl_Status = SQL_CI.PRC..XZAC;52 @va_NumeroAction_in,  @va_Site_in, @va_HeureLancementFin_in, @va_Manuelle_in, 
				@va_NomSiteLocal_in, XDC_OUI

		   return @vl_Status
	end
		
	/*A tester si le site est correct */
	if @va_NomSiteLocal_in = null
	begin
		if not exists ( select numero from CFG..RES_DIS
                                where serveur = @@servername and type <> XDC_TYPE_PCNIVEAU3 )
		return XDC_NOK
	end
	else
	begin
		if not exists ( select numero from CFG..RES_DIS
                                where code = @va_NomSiteLocal_in and type <> XDC_TYPE_PCNIVEAU3 )
		return XDC_NOK
	end

	/*A Verifier la non indisponibilite de l'equipement */
	select @vl_NumEqt = EXP..ACT_GEN.equipement 
	from EXP..EQT_DSP, EXP..ACT_GEN
	where 	EXP..ACT_GEN.numero = @va_NumeroAction_in and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GEN.type = XDC_ACT_TUNNEL and
		EXP..EQT_DSP.equipement = EXP..ACT_GEN.equipement and
		EXP..EQT_DSP.type = XDC_EQT_TUBE and
		EXP..EQT_DSP.majeure = XDC_NON and
		EXP..EQT_DSP.HS = XDC_NON and
		EXP..EQT_DSP.desactive = XDC_NON and
		EXP..EQT_DSP.critique = XDC_NON and
		EXP..EQT_DSP.dernier = XDC_OUI

	if @@rowcount != 1
		return XDC_NOK

	/*A mettre a jour la date de lancement fin */
	update EXP..ACT_GEN set heure_lancement_fin = @va_HeureLancementFin_in
	where	numero = @va_NumeroAction_in and sit = @va_Site_in and
		heure_lancement_fin is null

	if @@rowcount > 1
		return XDC_NOK

	/*A cas de commande automatique par sequence */
	if @va_Manuelle_in =XDC_NON
	begin
		/*A sequence a utiliser */
		select @vl_Sequence = XDC_SEQ_OUV_TUN
	
		/*A numero de commande a envoyer : - numero action */
		select @vl_Commande = - @va_NumeroAction_in

		/*A arreter l'affichage tunnel */

		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP06 @vl_NumEqt, @vl_Sequence,
				@vl_Commande	

		else if @vl_Site = XDC_DP			
			exec @vl_Status = TASRV_DP...ACRP06 @vl_NumEqt, @vl_Sequence,
				@vl_Commande

		else if @vl_Site = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP06 @vl_NumEqt, @vl_Sequence,
				@vl_Commande
	
		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP06 @vl_NumEqt, @vl_Sequence,
				@vl_Commande
	
		if @vl_Status != XDC_OK
			return @vl_Status 
	end

	return XDC_OK

go
