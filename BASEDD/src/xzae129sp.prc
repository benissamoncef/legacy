/*E*/
/*Fichier : $Id: xzae129sp.prc,v 1.6 2008/12/10 13:19:26 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2008/12/10 13:19:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae129sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* EXPTORIQUE :
*
* JMG	13/05/97	: Creation
* PN	23/12/98	: Ajout en sortie du nb de PL dem/1724 1.4
* PC	15/01/07	: Ajout parametre de sortie pour METACOR DEM605 v1.3
* JMG	03/12/08	: ajout validation dans FMC_ACC 1.4 DEM/852
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'un complement de fiche main courante 
* de type accident 
* 
* Sequence d'appel
* SP	XZAE129_Lire_Fiche_Accident_Pour_Strada
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet		va_PL_out
* XDY_Octet		va_VL_out
* XDY_Octet		va_Car_out
* XDY_Octet             va_Rem_out
* XDY_Octet		va_BlesseGra_out
* XDY_Octet		va_Mort_out
* XDY_Booleen		va_MatDang_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* 
* La procedure stockee au CI recherche les informations 
* dans la base EXP 
* 
* Fonction
* Select dans les tables ACCIDENT 
* where numero numero
------------------------------------------------------*/

use PRC
go

drop proc XZAE129
go

create procedure XZAE129
	@va_NumEvt_in		int ,
	@va_CleEvt_in		tinyint ,
	@va_VL_out		tinyint = null	output,
	@va_PL_out		tinyint = null	output,
	@va_Car_out		tinyint = null	output,
	@va_Rem_out             tinyint = null  output,
	@va_Blesse_out		tinyint = null	output,
	@va_Mort_out		tinyint = null	output,
	@va_MatDang_out		bit = XDC_NON	output,
	@va_VehEscota_out       bit = XDC_NON   output,
	@va_HomEscota_out       bit = XDC_NON   output,
	@va_Personalite_out     bit = XDC_NON   output,
	@va_Blesses_graves_out  tinyint = null output
as
	declare @vl_VL_in tinyint, @vl_PL_in tinyint,
		@vl_Remorque_in tinyint, @vl_motos_in tinyint,
		@vl_bl_in tinyint, @vl_bg_in tinyint

	/*A Controle les arguments en entree obligatoire */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*B lecture des infos sur l'accident */
	select 
			@vl_VL_in		= nombre_de_vl,
			@va_PL_out		= nombre_de_pl,
			@va_Rem_out		= nombre_de_remorques,
			@va_Car_out		= nombre_de_car,
			@vl_motos_in		= nombre_de_moto,
			@vl_bl_in		= blesses_legers,
			@vl_bg_in		= blesses_graves,
			@va_Mort_out		= morts,
			@va_VehEscota_out	= vehicule_ESCOTA,
			@va_MatDang_out		= matieres_dangereuses,
			@va_HomEscota_out	= personnel_ESCOTA,
			@va_Personalite_out	= personnalites
	from EXP..FMC_ACC
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in 
	group by numero, cle
	having validation = max(validation)

	/*B test si complement accident a ete trouve */
	if @@rowcount = 0
		return XDC_NOK

	select @va_VL_out=@vl_VL_in+@va_PL_out+@va_Rem_out+@vl_motos_in
			 +@va_Car_out
	select @va_Blesse_out=@vl_bl_in + @vl_bg_in

	/* METACOR */
	select @va_Blesses_graves_out = @vl_bg_in
	
	return XDC_OK
go
