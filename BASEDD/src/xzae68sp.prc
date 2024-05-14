/*E*/
/*  Fichier : $Id: xzae68sp.prc,v 1.10 2007/03/26 15:21:38 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 2007/03/26 15:21:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae68sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	01/09/94	: Creation
* C.T.  06/10/94	: ajout colonne dans LISTE_CONSEQ
*                         pour multi-utilisation de 
*                         la proc	(V 1.4)
* C.T.  18/10/94	: enleve le retour de la liste dans 
*                         le cas d'un appel via une procedure
*                         stockee (pas reteste)	(V 1.5)
* C.T.	10/11/94	: Modif bugs applix (V 1.6)
* C.T.	14/11/94	: Ajout commenatire d'utilisation (V 1.7)
* B.G.	01/12/94	: Ajout date courante si nulle (V 1.8)
* C.T.+B.G.  02/10/95	: effacement de la table LISTE_CONSEQ avant demarrage (1.9)
* P.N.	24/08/05	: prise ne cpte du spid pour le cursor v1.10 DEM501
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE68_ListeConsequences
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in 
* XDY_Octet		va_Effacement_in
* 
* Arguments en sortie
* XDY_Entier		va_NumEvt_out
* XDY_Octet		va_CleEvt_out
* XDY_Octet		va_Type_out
* 
* Service rendu
* Parcoure l'arbre des consequences d'un evenement (en profondeur)
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* date nulle -> date courante
* En formation, preciser l'horodate
* Ne pas renseigner le parametre va_Effacement_in, il est utilise 
* en interne pour une autre procedure
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP. (via XZAE;70)
* 
* 
* Fonction
* appelle XZAE70 recursivement et construit la liste 
* ordonnee des consequences
------------------------------------------------------*/

use PRC
go

drop table LISTE_CONSEQ
go

/*A creation d'une table de stockage des consequences : arbre de consequences */
create table LISTE_CONSEQ (	numero int not null, cle tinyint , type smallint null, 
				numero_pere int not null, cle_pere tinyint not null, 
				profondeur int null, spid int not null)

go

create procedure XZAE;68
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Horodate_in		datetime = null,
	@va_Effacement_in	tinyint	= XZAEC_EffacDonnees
as
	declare @vl_NumConseq int, @vl_CleConseq tinyint,  @vl_Type smallint,
		@vl_NumPere int, @vl_ClePere tinyint, @vl_Profondeur int, @vl_Status int
	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A si date nulle : date courante */
	if @va_Horodate_in = null
		select @va_Horodate_in=getdate()

	/*A initialisation e la profondeur de depart */
	select @vl_Profondeur = 1

	/*A initialisation de la table de travail */
	delete LISTE_CONSEQ
	where spid = @@spid

	/*A premiere liste de consequences de l'evenement inseree dans l'arbre */
	exec @vl_Status = XZAE;70 @va_NumEvt_in , @va_CleEvt_in, @va_Horodate_in , 
				  XZAEC_MemoriserConseq

	if @vl_Status != XDC_OK
		return @vl_Status

	/*A mise a jour de la profondeur des elements ajoutes */
	update LISTE_CONSEQ
	set profondeur = @vl_Profondeur
	where profondeur = null
	and spid = @@spid

	/*A creation d'un curseur pour parcourir les liste de consequences */
	declare Pointeur_liste cursor 
	for select numero, cle
	from LISTE_CONSEQ
	where spid = @@spid
	
	/*A recherche et ajout des consequences dans l'arbre */
	open Pointeur_liste
	fetch Pointeur_liste into @vl_NumConseq, @vl_CleConseq
	while (@@sqlstatus = 0)
	begin
		/*B ajout dans l'arbre des consequence la liste suivante de consequences */
		exec @vl_Status = XZAE;70 @vl_NumConseq, @vl_CleConseq, @va_Horodate_in , 
					XZAEC_MemoriserConseq

		/*B test si la procedure s'est bien deroulee */
		if @vl_Status != XDC_OK
		begin
			delete LISTE_CONSEQ where spid = @@spid
			return @vl_Status
		end


		/*B recherche profondeur du pere des evenements ajoutes */
		select @vl_Profondeur = profondeur
		from LISTE_CONSEQ
		where numero = @vl_NumConseq and cle = @vl_CleConseq and spid = @@spid

		/*B test si bouclage possible (une consequence a deux causes ) */
		/*! ne doit pas arriver : sinon erreur codage                  */
		if @@rowcount != 1
			return XDC_NOK

		/*B mise a jour de la profondeur des elements ajoutes */
		update LISTE_CONSEQ
		set profondeur = @vl_Profondeur + 1
		from LISTE_CONSEQ
		where 	numero_pere = @vl_NumConseq and cle_pere = @vl_CleConseq and 
			profondeur = null and spid = @@spid

		/*B passage a la consequence suivante */
		fetch Pointeur_liste into @vl_NumConseq, @vl_CleConseq
	end

	close Pointeur_liste

	if @va_Effacement_in = XZAEC_EffacDonnees
	begin
		/*A renvoi de l'arbre de consequences */
		select numero, cle, type, numero_pere, cle_pere, profondeur 
		from LISTE_CONSEQ where spid = @@spid

		/* vider la table de stockage de l'arbre */
		delete LISTE_CONSEQ where spid = @@spid
	end

	return XDC_OK
go
